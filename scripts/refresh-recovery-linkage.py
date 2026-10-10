#!/usr/bin/env python3
"""Reclassify existing module audits against one identified resident image."""
import argparse
from collections import Counter, defaultdict
import csv
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def rows(path):
    with path.open() as f:
        return list(csv.DictReader(f))


def canonical(address):
    physical = address & 0x1FFFFFFF
    if address >> 29 in (0, 4, 5) and 0x0C000000 <= physical < 0x0D000000:
        return physical | 0x80000000
    return address


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('inventory', type=Path)
    p.add_argument('resident', type=Path)
    p.add_argument('output', type=Path)
    p.add_argument('--motion-audit', type=Path)
    a = p.parse_args()
    output = a.output.resolve()
    if not any(output.is_relative_to(ROOT / d) for d in ('work', 'docs/artifacts')):
        p.error('Output must be private ignored evidence')
    inventory = json.loads(a.inventory.read_text())
    resident = json.loads((a.resident / 'report.json').read_text())
    if not inventory['passed'] or not resident['passed']:
        raise ValueError('Input audit incomplete')
    boot = Path(resident['boot']['path'])
    if sha(boot) != resident['boot']['sha256'] or sha(boot) != inventory['boot_sha256']:
        raise ValueError('Resident identity differs from inventory')
    resident_entries = {int(r['address'], 0) for r in rows(a.resident / 'functions.csv')}
    base, end = 0x8C010000, 0x8C010000 + boot.stat().st_size
    inputs = {str(a.inventory.resolve()): sha(a.inventory),
              str((a.resident / 'functions.csv').resolve()): sha(a.resident / 'functions.csv')}
    if a.motion_audit:
        report_path = a.motion_audit / 'report.json'
        motion = json.loads(report_path.read_text())
        comparison = motion['comparison']
        if not motion['passed'] or comparison['removed_entries_outside_data'] or comparison['removed_instructions_outside_data']:
            raise ValueError('Motion loss audit incomplete')
        matches = [m for m in inventory['modules'] if m['file'] == 'SONICADV/STG01.PRS']
        if len(matches) != 1 or matches[0]['decoded_sha256'] != motion['image_sha256']:
            raise ValueError('Motion input mismatch')
        module = matches[0]
        module['superseded_evidence'] = module['evidence']
        module['evidence'] = str((a.motion_audit / 'annotated').resolve())
        module['coverage'] = comparison['coverage']
        module['closure_metrics'] = comparison['metrics']
        module['annotation_report'] = str(report_path.resolve())
        inputs[str(report_path.resolve())] = sha(report_path)
    counts, totals = Counter(), Counter()
    candidates, records = defaultdict(list), []
    for module in inventory['modules']:
        raw = Path(module['directory']) / 'decoded.bin'
        if sha(raw) != module['decoded_sha256']:
            raise ValueError('Module input changed: ' + module['file'])
        evidence = Path(module.get('evidence') or module['prior_evidence'])
        local = {int(r['address'], 0) for r in rows(evidence / 'functions.csv')}
        calls = rows(evidence / 'external-calls.csv')
        if len(calls) != module['coverage']['external_call_records']:
            raise ValueError('Module report/call count mismatch')
        for filename in ('functions.csv', 'instructions.csv', 'external-calls.csv'):
            path = evidence / filename
            inputs[str(path.resolve())] = sha(path)
        for key in ('functions', 'unique_instruction_addresses', 'external_call_records'):
            totals[key] += module['coverage'][key]
        module_counts = Counter()
        for call in calls:
            target = canonical(int(call['target'], 0))
            kind = 'outside_module_and_resident_map'
            if call['resolved'] != '1':
                kind = 'unresolved'
            elif target in resident_entries:
                kind = 'resident_known_entry'
            elif base <= target < end:
                kind = 'resident_candidate_not_in_map'
                candidates[hex(target)].append({'module': module['file'], 'caller': call['caller'], 'site': call['site']})
            elif int(module['base'], 0) <= target < int(module['base'], 0) + module['decoded_size'] and target not in local:
                kind = 'local_candidate_not_in_map'
            counts[kind] += 1
            module_counts[kind] += 1
            records.append(dict(module=module['file'], **call, canonical_target=hex(target), classification=kind))
        module['external_linkage'] = dict(module_counts)
    inventory['linkage_refresh'] = {'resident': str(a.resident.resolve()), 'source_sha256': sha(Path(__file__))}
    output.mkdir(parents=True, exist_ok=False)
    report = {'scope': 'Static candidate comparison; no automatic seeding or execution proof',
              'input_sha256': inputs, 'source_sha256': sha(Path(__file__)),
              'module_count': len(inventory['modules']), 'totals': dict(totals),
              'aggregate_external_records': dict(counts), 'resident_missing_candidates': dict(candidates)}
    (output / 'module-linkage.json').write_text(json.dumps(report, indent=2) + '\n')
    (output / 'stage-inventory-linked.json').write_text(json.dumps(inventory, indent=2) + '\n')
    with (output / 'module-links.csv').open('w') as f:
        writer = csv.DictWriter(f, fieldnames=list(records[0]))
        writer.writeheader()
        writer.writerows(records)
    print(json.dumps({'totals': dict(totals), 'classifications': dict(counts), 'resident_candidates': len(candidates)}, indent=2))


if __name__ == '__main__':
    main()
