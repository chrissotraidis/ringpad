#!/usr/bin/env python3
"""Compare the original HUD load with independently decoded CON_REGULAR bytes.

Checks transfers and record identity, not sprite rasterization or normal startup.
"""
import hashlib
import json
from pathlib import Path
import struct
import sys


def digest(data):
    return hashlib.sha256(data).hexdigest()


def check(pvm, before, after, vram_before, vram_after):
    assert digest(pvm) == '6645bbddc831f0f9c02b931d5421858d655056e9e3f2f24cb738b3b99b5f9640'
    assert len(before) == len(after) == 0x1000000
    assert len(vram_before) == len(vram_after) == 0x800000
    u16 = lambda b, p: struct.unpack_from('<H', b, p)[0]
    u32 = lambda b, p: struct.unpack_from('<I', b, p)[0]
    assert pvm[:4] == b'PVMH' and u16(pvm, 8) == 0x109 and u16(pvm, 10) == 56
    assert after[0xd00000:0xd00000 + len(pvm)] == pvm
    # Existing character/board/terrain registrations must survive the HUD load.
    assert before[0x7649f4:0x7649f4+108*68] == after[0x7649f4:0x7649f4+108*68]
    expected = bytearray(vram_before)
    chunk, extents, rows, registrations = 8 + u32(pvm, 4), [], [], {}
    gids = [u32(pvm, 12+i*34+30) for i in range(56)]
    assert len(set(gids)) == 54
    for i in range(56):
        assert pvm[chunk:chunk+4] == b'PVRT'
        fmt, width, height = struct.unpack_from('<IHH', pvm, chunk+8)
        assert fmt >> 8 in (1, 2, 9) and fmt & 255 in (0, 1, 2)
        mip = fmt >> 8 == 2
        size = width*height*2
        if mip:
            assert width == height
            size = 8 + sum((2**n)**2*2 for n in range(1, width.bit_length()))
        gid = gids[i]
        fresh = gid not in registrations
        slot = 108+len(registrations) if fresh else registrations[gid]['slot']
        record = 0x7649f4+slot*68
        link = 0x1c6da8+i*12+8
        assert u32(before, link) == 0
        assert u32(before, record) == 0xffffffff and u16(before, record+64) == 0
        assert u32(after, link) == 0x8c000000+record
        assert u32(after, record) == gid
        assert u16(after, record+64) == gids.count(gid)
        assert struct.unpack_from('<3I', after, record+36) == (width, height, size)
        offset = u32(after, record+52)
        dest = offset+(4 if mip else 0)
        assert size <= u32(pvm, chunk+4)-8 and dest+size <= len(expected)
        payload = pvm[chunk+16:chunk+16+size]
        if fresh:
            assert all(dest+size <= start or dest >= end for start, end in extents)
            extents.append((dest, dest+size))
            registrations[gid] = {'slot': slot, 'payload': payload, 'format': fmt,
                                  'width': width, 'height': height, 'destination': dest}
        else:
            assert registrations[gid] == {'slot': slot, 'payload': payload, 'format': fmt,
                                          'width': width, 'height': height, 'destination': dest}
        assert vram_after[dest:dest+size] == payload, (i, 'HUD payload')
        expected[dest:dest+size] = payload
        rows.append({'index': i, 'slot': slot, 'format': hex(fmt), 'new_registration': fresh,
                     'bytes': size, 'destination': dest})
        chunk += 8+u32(pvm, chunk+4)
    assert chunk == len(pvm)
    assert vram_after == expected, 'Unexplained VRAM changes outside HUD payloads'
    assert u32(after, 0x677a58) == u32(after, 0x893920) == 162
    return rows


def main():
    capture, reference = map(Path, sys.argv[1:])
    names = ('hud-before-ram.bin', 'hud-after-ram.bin',
             'hud-before-vram.bin', 'hud-after-vram.bin')
    buffers = [(capture/name).read_bytes() for name in names]
    pvm = reference.read_bytes()
    rows = check(pvm, *buffers)
    # Corruption controls cover a bad permanent link and a changed transferred byte.
    for index, offset in ((1, 0x1c6da8+11*12+8), (3, rows[11]['destination'])):
        corrupted = list(buffers)
        corrupted[index] = bytearray(corrupted[index])
        corrupted[index][offset] ^= 1
        try:
            check(pvm, *corrupted)
        except AssertionError:
            pass
        else:
            raise AssertionError('HUD corruption control accepted')
    result = {'passed': True, 'scope': '56 HUD links / 54 unique payloads; original duplicate-ID reuse; prior records and outside VRAM preserved',
              'bytes_verified': sum(r['bytes'] for r in rows if r['new_registration']), 'transfers': rows,
              'negative_controls_rejected': 2, 'normal_startup_verified': False,
              'sprite_rasterization_verified': False, 'allocator_policy_verified': False,
              'identities': {str(reference): digest(pvm), str(Path(__file__)): digest(Path(__file__).read_bytes()),
                             **{str(capture/n): digest(b) for n, b in zip(names, buffers)}}}
    (capture/'hud-texture-audit.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k: result[k] for k in ('passed', 'scope', 'bytes_verified', 'negative_controls_rejected')}))


if __name__ == '__main__':
    main()
