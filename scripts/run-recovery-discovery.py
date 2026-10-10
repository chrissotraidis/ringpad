#!/usr/bin/env python3
"""Source-free raw-discovery counterexamples; retains false promotions explicitly."""
import csv
import importlib.util
import json
from pathlib import Path
import struct
import sys
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('aot',ROOT/'scripts/run-recovery-aot.py')
aot=importlib.util.module_from_spec(spec);spec.loader.exec_module(aot)

def fixture(callbacks):
    data=bytearray(0x1000);base=0x8C010000
    def word(at,v):struct.pack_into('<H',data,at,v)
    def long(at,v):struct.pack_into('<I',data,at,v)
    # Root loads a pointer from a data table, then reads the pointed-to word.
    # No JSR/JMP or function-pointer store occurs anywhere in this root.
    for at,value in [(0,0xD103),(2,0x5211),(4,0x6022),(6,0x000B),(8,0x0009)]:word(at,value)
    long(0x10,base+0x800)
    for index,at in enumerate((0x100,0x200,0x300,0x400)):
        long(0x800+index*8,index);long(0x804+index*8,base+at)
        if callbacks:
            # Independent explicit callback control: literal-fed tail veneer.
            word(at,0xD101);word(at+2,0x412B);word(at+4,0x0009)
            long(at+8,base+0x600)
        else:
            # Finite floats with no pointer values. First halves also happen to
            # decode MOV.L PC,R8; JMP R0; ADD #1,R12. They are never called.
            long(at,0x402BD800);long(at+4,0x3F007C01)
            for off in range(8,0x80,4):long(at+off,0x3F800000)
    word(0x600,0xE02A);word(0x602,0x000B);word(0x604,0x0009)
    return data

def main():
    if len(sys.argv)!=1:raise ValueError('usage: run-recovery-discovery.py')
    root=aot.LOGS
    control=root/'control.bin';control.write_bytes(struct.pack('<3H',0xE02A,0x000B,0x0009))
    sys.argv=[sys.argv[0],'--coverage',str(control)];aot.main()
    aot.REPORT.update(passed=False,scope='Raw discovery diagnostic; false positives are not hidden',r3_complete=False)
    aot.REPORT['cases']={}
    for callbacks in (False,True):
        name='callbacks' if callbacks else 'data_only';out=root/name;out.mkdir();aot.LOGS=out
        raw=out/'input.bin';raw.write_bytes(fixture(callbacks))
        aot.run('raw-closure',[aot.BUILD/'dc_raw_recomp',raw,'--base=0x8C010000','--entry=0x8C010000',
            '--max-functions=256','--closure-passes=8',f'--output={out}',f'--map={out/"functions.csv"}','--no-emit'],timeout=30)
        entries={int(r['address'],0) for r in csv.DictReader((out/'functions.csv').open())}
        selected=sorted(entries.intersection({0x8C010100,0x8C010200,0x8C010300,0x8C010400}))
        if callbacks and len(selected)!=4:raise ValueError('Positive callback control lost a veneer')
        result={'input_sha256':aot.sha256(raw),'promoted_table_targets':[hex(x) for x in selected],
            'data_false_promotions':0 if callbacks else len(selected)}
        aot.REPORT['cases'][name]=result;print(name,json.dumps(result),flush=True)
    # Explicit data metadata must remove data-only roots without changing bytes.
    out=root/'annotated';out.mkdir();aot.LOGS=out
    raw=out/'input.bin';raw.write_bytes(fixture(False))
    ranges=[f'--data-range=0x{0x8C010000+at:X}:0x{0x8C010000+at+0x80:X}' for at in (0x100,0x200,0x300,0x400)]
    args=[aot.BUILD/'dc_raw_recomp',raw,'--base=0x8C010000','--entry=0x8C010000','--max-functions=256','--closure-passes=8',f'--output={out}',f'--map={out/"functions.csv"}','--no-emit',*ranges]
    aot.run('annotated-data',args,timeout=30)
    entries={int(r['address'],0) for r in csv.DictReader((out/'functions.csv').open())}
    if entries!={0x8C010000}:raise ValueError('Declared data still promoted or real root lost')
    if aot.sha256(raw)!=aot.REPORT['cases']['data_only']['input_sha256']:raise ValueError('Data bytes changed')
    negatives=[]
    for label,extra,expected in [('explicit-seed',['--seed=0x8C010100'],'explicit seed overlaps'),
                                  ('out-of-bounds',['--data-range=0x8C020000:0x8C020004'],'outside raw image'),
                                  ('empty-range',['--data-range=0x8C010100:0x8C010100'],'outside raw image')]:
        try:aot.run(label,[*args,*extra],timeout=30)
        except RuntimeError as error:
            if expected not in str(error):raise
            negatives.append(label)
        else:raise ValueError('Invalid data contract accepted: '+label)
    # Executable edges must contradict metadata before target filtering.
    for label,words,alias_literal in (
        ('direct-call', [0x4F22,0xB07D,0x0009,0x4F26,0x000B,0x0009], None),
        ('indirect-call-alias', [0x4F22,0xD103,0x410B,0x0009,0x4F26,0x000B,0x0009], 0xAC010100),
        ('branch', [0xA07E,0x0009], None),
    ):
        conflict=bytearray(fixture(False))
        struct.pack_into('<'+str(len(words))+'H',conflict,0,*words)
        if alias_literal is not None:struct.pack_into('<I',conflict,0x10,alias_literal)
        raw.write_bytes(conflict)
        try:aot.run(label+'-data-conflict',args,timeout=30)
        except RuntimeError as error:
            if 'reachable instruction overlaps declared data' not in str(error):raise
            negatives.append(label+'-data-conflict')
        else:raise ValueError('Executable edge silently excluded: '+label)
    aot.REPORT['data_annotation_controls']={'entries':[hex(x) for x in entries],'rejected':negatives}
    aot.LOGS=root
    aot.REPORT.update(passed=True,data_false_positive_reproduced=bool(aot.REPORT['cases']['data_only']['data_false_promotions']),
        source_sha256=aot.sha256(Path(__file__)))
    print('Evidence:',root,flush=True)

if __name__=='__main__':
    root=aot.LOGS;root.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:(root/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n')
    sys.exit(0 if aot.REPORT['passed'] else 1)
