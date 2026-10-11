#!/usr/bin/env python3
"""Prove captured no-contact queries clear the verified static terrain mesh.

A sphere swept by displacement d lies inside a sphere of radius r + |d| at
its starting center. Clearance from that larger sphere proves a miss without
reimplementing the game's collision algorithm. Intersecting cases fail closed.
"""
import csv
import hashlib
import json
import math
from pathlib import Path
import struct
import sys


def sub(a, b):
    return tuple(x - y for x, y in zip(a, b))


def dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def distance(point, a, b, c):
    ab, ac, ap = sub(b, a), sub(c, a), sub(point, a)
    n = (ab[1]*ac[2]-ab[2]*ac[1], ab[2]*ac[0]-ab[0]*ac[2], ab[0]*ac[1]-ab[1]*ac[0])
    nn = dot(n, n)
    answers = []
    if nn:
        projection = tuple(x - y * dot(ap, n) / nn for x, y in zip(point, n))
        v = sub(projection, a)
        d00, d01, d11, d20, d21 = dot(ab, ab), dot(ab, ac), dot(ac, ac), dot(v, ab), dot(v, ac)
        det = d00*d11 - d01*d01
        if det > 0:
            x, y = (d11*d20-d01*d21)/det, (d00*d21-d01*d20)/det
            if x >= 0 and y >= 0 and x + y <= 1:
                answers.append(dot(ap, n)**2 / nn)
    for start, end in ((a, b), (b, c), (c, a)):
        edge, v = sub(end, start), sub(point, start)
        denominator = dot(edge, edge)
        t = max(0, min(1, dot(v, edge) / denominator)) if denominator else 0
        answers.append(sum((x-t*y)**2 for x, y in zip(v, edge)))
    return math.sqrt(min(answers))


def main():
    if len(sys.argv) != 2:
        raise ValueError('usage: audit-recovery-collision-misses.py CAPTURE_DIR')
    # Analytic face, edge, vertex, and degenerate-triangle controls.
    a, b, c = (0, 0, 0), (2, 0, 0), (0, 2, 0)
    for point, expected in [((0.5, 0.5, 3), 3), ((1, -1, 2), math.sqrt(5)), ((-2, -2, 0), math.sqrt(8))]:
        assert math.isclose(distance(point, a, b, c), expected, abs_tol=1e-12)
    assert distance((0, 0, 3), a, a, a) == 3
    capture = Path(sys.argv[1])
    ram = (capture / 'collision-ram.bin').read_bytes()
    post = (capture / 'update-ram.bin').read_bytes()
    sha = lambda data: hashlib.sha256(data).hexdigest()
    audit = json.loads((capture / 'collision-geometry-audit.json').read_text())
    assert audit['passed'] and audit['sha256']['ram'] == sha(ram)
    assert len(ram) == len(post) == 0x1000000

    def read(address, size, data=ram):
        offset = (address & 0x1fffffff) - 0x0c000000
        assert 0 <= offset <= len(data) - size
        return data[offset:offset+size]

    u = lambda address: struct.unpack('<I', read(address, 4))[0]
    assert u(0x8c1a269c) == 0
    triangles, seen_objects, seen_nodes = [], set(), set()
    node = u(0x8c02d6e8)
    while node:
        assert node not in seen_objects
        seen_objects.add(node)
        tri = u(node + 16)
        while tri:
            assert tri not in seen_nodes
            seen_nodes.add(tri)
            assert read(tri + 24, 40) == read(tri + 24, 40, post), 'Collision mesh changed during update'
            values = struct.unpack('<9f', read(tri + 24, 36))
            assert all(math.isfinite(x) for x in values)
            triangles.append((values[:3], values[3:6], values[6:]))
            tri = u(tri)
        node = u(node)
    assert len(triangles) == audit['triangles'] == 929 and len(seen_objects) == 12
    rows = list(csv.reader((capture / 'collision-queries.csv').read_text().splitlines()))
    assert 0 < len(rows) <= 64
    results = []
    for row in rows:
        words = [int(x, 16) for x in row]
        assert len(words) == 20
        caller, query, obj, flags, returned, contacts = words[:6]
        before, after = words[6:13], words[13:20]
        values = struct.unpack('<7f', struct.pack('<7I', *before))
        assert all(math.isfinite(x) for x in values) and values[0] > 0
        expanded_radius = values[0] + math.sqrt(sum(x*x for x in values[4:7]))
        nearest = min(distance(values[1:4], *triangle) for triangle in triangles)
        assert nearest - expanded_radius > 0.01, 'This query needs a positive-contact oracle'
        assert returned == contacts == 0, 'Expected collision miss differs'
        assert before == after, 'Miss changed query sphere/displacement'
        results.append({'caller': hex(caller), 'object': hex(obj), 'flags': hex(flags),
                        'query': values, 'nearest_triangle_distance': nearest,
                        'expanded_radius': expanded_radius, 'clearance': nearest-expanded_radius})
    result = {'passed': True, 'scope': 'captured static-mesh no-contact queries only',
              'queries': results, 'triangles': len(triangles), 'positive_contact_verified': False,
              'ground_contact_verified': False, 'normal_startup_verified': False, 'gameplay': False,
              'sha256': {'ram': sha(ram), 'update_ram': sha(post), 'queries': sha((capture/'collision-queries.csv').read_bytes()),
                         'geometry_audit': sha((capture/'collision-geometry-audit.json').read_bytes()), 'script': sha(Path(__file__).read_bytes())}}
    (capture/'collision-misses-audit.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({'passed': True, 'queries': len(results), 'minimum_clearance': min(x['clearance'] for x in results),
                      'positive_contact_verified': False}))


if __name__ == '__main__':
    main()
