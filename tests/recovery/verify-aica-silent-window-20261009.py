"""Check saved silent AICA timing windows; does not model ARM registers or audio quality."""
import argparse
import json
from pathlib import Path
import struct


def require(condition, message):
    if not condition:
        raise ValueError(message)


def read_capture(prefix):
    prefix = str(prefix)
    hardware = dict(line.split('=', 1) for line in
                    Path(prefix + '-hardware.txt').read_text().splitlines())
    return (hardware, Path(prefix + '-aica-timers.bin').read_bytes(),
            Path(prefix + '-aica-arm-state.bin').read_bytes())


def predict(before, cycles):
    hardware, timers, arm = before
    require(cycles > 0, 'Expected positive SH4 cycle window')
    require(len(timers) == 72 and len(arm) == 208, 'Unsupported capture ABI')
    fixed = {'aica_arm7_enabled': 1, 'aica_arm7_in_reset': 0,
             'aica_arm7_faults': 0, 'aica_arm7_idle_fast_forward': 0,
             'aica_native_active_mask': 0, 'aica_native_nonzero_frames': 0,
             'aica_host_mix_enabled': 0, 'device_clock_enabled': 1,
             'device_clock_host_sync': 0, 'aica_pvr_sync': 0}
    for key, value in fixed.items():
        require(int(hardware[key]) == value, 'Unsupported mode: ' + key)
    require(arm[202] == 0, 'ARM7 is halted')
    sh4_hz = int(hardware['device_clock_sh4_hz'])
    aica_hz = int(hardware['device_clock_aica_hz'])
    divider = int(hardware['aica_native_arm_steps_per_frame'])
    require(sh4_hz > 0 and aica_hz > 0 and divider > 0, 'Invalid clock rate')
    steps, fraction = divmod(cycles * aica_hz +
                            int(hardware['device_clock_aica_fraction']), sh4_hz)
    frames, remainder = divmod(int(hardware['aica_native_audio_step_accumulator'])
                              + steps, divider)
    expected = dict(fixed)
    for key in ['device_clock_sh4_hz', 'device_clock_aica_hz',
                'aica_native_arm_steps_per_frame', 'aica_timer_base_div',
                'aica_timer_rate_num', 'aica_timer_rate_den', 'holly_irq_taken']:
        expected[key] = int(hardware[key])
    expected.update(device_clock_sh4_cycles=int(hardware['device_clock_sh4_cycles']) + cycles,
                    device_clock_aica_steps=int(hardware['device_clock_aica_steps']) + steps,
                    device_clock_aica_fraction=fraction,
                    aica_native_frames=int(hardware['aica_native_frames']) + frames,
                    aica_native_audio_step_accumulator=remainder)
    result = bytearray(timers)
    for offset in (0, 24, 48):
        count, prescale = timers[offset:offset + 2]
        accumulator, overflows = struct.unpack_from('<QQ', timers, offset + 8)
        timer_divider = (max(1, int(hardware['aica_timer_base_div'])) *
                         max(1, int(hardware['aica_timer_rate_den']))) << prescale
        ticks, accumulator = divmod(accumulator + steps *
                                   max(1, int(hardware['aica_timer_rate_num'])), timer_divider)
        # Interrupt delivery across overflow needs a separate ARM path model.
        require(count + ticks < 256, 'Timer overflow is outside this silent-window scope')
        result[offset] = count + ticks
        struct.pack_into('<QQ', result, offset + 8, accumulator, overflows)
    return dict(hardware=expected, timers=bytes(result), steps=steps, frames=frames,
                instructions=struct.unpack_from('<Q', arm, 176)[0] + steps)


def verify(before, after, cycles):
    expected = predict(before, cycles)
    hardware, timers, arm = after
    require(len(timers) == 72 and len(arm) == 208, 'Unsupported return capture ABI')
    for key, value in expected['hardware'].items():
        require(int(hardware[key]) == value, 'Hardware mismatch: ' + key)
    require(timers == expected['timers'], 'AICA timer bytes mismatch')
    require(struct.unpack_from('<Q', arm, 176)[0] == expected['instructions'],
            'ARM7 instruction count mismatch')
    # Includes exception counters, IRQ/FIQ line state and halt flag, not registers.
    require(arm[184:] == before[2][184:], 'ARM7 exception/line/halt state changed')
    return dict(passed=True, SH4_cycles=cycles, AICA_steps=expected['steps'],
                mixer_frames=expected['frames'], timers_verified=True,
                instruction_count_verified=True, full_ARM_register_path_verified=False,
                audio_quality_verified=False)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--before', required=True, help='Capture path prefix')
    parser.add_argument('--after', required=True, help='Capture path prefix')
    parser.add_argument('--cycles', type=int, required=True)
    args = parser.parse_args()
    print(json.dumps(verify(read_capture(args.before), read_capture(args.after),
                            args.cycles), indent=2))
