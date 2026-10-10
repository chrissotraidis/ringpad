#!/usr/bin/env python3
"""Exact owner-image stage-loader bindings and bounded static module audits."""
import argparse
from collections import Counter
import csv
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('aot',ROOT/'scripts/run-recovery-aot.py')
aot=importlib.util.module_from_spec(spec);spec.loader.exec_module(aot)
BOOT_HASH='68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29'
PRIOR={'MINICART.PRS':'20261005-121751','SBOARD.PRS':'20261005-121754',
       'AL_MAIN.PRS':'20261005-122345','AL_RACE.PRS':'20261005-121755','AL_GARDEN00.PRS':'20261005-122229'}

def bindings(boot,scan):
    data=boot.read_bytes();base=0x8C010000
    def unpack(fmt,address):return struct.unpack_from(fmt,data,(address&0x1FFFFFFF)-0x0C010000)[0]
    def literal(pc,reg):
        w=unpack('<H',pc)
        if w&0xFF00!=0xD000+(reg<<8):raise ValueError(f'Expected PC literal R{reg} at {pc:X}')
        address=((pc+4)&~3)+4*(w&255)
        return unpack('<I',address)
    def filename(address):
        offset=(address&0x1FFFFFFF)-0x0C010000
        if not 0<=offset<len(data):raise ValueError('String outside boot')
        return data[offset:data.index(0,offset,offset+128)].decode('ascii').upper()
    def branch(pc,kind):
        w=unpack('<H',pc)
        if w>>12!=kind:raise ValueError(f'Wrong branch at {pc:X}')
        disp=w&0xFFF
        return pc+4+2*(disp-4096 if disp&2048 else disp)
    # Validate the selector's range, table-base instruction, and common R12 base.
    if unpack('<H',0x8C09EB16)!=0xE12B or unpack('<H',0x8C09EB24)!=0xC708:
        raise ValueError('Stage selector/table contract changed')
    common=literal(0x8C09EB06,12)
    if common!=0x0C900000:raise ValueError('Common load base changed')
    files={Path(r['path']).name:r for r in scan['rows']}
    modules={};stages=[]
    def add(name,dest,stage,site,task_entry=None):
        row=files[name];dest=(dest&0x1FFFFFFF)|0x80000000
        key=(name,dest)
        module=modules.setdefault(key,{'file':row['path'],'index':row['index'],'base':hex(dest),
            'decoded_size':row['decoded_size'],'decoded_sha256':row['decoded_sha256'],
            'encoded_sha256':row['encoded_sha256'],'prefix_kind':row['prefix_kind'],
            'directory':row['private_directory'],'loads':[],'entries':[]})
        module['loads'].append({'stage':stage,'site':hex(site)})
        canonical=((task_entry&0x1FFFFFFF)|0x80000000) if task_entry else None
        if canonical and dest<=canonical<dest+row['decoded_size']:
            module['entries']=sorted(set(module['entries']+[hex(canonical)]))
        return module
    for stage in range(43):
        case=0x8C09EB2C+unpack('<h',0x8C09EB48+2*stage)
        entry=unpack('<I',0x8C1A1B68+4*stage)
        rec={'stage':stage,'case':hex(case),'task_entry':hex(entry)}
        if case==0x8C09F546:
            if entry:raise ValueError('No-load stage has nonzero entry')
            rec['loads']=[];stages.append(rec);continue
        name=filename(literal(case,4))
        if branch(case+2,0xB)!=0x8C09F664 or unpack('<H',case+4)!=0x65C3:raise ValueError('Primary load changed')
        add(name,common,stage,case+2,entry);rec['loads']=[name]
        # Exact reviewed secondary code loads with immediate R4/R5 literals.
        secondary={29:0x8C09F106,32:0x8C09F168,39:0x8C09F3EC,
                   40:0x8C09F4BC,41:0x8C09F4EC,42:0x8C09F516}.get(stage)
        if secondary:
            name=filename(literal(secondary,4));dest=literal(secondary+2,5)
            if branch(secondary+4,0xB)!=0x8C09F664 or unpack('<H',secondary+6)!=9:raise ValueError('Secondary load changed')
            add(name,dest,stage,secondary+4,entry);rec['loads'].append(name)
        # Tikal program is a tail load after two actual selector cases.
        if stage in (19,34):
            pc=0x8C09EF3C if stage==19 else 0x8C09F236
            name=filename(literal(pc,4));dest=literal(pc+2,5)
            if branch(pc+16,0xA)!=0x8C09F664:raise ValueError('Tikal tail load changed')
            add(name,dest,stage,pc+16);rec['loads'].append(name)
        stages.append(rec)
    return stages,list(modules.values())

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('boot',type=Path);parser.add_argument('scan',type=Path)
    parser.add_argument('--audit',action='store_true')
    args=parser.parse_args()
    if aot.sha256(args.boot)!=BOOT_HASH:raise ValueError('Wrong owner boot')
    scan=json.loads(args.scan.read_text())
    if not scan.get('completed') or scan.get('disc_sha256')!='105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533' or scan.get('disc_sha256_after')!=scan.get('disc_sha256'):
        raise ValueError('Incomplete scan or wrong owner disc identity')
    stages,modules=bindings(args.boot,scan)
    report={'scope':'Static stage bindings and bounded module closure; no execution/full coverage claim',
        'passed':False,'r3_complete':False,'boot_sha256':BOOT_HASH,'scan_sha256':aot.sha256(args.scan),
        'script_sha256':aot.sha256(Path(__file__)),'audit_source_sha256':aot.sha256(ROOT/'tests/recovery/coverage.cpp'),'stages':stages,'modules':modules,'remaining':[]}
    root=aot.LOGS
    try:
        for module in modules:
            private=Path(module['directory'])
            if aot.sha256(private/'decoded.bin')!=module['decoded_sha256'] or aot.sha256(private/'encoded.prs')!=module['encoded_sha256']:
                raise ValueError('Candidate identity changed')
            module['root_basis']='stage task table' if module['entries'] else 'load-base code hypothesis; callable root not established'
            decoded=(private/'decoded.bin').read_bytes()
            if len(decoded)!=module['decoded_size']:raise ValueError('Decoded size mismatch')
            module['registration_callbacks']=[]
            for entry in module['entries']:
                offset=int(entry,0)-int(module['base'],0)
                if offset+8>len(decoded):raise ValueError('Stage entry outside image')
                w0,w1,w2,w3=struct.unpack_from('<4H',decoded,offset)
                if w0&0xFF00!=0xD300 or w1&0xFF00!=0xD500 or w2!=0x432B or w3&0xFF00!=0xE600:continue
                def word_literal(word,pc):
                    at=((pc+4)&~3)+4*(word&255)
                    if at+4>len(decoded):raise ValueError('Registration literal outside image')
                    return struct.unpack_from('<I',decoded,at)[0]
                target=word_literal(w0,offset);metadata=word_literal(w1,offset+2)
                at=(metadata&0x1FFFFFFF)-(int(module['base'],0)&0x1FFFFFFF)
                if target!=0x8C0A1690 or not 0<=at<=len(decoded)-24:raise ValueError('Unrecognized registration contract')
                size,kind,*callbacks=struct.unpack_from('<6I',decoded,at)
                if size>0x10000 or kind>0x10000:raise ValueError('Unrecognized registration metadata')
                for callback in callbacks:
                    if not callback:continue
                    callback=(callback&0x1FFFFFFF)|0x80000000
                    if callback&1 or not int(module['base'],0)<=callback<int(module['base'],0)+len(decoded):raise ValueError('Callback outside image')
                    module['registration_callbacks'].append(hex(callback))

            name=Path(module['file']).name
            if name in PRIOR:
                evidence=ROOT/'docs/artifacts/recovery-aot'/PRIOR[name]
                prior=json.loads((evidence/'report.json').read_text())
                if not prior['passed'] or prior['boot']['sha256']!=module['decoded_sha256']:raise ValueError('Invalid prior audit')
                module['prior_evidence']=str(evidence);module['coverage']=prior['coverage']
            print(name,module['base'],module['entries'],module['root_basis'],flush=True)
        selected={m['file'] for m in modules}
        report['unbound_prefix_candidates']=[r['path'] for r in scan['rows'] if r['prefix_kind'] in ('registration_header','stack_prologue_prefix') and r['path'] not in selected]
        if not args.audit:report['passed']=True;aot.REPORT['passed']=True;return
        control=root/'control.bin';control.write_bytes(struct.pack('<3H',0xE02A,0x000B,0x0009))
        sys.argv=[sys.argv[0],'--coverage',str(control)];aot.main()
        report['toolchain_report']='report.json';report['candidate_pin']=aot.PIN
        report['candidate_patches']=aot.REPORT.get('patches')
        reference=aot.WORK/'prs-reference/prs-reference'
        if aot.sha256(reference)!='abff04206838dbe559c62153d8405c654c897ce6a6824643aaccd6ae8994c82d':raise ValueError('Reference decoder identity changed')
        report['reference_sha256']=aot.sha256(reference)
        for module in modules:
            if module.get('prior_evidence'):continue
            name=Path(module['file']).stem;directory=root/name;directory.mkdir()
            aot.LOGS=directory
            raw=Path(module['directory'])/'decoded.bin'
            entries=list(dict.fromkeys((module['entries'] or [module['base']])+module['registration_callbacks']))
            seeds=directory/'entries.txt';seeds.write_text(''.join(e+'\n' for e in entries))
            try:
                # Reference interface takes input, output, expected decoded length.
                aot.run('reference-decode',[reference,raw.with_name('encoded.prs'),directory/'reference.bin',str(module['decoded_size'])],timeout=30)
                if aot.sha256(directory/'reference.bin')!=module['decoded_sha256']:raise ValueError('Reference decode differs')
                output=aot.run('raw-closure',[aot.BUILD/'dc_raw_recomp',raw,f'--base={module["base"]}',f'--entry={entries[0]}',f'--seed-file={seeds}',
                    '--max-functions=16384','--closure-passes=32',f'--output={directory}',f'--map={directory/"functions.csv"}','--no-emit'],timeout=120)
                module['closure_metrics']={k.strip():int(v) for k,v in re.findall(r'^([A-Za-z][A-Za-z0-9 /_-]+):\s+(\d+)\s*$',output,re.M)}
                module['closure_limit_warning']='[WARN]' in output
                module['coverage']=json.loads(aot.run('audit-coverage',[aot.WORK/'coverage-audit',raw,module['base'],directory/'functions.csv',directory,f'--entry={entries[0]}'],timeout=60))
                with (directory/'external-calls.csv').open() as f:module['external_reasons']=dict(Counter(r['reason'] for r in csv.DictReader(f)))
                module['audit_completed']=True;module['evidence']=str(directory)
                print(name,json.dumps(module['coverage']),flush=True)
            except Exception as error:
                module['audit_completed']=False;module['error']=str(error);report['remaining'].append(name)
                print(name,'AUDIT FAILED',str(error)[-500:],flush=True)
            finally:
                if aot.sha256(raw)!=module['decoded_sha256']:raise ValueError('Decoded input changed')
                (root/'stage-inventory.json').write_text(json.dumps(report,indent=2)+'\n')
        report['passed']=not report['remaining']
    finally:
        aot.LOGS=root
        report['original_boot_unchanged']=aot.sha256(args.boot)==BOOT_HASH
        (root/'stage-inventory.json').write_text(json.dumps(report,indent=2)+'\n')
        print('Evidence:',root/'stage-inventory.json',flush=True)
    if not report['passed']:raise ValueError('One or more module audits failed; inspect inventory')

if __name__=='__main__':
    aot.LOGS.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:(aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n')
    sys.exit(0 if aot.REPORT['passed'] else 1)
