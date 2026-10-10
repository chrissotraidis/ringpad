#!/usr/bin/env python3
"""Check captured terrain texture transfers and record identity, not gameplay."""
import hashlib
import json
from pathlib import Path
import struct
import sys


def digest(data):
    return hashlib.sha256(data).hexdigest()


def main():
    if len(sys.argv) != 4:
        raise ValueError('usage: audit-recovery-terrain-output.py CAPTURE_DIR DECODED_PVM SBOARD')
    capture, reference, board = map(Path, sys.argv[1:])
    pvm, stage = reference.read_bytes(), board.read_bytes()
    assert digest(pvm) == 'bda0c505eb14c462a04d9889b3520114e9c088be79915fc8718ec7d1d729f5da'
    assert digest(stage) == '428dce0260d89a2e524691456cfb228bdc8cc4247631cd5e850eff69a28ff3ac'
    files = {name: (capture / f'terrain-{name}.bin').read_bytes() for name in
             ('scratch', 'texture-records', 'texture-list', 'records', 'visible', 'vram')}
    records, vram = files['texture-records'], files['vram']
    assert len(records) == 2048 * 68 and len(vram) == 0x800000
    u16 = lambda b, p: struct.unpack_from('<H', b, p)[0]
    u32 = lambda b, p: struct.unpack_from('<I', b, p)[0]
    assert pvm[:4] == b'PVMH' and u16(pvm, 8) == 0x109 and u16(pvm, 10) == 50
    chunk, transfers, extents = 8 + u32(pvm, 4), [], []
    for i in range(50):
        assert pvm[chunk:chunk+4] == b'PVRT'
        fmt, width, height = struct.unpack_from('<IHH', pvm, chunk + 8)
        assert fmt in (0x201, 0x202, 0x401, 0x402) and width == height
        assert width in (32, 64, 128, 256)
        vq = (fmt >> 8) == 4
        # Small-to-large mip chain. VQ includes its 2048-byte codebook;
        # the original VQ surface transfer is rounded to 16 bytes.
        if vq:
            size = 2049 + sum(side * side // 4 for side in
                              (2 ** n for n in range(1, width.bit_length())))
            size = (size + 15) & ~15
        else:
            size = 8 + sum(side * side * 2 for side in
                           (2 ** n for n in range(1, width.bit_length())))
        gid = u32(pvm, 12 + i * 34 + 30)
        matches = [slot for slot in range(2048)
                   if u32(records, slot * 68) == gid and u16(records, slot * 68 + 64)]
        assert matches == [58 + i], (i, 'registration', matches)
        slot = matches[0]
        words = struct.unpack_from('<16I', records, slot * 68)
        offset = words[13]
        dest = offset + (0 if vq else 4)
        assert words[9:12] == (width, height, size), (i, 'surface dimensions/size')
        assert words[3] == (0x80000000 | (0x40000000 if vq else 0)
                            | ((fmt & 255) << 27) | (offset >> 3)), (i, 'texture header')
        assert u32(files['texture-list'], i * 12 + 8) == 0x8C7649F4 + slot * 68
        assert size <= u32(pvm, chunk + 4) - 8 and dest + size <= len(vram)
        expected = pvm[chunk + 16:chunk + 16 + size]
        assert files['scratch'][chunk + 16:chunk + 16 + size] == expected, (i, 'decoded payload')
        assert vram[dest:dest + size] == expected, (i, 'VRAM payload')
        assert all(dest + size <= start or dest >= end for start, end in extents)
        extents.append((dest, dest + size))
        transfers.append({'slot': slot, 'global_id': gid, 'bytes': size, 'destination': hex(dest)})
        chunk += 8 + u32(pvm, chunk + 4)
    assert chunk == len(pvm)
    assert files['records'] == stage[0x1A0:0x1A0 + 315 * 36]
    count = u32(files['visible'], 0)
    assert 0 < count <= 315
    pointers = [u32(files['visible'], 4 + i * 4) for i in range(count)]
    assert len(set(pointers)) == count
    assert all(0 <= p - 0x0C9001A0 < 315 * 36 and (p - 0x0C9001A0) % 36 == 0 for p in pointers)
    result = {'passed': True, 'scope': '50 exact texture payload transfers; source record identity and selected-pointer bounds only',
              'gameplay': False, 'collision_verified': False, 'visibility_math_verified': False,
              'outside_vram_preservation_verified': False, 'texture_count': 50,
              'bytes_verified': sum(t['bytes'] for t in transfers), 'selected_count': count,
              'transfers': transfers, 'selected_indices': [(p - 0x0C9001A0) // 36 for p in pointers],
              'input_sha256': {str(reference): digest(pvm), str(board): digest(stage),
                               **{str(capture / f'terrain-{n}.bin'): digest(b) for n, b in files.items()}},
              'script_sha256': digest(Path(__file__).read_bytes())}
    output = capture / 'terrain-transfer-audit.json'
    output.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({k: result[k] for k in ('passed', 'scope', 'texture_count', 'bytes_verified', 'selected_count')}))


if __name__ == '__main__':
    main()
