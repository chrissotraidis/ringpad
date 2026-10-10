"""Regress the scoped AICA checker and freeze the pending video-copy timing prediction."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[2]
BASE = ROOT / 'docs/artifacts/recovery-aot'
OUT = BASE / '20261009-video-payload-aica-verifier-preparation'
SOURCE = Path(__file__).with_name('verify-aica-silent-window-20261009.py')


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    assert not OUT.exists()
    assert shutil.disk_usage(ROOT).free >= 419430400
    spec = importlib.util.spec_from_file_location('aica_window', SOURCE)
    checker = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(checker)
    identities = {str(SOURCE): sha(SOURCE), str(Path(__file__).resolve()): sha(__file__)}
    pins = {
        ROOT / 'work/dreamcast-recovery/arm64-fpu-audio-domain-candidate/dc_runtime.cpp':
        'cb7576f9ff6a06afc145a98fba293c996da43ffa9cc9973cf124e4ff27c6b1cf',
        BASE / '20261006-235341/provider-generated/dc_arm7.hpp':
        '677e97ab2cd28f0e2fa2e9d4e392a44990fa0084b879cf102fd69f9e8bb342cc',
        BASE / '20261006-235341/provider-generated/dc_runtime.hpp':
        '5c46a70b319c56663e6e249a20b397c76683d56dd8b8d855d975f5b85acd2079',
    }
    for path, digest in pins.items():
        assert sha(path) == digest
        identities[str(path)] = digest
    cases = [
        ('20261009-original-startup-live-movie-state-two',
         'live-movie-state-two-checkpoint.json',
         'e464d00f9ec901ede55a653256177b85656c390706b7fc5b7828e878991ba439',
         'state-two-live-entry', 'state-two-live-repeat', 5962, 1),
        ('20261009-original-startup-live-movie-stream-advance',
         'live-movie-stream-advance-checkpoint.json',
         '673b81137bd8cf15e0aef4f8420823f192bc6c56d1ff8096de9066cf45b8793c',
         'stream-advance-live-entry', 'stream-advance-live-return', 119, 0),
    ]
    positives, negatives = [], []

    def sealed_capture(directory, phase, sealed):
        prefix = BASE / directory / ('original-startup-' + phase)
        for suffix in ('hardware.txt', 'aica-timers.bin', 'aica-arm-state.bin'):
            path = Path(str(prefix) + '-' + suffix)
            assert sha(path) == sealed[str(path)]
            identities[str(path)] = sealed[str(path)]
        return checker.read_capture(prefix)

    for directory, checkpoint, digest, entry, end, cycles, frames in cases:
        checkpoint = BASE / directory / checkpoint
        assert sha(checkpoint) == digest
        identities[str(checkpoint)] = digest
        sealed = json.loads(checkpoint.read_text())['identities']
        before = sealed_capture(directory, entry, sealed)
        after = sealed_capture(directory, end, sealed)
        result = checker.verify(before, after, cycles)
        assert result['mixer_frames'] == frames
        positives.append(dict(capture=directory, **result))
        mutations = []
        for key in ('device_clock_aica_fraction', 'device_clock_aica_steps',
                    'aica_native_frames', 'aica_native_audio_step_accumulator',
                    'aica_native_nonzero_frames', 'aica_timer_rate_num'):
            changed = list(copy.deepcopy(after))
            changed[0][key] = str(int(changed[0][key]) + 1)
            mutations.append((key, before, changed))
        for label, part, offset in [('timer counter', 1, 0),
                                    ('timer accumulator', 1, 8),
                                    ('timer overflow', 1, 16),
                                    ('instruction count', 2, 176),
                                    ('exception count', 2, 184),
                                    ('halt flag', 2, 202)]:
            changed = list(copy.deepcopy(after))
            blob = bytearray(changed[part]); blob[offset] ^= 1
            changed[part] = bytes(blob)
            mutations.append((label, before, changed))
        changed = list(copy.deepcopy(before))
        changed[0]['aica_host_mix_enabled'] = '1'
        mutations.append(('unsupported host timing', changed, after))
        changed = list(copy.deepcopy(before))
        blob = bytearray(changed[1]); blob[0] = 255
        blob[8:16] = (1023).to_bytes(8, 'little')
        changed[1] = bytes(blob)
        mutations.append(('timer overflow scope', changed, after))
        for label, changed_before, changed_after in mutations:
            try:
                checker.verify(changed_before, changed_after, cycles)
            except ValueError as error:
                negatives.append(dict(capture=directory, mutation=label, rejection=str(error)))
            else:
                raise AssertionError('Mutation was accepted: ' + label)
    # Latest native endpoint supplies only the BEFORE state for the pending run.
    before = sealed_capture(cases[-1][0], 'failure', sealed)
    predicted = checker.predict(before, 3319)
    assert predicted['steps'] == 374 and predicted['frames'] == 1
    assert predicted['hardware']['device_clock_aica_fraction'] == 193574400
    assert predicted['hardware']['aica_native_audio_step_accumulator'] == 233
    OUT.mkdir()
    (OUT / 'expected-aica-timers.bin').write_bytes(predicted.pop('timers'))
    (OUT / 'expected-aica-summary.json').write_text(json.dumps(predicted, indent=2) + '\n')
    report = dict(passed=True, scope=__doc__, inputs_sha256=identities,
                  historical_capture_checks=positives, rejected_mutations=negatives,
                  native_video_copy_executed=False, native_video_copy_verified=False,
                  prediction_pending_native_comparison=True,
                  full_ARM_register_path_verified=False, audio_quality_verified=False,
                  G1_resolved=False, free_bytes=shutil.disk_usage(ROOT).free)
    (OUT / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    assert all(sha(path) == digest for path, digest in identities.items())
    all_ids = dict(identities)
    all_ids.update({str(path): sha(path) for path in OUT.iterdir()})
    (OUT / 'aica-verifier-checkpoint.json').write_text(
        json.dumps(dict(identities=all_ids), indent=2) + '\n')
    print(json.dumps(dict(passed=True, historical_windows=len(positives),
                          rejected_mutations=len(negatives),
                          checkpoint_sha256=sha(OUT / 'aica-verifier-checkpoint.json'),
                          pending_native_prediction=predicted), indent=2))


if __name__ == '__main__':
    main()
