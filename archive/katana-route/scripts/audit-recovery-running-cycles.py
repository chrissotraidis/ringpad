#!/usr/bin/env python3
"""Audit two supplied original update/endframe cycles; not normal gameplay."""
import hashlib
import json
import math
from pathlib import Path
import re
import struct
import sys

NAMES = ('loop-before-update-ram.bin', 'loop-update-ram.bin', 'loop-endframe-ram.bin',
         'cycle-2-before-update-ram.bin', 'cycle-2-update-ram.bin', 'cycle-2-endframe-ram.bin')


def word(data, address):
    offset = address - 0x8c000000
    assert 0 <= offset <= len(data)-4
    return struct.unpack_from('<I', data, offset)[0]


def groups(data):
    result, seen = [], set()
    for group in range(8):
        node, previous, chain = word(data, 0x8c7a32cc+4*group), 0, []
        while node:
            assert 0x8ce40010 <= node < 0x8ce40010+4096*48
            assert (node-0x8ce40010) % 48 == 0 and node not in seen
            seen.add(node)
            assert word(data, node+4) == previous
            chain.append(node)
            previous, node = node, word(data, node)
        result.append(chain)
    return result


def check(captures, log):
    assert all(len(data) == 0x1000000 for data in captures)
    initial = groups(captures[0])
    assert sum(map(len, initial)) == 13
    deferred = [task for chain in initial for task in chain
                if word(captures[0], task+16) == 0x8c09a2fc]
    assert len(deferred) == 2
    retained = [[task for task in chain if task not in deferred] for chain in initial]
    assert all(groups(data) == retained for data in captures[1:])
    for data in captures[1:]:
        for task in deferred:
            assert all(word(data, task+offset) == 0 for offset in (16, 20, 24))
    task = initial[0][0]
    assert word(captures[0], task+16) == 0x8c0a2b38
    timer = word(captures[0], task+36)
    timers = [word(data, timer+4) for data in captures]
    assert timers == [6, 4, 4, 4, 2, 2], 'Two updates per cycle not reflected in timer'
    assert captures[2] == captures[3], 'Guest RAM changed between consecutive cycles'
    positions = []
    for data in captures:
        main = word(data, 0x8c790688)
        xyz = struct.unpack_from('<3f', data, main-0x8c000000+32)
        assert all(math.isfinite(value) for value in xyz)
        positions.append(xyz)
        assert word(data, 0x8c7649f0) == 0  # This does NOT exercise normal running mode4.
    assert positions[0] != positions[1] and positions[3] != positions[4]
    assert positions[1] == positions[2] == positions[3] and positions[4] == positions[5]
    for data in (captures[2], captures[5]):
        assert [word(data, address) for address in
                (0x8c758f3c, 0x8c758f40, 0x8c758f44, 0x8c758f48)] == [2, 1, 2, 2]
    clock = []
    for line in log.splitlines():
        if line.startswith('CADENCE CLOCK'):
            row = dict(re.findall(r'(\w+)=([^ ]+)', line))
            clock.append(row)
    cycles = [int(row['cycles']) for row in clock]
    assert cycles and all(a <= b for a, b in zip(cycles, cycles[1:])), 'Clock moved backwards'
    entries = [row for row in clock if row['phase'] == 'callback-entry']
    returns = [row for row in clock if row['phase'] == 'callback-return']
    assert [int(row['counter']) for row in entries] == [2, 1, 2, 1, 2]
    assert [int(row['counter']) for row in returns] == [1, 0, 1, 0, 1]
    assert [int(row['ticks']) for row in returns] == [1, 2, 3, 4, 5]
    ends = [row for row in clock if row['phase'] == 'endframe-return']
    assert len(ends) == 2 and [int(row['ticks']) for row in ends] == [3, 5]
    for cycle in (1, 2):
        assert log.count(f'PASS supplied original running/endframe cycle={cycle} ') == 1
    frame_results = re.findall(r'FRAME RESULT logical=(\d+) completes=(\d+) vblanks=(\d+) flips=(\d+)', log)
    assert [tuple(map(int, row[:2])) for row in frame_results[-2:]] == [(4, 4), (5, 5)]
    return {'timers': timers, 'positions': positions, 'root_task_counts': [13, 11], 'deferred_removed': list(map(hex, deferred)),
            'endframe_cycles': [int(row['cycles']) for row in ends], 'endframe_ticks': [3, 5]}


def main():
    capture = Path(sys.argv[1])
    data = [(capture/name).read_bytes() for name in NAMES]
    log = (capture/'execute-provider.log').read_text()
    result = check(data, log)
    broken = list(data)
    altered = bytearray(data[-1])
    timer = word(data[0], groups(data[0])[0][0]+36)
    struct.pack_into('<I', altered, timer-0x8c000000+4, 3)
    broken[-1] = altered
    reverse_clock = re.sub(r'(phase=endframe-return.*?cycles=)\d+', r'\g<1>0', log, count=1)
    for inputs, text in ((broken, log), (data, reverse_clock)):
        try:
            check(inputs, text)
        except AssertionError:
            pass
        else:
            raise AssertionError('Corruption control accepted')
    files = [capture/name for name in NAMES] + [capture/'execute-provider.log', Path(__file__)]
    result.update(passed=True, negative_controls_rejected=2,
                  scope='Two supplied cycles; retained task links, timer, finite changing positions, continuous clock and cadence reload only',
                  normal_startup_verified=False, running_mode4_verified=False,
                  camera_contact_verified=False, gameplay_verified=False,
                  identities={str(path.resolve()): hashlib.sha256(path.read_bytes()).hexdigest() for path in files})
    (capture/'running-cycles-audit.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({key: result[key] for key in ('passed', 'timers', 'endframe_cycles', 'negative_controls_rejected')}))


if __name__ == '__main__':
    main()
