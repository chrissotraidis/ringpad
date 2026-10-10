#!/usr/bin/env python3
"""Independent static-terrain neighborhood oracle for the one-player fixture."""
import hashlib
import json
import math
from pathlib import Path
import struct
import sys


def main():
    if len(sys.argv) != 3:
        raise ValueError('usage: audit-recovery-collision-selection.py CAPTURE_DIR SBOARD')
    capture, board = map(Path, sys.argv[1:])
    source = board.read_bytes()
    sha = lambda data: hashlib.sha256(data).hexdigest()
    assert sha(source) == '428dce0260d89a2e524691456cfb228bdc8cc4247631cd5e850eff69a28ff3ac'
    ram = (capture / 'collision-ram.bin').read_bytes()
    assert len(ram) == 0x1000000

    def read(address, size):
        offset = (address & 0x1FFFFFFF) - 0x0C000000
        assert 0 <= offset <= len(ram) - size, hex(address)
        return ram[offset:offset + size]

    u32 = lambda address: struct.unpack('<I', read(address, 4))[0]
    u16 = lambda address: struct.unpack('<H', read(address, 2))[0]
    f32 = lambda address: struct.unpack('<f', read(address, 4))[0]
    assert u32(0x8C75D774) == 0x0C902DEC and u32(0x8C1A26B0) == 1
    player, second = u32(0x8C790688), u32(0x8C79068C)
    assert player and second == 0, 'Oracle covers one live player only'
    position = struct.unpack('<3f', read(player + 32, 12))
    radius = f32(0x8C1A26A4)
    assert all(math.isfinite(v) for v in (*position, radius)) and radius > 0
    assert u32(0x8C1A269C) == 0, 'Dynamic terrain requires a separate oracle'
    records = source[0x1A0:0x1A0 + 315 * 36]
    assert read(0x0C9001A0, len(records)) == records, 'Source records changed'
    expected, margins = [], []
    for i in range(315):
        record = records[i * 36:(i + 1) * 36]
        flags = struct.unpack_from('<I', record, 32)[0]
        if not flags & 0x00400003:
            continue
        x, y, z, sphere_radius = struct.unpack_from('<4f', record)
        assert all(math.isfinite(v) for v in (x, y, z, sphere_radius))
        # Geometric sphere overlap in independent double precision. Refuse
        # near-boundary cases rather than assume SH-4 rounding equivalence.
        margin = sphere_radius + radius - math.dist((x, y, z), position)
        margins.append(abs(margin))
        assert abs(margin) > 0.001, 'Ambiguous float boundary needs a dedicated case'
        if margin > 0:
            expected.append(i)
    assert 0 < len(expected) < 128, 'Nonempty, unsaturated neighborhood required'
    assert u32(0x8C1A26A0) == len(expected), 'Static selection count differs'
    assert u16(0x8C758F70) == len(expected), 'Candidate count differs'
    for slot, index in enumerate(expected):
        record = records[index * 36:(index + 1) * 36]
        assert read(0x8C75C574 + slot * 36, 36) == record, (slot, 'selected record')
        obj = struct.unpack_from('<I', record, 24)[0]
        flags = struct.unpack_from('<I', record, 32)[0]
        assert read(0x8C758F74 + slot * 12, 12) == struct.pack('<3I', flags, obj, 0), (slot, 'candidate link')
    result = {'passed': True, 'scope': 'one-player static terrain sphere selection and exact candidate links',
              'indices': expected, 'position': position, 'query_radius': radius,
              'minimum_boundary_margin': min(margins), 'collision_list_head': hex(u32(0x8C02D6E8)),
              'triangle_collision_verified': False, 'ground_contact_verified': False,
              'normal_startup_verified': False, 'gameplay': False,
              'sha256': {'ram': sha(ram), 'source': sha(source), 'script': sha(Path(__file__).read_bytes())}}
    (capture / 'collision-selection-audit.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result))


if __name__ == '__main__':
    main()
