#!/usr/bin/env python3
"""Verify the supplied startup's original angle destructor and repeat images.

0B450E saves three model angle triples in work+100;0B44B0 restores them.
This does not establish normal startup, full teardown, or camera correctness.
"""
import hashlib
import json
from pathlib import Path
import struct
import sys


def check_angles(before, updated, cleaned, boot):
    assert len(before) == len(updated) == len(cleaned) == 0x1000000

    def u(data, address):
        return struct.unpack_from('<I', data, address - 0x8c000000)[0]

    task = 0x8ce40070
    work = u(before, task + 36)
    assert u(before, task + 24) == 0x8c0b44b0
    rows = []
    for node, model in enumerate((0x8c1d1b20, 0x8c1d1758, 0x8c1d19e8)):
        assert u(before, work + 88 + node*4) == model
        for component in range(3):
            address = model + 20 + component*4
            saved = u(before, work + 100 + node*12 + component*4)
            original = struct.unpack_from('<I', boot, address - 0x8c010000)[0]
            assert saved == original == u(before, address)
            assert u(cleaned, address) == saved
            rows.append({'address': hex(address), 'saved': saved,
                         'updated': u(updated, address), 'restored': u(cleaned, address)})
    assert u(updated, 0x8c1d1b38) == 61440
    assert u(cleaned, 0x8c1d1b38) != 61440
    return rows


def audit(capture, boot_path):
    boot = boot_path.read_bytes()
    cases = []
    for index in (1, 2):
        case = capture / f'startup-{index}'
        data = [(case / name).read_bytes() for name in
                ('collision-ram.bin', 'update-ram.bin', 'model-cleanup-ram.bin')]
        rows = check_angles(*data, boot)
        corrupted = bytearray(data[2])
        struct.pack_into('<I', corrupted, 0x1d1b38, 61440)
        try:
            check_angles(data[0], data[1], corrupted, boot)
        except AssertionError:
            pass
        else:
            raise AssertionError('Unrestored-yaw negative control accepted')
        cases.append({'case': index, 'angles': rows,
                      'unrestored_yaw_negative_control_rejected': True})
    images = []
    for first, second in ((1, 4), (2, 5), (3, 6)):
        paths = [capture / f'scene-{i}-render.ppm' for i in (first, second)]
        buffers = [p.read_bytes() for p in paths]
        assert buffers[0] == buffers[1], f'Repeat image differs: {first}/{second}'
        images.append({'frames': [first, second], 'identical': True,
                       'sha256': hashlib.sha256(buffers[0]).hexdigest()})
    return {'passed': True, 'scope': 'Supplied startup angle release and exact repeat renders',
            'cases': cases, 'images': images, 'gameplay_verified': False}


if __name__ == '__main__':
    capture, boot = map(Path, sys.argv[1:])
    result = audit(capture, boot)
    (capture / 'model-release-audit.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result))
