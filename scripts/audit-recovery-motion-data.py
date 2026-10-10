#!/usr/bin/env python3
"""Validate exact STG01 shape data before applying explicit static data ranges."""
import csv
import importlib.util
import json
import math
from pathlib import Path
import re
import struct
import sys
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('aot',ROOT/'scripts/run-recovery-aot.py')
aot=importlib.util.module_from_spec(spec);spec.loader.exec_module(aot)
IMAGE_SHA='2139e5c46f62eb8d11abeb64f646eea49c30a60c7470644672fb62768bbe05d4'
REFERENCE='https://github.com/X-Hax/sa_tools/blob/5d52a39a04e056b8a7e690ce87386750b4b07058/Libraries/SAModel/Animation.cs'

def main():
    if len(sys.argv)!=3:raise ValueError('usage: audit-recovery-motion-data.py STG01_DECODED PRIOR_AUDIT')
    raw,prior=map(lambda s:Path(s).resolve(strict=True),sys.argv[1:])
    if aot.sha256(raw)!=IMAGE_SHA:raise ValueError('Wrong owner STG01 identity')
    data=raw.read_bytes();base=0x0C900000
    def read(fmt,address):
        offset=(address&0x1FFFFFFF)-base
        if offset<0 or offset+struct.calcsize(fmt)>len(data):raise ValueError('Structure outside image')
        return struct.unpack_from(fmt,data,offset)
    def u32(a):return read('<I',a)[0]
    obj=u32(0x8C901AB0);model=u32(obj+4);count=u32(model+8)
    if obj!=0x0CA2E428 or count!=180:raise ValueError('Consumer object/model contract changed')
    if read('<H',0x8C901A06)[0]!=0xD626 or read('<H',0x8C901A10)[0]!=0xD625 or read('<H',0x8C901A18)[0]!=0x430B or u32(0x8C901AB4)!=0x8C040C8E:
        raise ValueError('Observed shape consumer changed')
    motions=[];ranges=set()
    for header in (0x0CA35CD0,u32(0x8C901AA0),u32(0x8C901AA8)):
        table,frames,flags,fn=read('<IIHH',header)
        if (frames,flags,fn)!=(30,0x30,2):raise ValueError('Unrecognized shape-motion header')
        vertices,normals,nv,nn=read('<4I',table)
        if not 0<nv==nn<=32:raise ValueError('Unrecognized channel counts')
        channels=[]
        for kind,ptr in (('vertex',vertices),('normal',normals)):
            keys=[]
            for index in range(nv):
                frame,array=read('<II',ptr+8*index)
                if frame>=frames:raise ValueError('Frame outside motion')
                values=read('<'+str(count*3)+'f',array)
                if not all(math.isfinite(v) for v in values):raise ValueError('Nonfinite vector')
                if kind=='normal' and not all(abs(sum(v*v for v in values[i:i+3])-1)<0.01 for i in range(0,len(values),3)):
                    raise ValueError('Invalid unit normal')
                ranges.add((array|0x80000000,(array+count*12)|0x80000000))
                keys.append({'frame':frame,'array':hex(array)})
            channels.append({'kind':kind,'table':hex(ptr),'keys':keys})
        motions.append({'header':hex(header),'model_data':hex(table),'frames':frames,'flags':hex(flags),'channels':channels,
                        'observed_consumer_argument':header!=0x0CA35CD0})
    root=aot.LOGS
    manifest={'scope':'Initial-image shape data; no claim about later code writes',
        'image_sha256':IMAGE_SHA,'format_reference':REFERENCE,'object':hex(obj),'model':hex(model),
        'vertex_count':count,'motions':motions,'ranges':[[hex(a),hex(z)] for a,z in sorted(ranges)],
        'unreferenced_motion':'0x0CA35CD0 has matching layout but no consumer proven',
        'source_sha256':aot.sha256(Path(__file__))}
    (root/'motion-data.json').write_text(json.dumps(manifest,indent=2)+'\n')
    control=root/'control.bin';control.write_bytes(struct.pack('<3H',0xE02A,0x000B,0x0009))
    sys.argv=[sys.argv[0],'--coverage',str(control)];aot.main()
    aot.REPORT.update(passed=False,scope='STG01 static data annotation and loss audit; no gameplay',r3_complete=False,
        manifest_sha256=aot.sha256(root/'motion-data.json'),image_sha256=IMAGE_SHA)
    output=root/'annotated';output.mkdir()
    log=aot.run('annotated-closure',[aot.BUILD/'dc_raw_recomp',raw,'--base=0x8C900000','--entry=0x8C900000',
        '--seed=0x8C900000','--max-functions=16384','--closure-passes=32',f'--output={output}',f'--map={output/"functions.csv"}',
        *[f'--data-range=0x{a:X}:0x{z:X}' for a,z in sorted(ranges)],'--no-emit'],timeout=120)
    metrics={k.strip():int(v) for k,v in re.findall(r'^([A-Za-z][A-Za-z0-9 /_-]+):\s+(\d+)\s*$',log,re.M)}
    if '[WARN]' in log:raise ValueError('Closure exhausted its bound')
    coverage=json.loads(aot.run('annotated-coverage',[aot.WORK/'coverage-audit',raw,'0x8C900000',output/'functions.csv',output],timeout=60))
    def addresses(path):return {int(r['address'],0) for r in csv.DictReader(path.open())}
    inside=lambda a:any(start<=a<end for start,end in ranges)
    old_entries=addresses(prior/'functions.csv');new_entries=addresses(output/'functions.csv')
    old_insns=addresses(prior/'instructions.csv');new_insns=addresses(output/'instructions.csv')
    losses=old_entries-new_entries;lost_insns=old_insns-new_insns
    unexpected_entries=sorted(a for a in losses if not inside(a))
    unexpected_insns=sorted(a for a in lost_insns if not inside(a))
    result={'metrics':metrics,'coverage':coverage,'removed_entries':[hex(a) for a in sorted(losses)],
        'added_entries':[hex(a) for a in sorted(new_entries-old_entries)],'removed_instruction_addresses':len(lost_insns),
        'removed_entries_outside_data':[hex(a) for a in unexpected_entries],
        'removed_instructions_outside_data':[hex(a) for a in unexpected_insns],
        'prior_map_sha256':aot.sha256(prior/'functions.csv'),'prior_instructions_sha256':aot.sha256(prior/'instructions.csv')}
    (root/'comparison.json').write_text(json.dumps(result,indent=2)+'\n')
    aot.REPORT['comparison']=result
    if unexpected_entries or unexpected_insns:raise ValueError('Loss outside declared data needs review')
    if metrics.get('Unknown SH-4') or metrics.get('RAW_SH4'):raise ValueError('Undecoded instructions remain')
    if aot.sha256(raw)!=IMAGE_SHA:raise ValueError('Original input changed')
    aot.REPORT.update(passed=True,original_input_unchanged=True)
    print(json.dumps({'coverage':coverage,'removed_entries':len(losses),'removed_instruction_addresses':len(lost_insns),'outside_data_losses':0}),flush=True)
    print('Evidence:',root,flush=True)

if __name__=='__main__':
    aot.LOGS.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:(aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n')
    sys.exit(0 if aot.REPORT['passed'] else 1)
