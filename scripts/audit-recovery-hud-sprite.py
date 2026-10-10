#!/usr/bin/env python3
"""Check the first original HUD sprite's header and four vertex packets.

The fixed reference is the capture immediately before original60A2F0. This
checks packet construction, not the later DMA, rasterization or displayed HUD.
"""
import hashlib
import json
from pathlib import Path
import struct
import sys


def digest(data):
    return hashlib.sha256(data).hexdigest()


def check(before, after):
    assert digest(before) == '31e88e7e1e982a543a2f19b979a293f38902aeaf21ce56bddd0408066a9c8487'
    assert len(before) == len(after) == 0x1000000
    u = lambda data, offset: struct.unpack_from('<I', data, offset)[0]
    sprite = 0x1c3218
    assert struct.unpack_from('<5fI', before, sprite) == (16., 32., 0., 1., 1., 0)
    frames = u(before, sprite+28)-0x8c000000
    assert struct.unpack_from('<10h', before, frames+11*20) == (64, 64, 0, 0, 0, 0, 256, 256, 11, 32)
    header = (0x828c0028, 0xc8800000, 0x941184db, 0x100bd9f4)
    assert struct.unpack_from('<4I', before, 0x894384) == header
    colors = struct.unpack_from('<4I', before, 0x670ab8)
    assert colors == (0x3f800000,)*4
    # The original store queue destination aliases this main-RAM packet buffer.
    address = u(before, 0x894398)
    offset = (address & 0x1fffffff)-0x0c000000
    assert 0 <= offset <= len(after)-160
    assert after[offset:offset+32] == struct.pack('<8I', *header, *colors), 'HUD header packet differs'
    rows = []
    for i, (x, y, tex_u, tex_v) in enumerate(((16., 32., 0., 0.), (16., 96., 0., 1.),
                                            (80., 32., 1., 0.), (80., 96., 1., 1.))):
        command = 0xf0000000 if i == 3 else 0xe0000000
        expected = struct.pack('<I6f', command, x, y, .5, tex_u, tex_v, 1.)
        start = offset+32+i*32
        assert after[start:start+28] == expected, (i, 'HUD vertex packet differs')
        rows.append({'x': x, 'y': y, 'reciprocal_z': .5, 'u': tex_u, 'v': tex_v})
    return {'packet_address': hex(address), 'packet_offset': offset, 'vertices': rows}


def main():
    capture, reference = map(Path, sys.argv[1:])
    before, after = reference.read_bytes(), (capture/'loop-display-ram.bin').read_bytes()
    result = check(before, after)
    for offset in (result['packet_offset'], result['packet_offset']+32+4):
        corrupt = bytearray(after)
        corrupt[offset] ^= 1
        try:
            check(before, corrupt)
        except AssertionError:
            pass
        else:
            raise AssertionError('HUD packet corruption control accepted')
    result.update(passed=True, scope='First HUD sprite11: one header and four vertices; no rasterization claim',
                  negative_controls_rejected=2, padding_verified=False, rasterization_verified=False,
                  identities={str(reference): digest(before), str(capture/'loop-display-ram.bin'): digest(after),
                              str(Path(__file__)): digest(Path(__file__).read_bytes())})
    (capture/'hud-sprite-audit.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k: result[k] for k in ('passed', 'scope', 'negative_controls_rejected')}))


if __name__ == '__main__':
    main()
