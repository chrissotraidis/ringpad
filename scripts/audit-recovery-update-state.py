#!/usr/bin/env python3
"""Check retained state after one original update in the supplied startup fixture.

This is a bounded consistency audit, not normal boot, collision response,
general floating-point correctness, cleanup, or gameplay acceptance.
"""
import hashlib
import json
import math
from pathlib import Path
import struct
import sys


def audit(capture):
    before = (capture / 'collision-ram.bin').read_bytes()
    after = (capture / 'update-ram.bin').read_bytes()
    assert len(before) == len(after) == 0x1000000
    log = (capture / 'execute-provider.log').read_text()
    assert 'UPDATE STATE returned' in log
    assert ('Original update traversal returned; inspect state before acceptance' in log
            or 'PASS original full update:13 root links;' in log)

    def read(data, address, fmt='<I'):
        offset = address - 0x8c000000
        assert 0 <= offset <= len(data) - struct.calcsize(fmt)
        return struct.unpack_from(fmt, data, offset)

    def u(data, address):
        return read(data, address)[0]

    def groups(data):
        result, seen = [], set()
        for group in range(8):
            node, previous, chain = u(data, 0x8c7a32cc + 4*group), 0, []
            while node:
                assert 0x8ce40010 <= node < 0x8ce40010 + 4096*48
                assert (node - 0x8ce40010) % 48 == 0 and node not in seen
                seen.add(node)
                assert u(data, node+4) == previous
                chain.append(node)
                previous, node = node, u(data, node)
            result.append(chain)
        return result

    initial, final = groups(before), groups(after)
    assert initial == final and sum(map(len, final)) == 13
    deferred = []
    for chain in initial:
        for task in chain:
            old, new = u(before, task+16), u(after, task+16)
            # Original 01B4D8 -> 09A426 schedules destruction for a later pass.
            if old == 0x8c01b4d8:
                assert new == 0x8c09a2fc
                deferred.append(task)
            else:
                assert new == old
    assert len(deferred) == 2 and u(after, 0x8c1c5a78) == 0
    timer = u(before, initial[0][0]+36)
    assert u(before, initial[0][0]+16) == 0x8c0a2b38
    assert u(before, timer+4) == 8 and u(after, timer+4) == 7

    main, work = u(after, 0x8c790688), u(after, 0x8c7906e8)
    position = read(after, main+32, '<3f')
    velocity = read(after, work+56, '<3f')
    previous_position = read(before, main+32, '<3f')
    assert all(math.isfinite(v) for v in position + velocity)
    f32 = lambda v: struct.unpack('<f', struct.pack('<f', v))[0]
    assert position == tuple(f32(p+v) for p, v in zip(previous_position, velocity))

    effect = u(after, 0x8ce40040+32)
    assert read(before, effect+1, '<B') == (0,)
    assert read(after, effect+1, '<B') == (1,)
    assert read(after, effect+6, '<H') == (41,) and u(after, effect+16) == 0

    motion_main, motion = u(after, 0x8ce400d0+32), u(after, 0x8ce400d0+36)
    assert u(after, 0x8c19fdf0) == 0
    assert read(after, motion_main+20, '<3I') == (0, 0, 0)
    assert read(after, motion+8, '<2f') == (0., 0.)
    assert read(after, motion+44, '<2I') == (4096, 4096)

    attachment_main = u(after, 0x8ce400a0+32)
    attachment = u(after, 0x8ce400a0+36)
    assert read(before, attachment_main, '<B') == (0,)
    assert read(after, attachment_main, '<B') == (1,)
    assert u(after, work+112) == attachment
    assert read(after, attachment+4, '<2f') == (0., 0.)
    assert u(after, u(after, attachment+16)+48) == u(after, attachment+20)

    result = {
        'passed': True,
        'scope': 'one supplied-state update; root links, selected retained state and integration consistency only',
        'group_sizes': list(map(len, final)),
        'deferred_destruction': list(map(hex, deferred)),
        'countdown': [8, 7], 'position': position, 'velocity': velocity,
        'effect_active': True, 'effect_timer': 41, 'effect_nodes': 0,
        'attachment_state': [0, 1],
        'normal_startup_verified': False, 'positive_contact_verified': False,
        'cleanup_verified': False, 'gameplay': False,
        'sha256': {name: hashlib.sha256(data).hexdigest() for name, data in
                   [('before', before), ('after', after), ('script', Path(__file__).read_bytes())]},
    }
    (capture / 'update-state-audit.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result))


if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise SystemExit('usage: audit-recovery-update-state.py CAPTURE_DIR')
    audit(Path(sys.argv[1]))
