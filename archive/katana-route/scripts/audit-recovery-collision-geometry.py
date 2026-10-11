#!/usr/bin/env python3
"""Compare this fixture's collision triangles with unchanged source model data.

Scope is deliberately explicit: one-player selection, leaf models, translation
only, triangle-list meshes. Unsupported transforms/formats fail rather than use
the guest implementation as an oracle. No contact/query acceptance follows.
"""
from collections import Counter
import hashlib
import json
import math
from pathlib import Path
import struct
import sys


def main():
    if len(sys.argv) != 3:
        raise ValueError('usage: audit-recovery-collision-geometry.py CAPTURE_DIR SBOARD')
    capture, board = map(Path, sys.argv[1:])
    ram = (capture / 'collision-ram.bin').read_bytes()
    source = board.read_bytes()
    sha = lambda b: hashlib.sha256(b).hexdigest()
    assert len(ram) == 0x1000000
    assert sha(source) == '428dce0260d89a2e524691456cfb228bdc8cc4247631cd5e850eff69a28ff3ac'
    selection = json.loads((capture / 'collision-selection-audit.json').read_text())
    assert selection['passed'] and selection['sha256']['ram'] == sha(ram)
    assert selection['sha256']['source'] == sha(source)

    def read(address, size):
        offset = (address & 0x1fffffff) - 0x0c000000
        assert 0 <= offset <= len(ram) - size, hex(address)
        return ram[offset:offset + size]

    def src(address, size):
        offset = (address & 0x1fffffff) - 0x0c900000
        assert 0 <= offset <= len(source) - size, hex(address)
        value = source[offset:offset + size]
        assert read(address, size) == value, ('source mutated', hex(address))
        return value

    u32 = lambda a: struct.unpack('<I', read(a, 4))[0]
    rf = lambda a: struct.unpack('<f', read(a, 4))[0]
    sf = lambda a, n: struct.unpack('<' + 'f' * n, src(a, n * 4))
    su = lambda a: struct.unpack('<I', src(a, 4))[0]
    sh = lambda a: struct.unpack('<H', src(a, 2))[0]
    f32 = lambda v: struct.unpack('<f', struct.pack('<f', v))[0]
    centroid_scale = f32(0.3333)  # Original literal028C00, not exact 1/3.
    objects = set(range(0x8c6befbc, 0x8c6befbc + 192 * 64, 64))
    geometry = set(range(0x8c6c1fbc, 0x8c6c1fbc + 8192 * 64, 64))

    def chain(head, pool):
        found = []
        seen = set()
        while head:
            assert head in pool and head not in seen, ('invalid/cyclic link', hex(head))
            seen.add(head)
            found.append(head)
            head = u32(head)
        return found

    active = chain(u32(0x8c02d6e8), objects)
    indices = selection['indices']
    assert len(active) == len(indices)
    used = set()
    rows = []
    max_center_error = max_radius_error = 0.0
    for node, index in zip(reversed(active), indices):
        record = 0x0c9001a0 + index * 36
        obj, flags = su(record + 24), su(record + 32)
        assert u32(node + 8) == obj and u32(node + 12) == flags
        assert src(obj + 20, 12) == bytes(12), 'Rotated model requires another oracle'
        assert sf(obj + 32, 3) == (1, 1, 1), 'Scaled model requires another oracle'
        assert su(obj + 44) == su(obj + 48) == 0, 'Hierarchy requires another oracle'
        translation = sf(obj + 8, 3)
        model = su(obj + 4)
        vertices, vertex_count = su(model), su(model + 8)
        meshsets, materials = su(model + 12), su(model + 16)
        expected = Counter()
        for j in range(sh(model + 20)):
            mesh = meshsets + j * 24
            tag, count, pointer = sh(mesh), sh(mesh + 2), su(mesh + 4)
            assert tag & 0xc000 == 0, 'Non-triangle-list mesh requires another oracle'
            material = su(materials + (tag & 0x3fff) * 20 + 16) if materials else 0
            for triangle in range(count):
                coords = []
                for k in range(3):
                    vertex = struct.unpack('<h', src(pointer + (triangle * 3 + k) * 2, 2))[0]
                    assert 0 <= vertex < vertex_count
                    coords.extend(f32(v + t) for v, t in zip(sf(vertices + vertex * 12, 3), translation))
                assert all(math.isfinite(v) for v in coords)
                expected[struct.pack('<9fI', *coords, material)] += 1
        nodes = chain(u32(node + 16), geometry)
        assert nodes and nodes[-1] == u32(node + 20)
        assert not used.intersection(nodes), 'Triangle shared between object lists'
        used.update(nodes)
        actual = Counter(read(triangle + 24, 40) for triangle in nodes)
        assert actual == expected, ('triangle vertices/material mismatch', index,
                                     sum((actual - expected).values()), sum((expected - actual).values()))
        bins, z_extents = [], []
        previous = 0
        for triangle in nodes:
            assert u32(triangle + 4) == previous, 'Broken reverse triangle link'
            previous = triangle
            values = struct.unpack('<13f', read(triangle + 8, 52))
            center, radius2, coords = values[:3], values[3], values[4:]
            assert all(math.isfinite(v) for v in values)
            points = [coords[i:i + 3] for i in (0, 3, 6)]
            z_extents.append(f32(max(p[2] for p in points) - min(p[2] for p in points)))
            bins.append(max(0, min(127, (int(f32(center[2] - rf(node + 56))) >> 4) + 64)))
            ideal = [sum(p[k] for p in points) * centroid_scale for k in range(3)]
            error = max(abs(a - b) for a, b in zip(center, ideal))
            assert error < 0.001, ('centroid mismatch', hex(triangle), error)
            ideal_radius2 = max(sum((v - c) ** 2 for v, c in zip(p, center)) for p in points)
            radius_error = abs(radius2 - ideal_radius2)
            assert radius_error <= max(0.002, abs(ideal_radius2) * 0.000002), ('radius mismatch', hex(triangle), radius_error)
            max_center_error = max(max_center_error, error)
            max_radius_error = max(max_radius_error, radius_error)
        assert bins == sorted(bins), 'Triangle spatial buckets out of order'
        assert rf(node + 40) == max(z_extents), 'Object Z extent differs'
        expected_center = tuple(f32(v + t) for v, t in zip(sf(model + 24, 3), translation))
        assert struct.unpack('<3f', read(node + 48, 12)) == expected_center, 'Model center differs'
        rows.append({'source_record': index, 'triangles': len(nodes)})
    free_objects = set(chain(u32(0x8c02d6e4), objects))
    free_geometry = set(chain(u32(0x8c02d6e0), geometry))
    assert set(active).isdisjoint(free_objects) and set(active) | free_objects == objects
    assert used.isdisjoint(free_geometry) and used | free_geometry == geometry
    result = {'passed': True, 'scope': 'translation-only leaf triangle meshes: exact vertices/materials, centroid/radius tolerances, bounds, ordered buckets and pool partition',
              'objects': len(active), 'triangles': len(used), 'rows': rows,
              'max_centroid_error': max_center_error, 'max_squared_radius_error': max_radius_error,
              'ground_contact_verified': False, 'normal_startup_verified': False, 'gameplay': False,
              'sha256': {'ram': sha(ram), 'source': sha(source), 'script': sha(Path(__file__).read_bytes())}}
    (capture / 'collision-geometry-audit.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result))


if __name__ == '__main__':
    main()
