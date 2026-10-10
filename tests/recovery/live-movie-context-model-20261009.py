"""Composed original context entry/save/restore model, rebased without guest execution."""
from pathlib import Path
import hashlib,json,re,struct,sys
assert len(sys.argv)==5,'usage RAM STATE TIMERS OUTPUT'
root=Path.cwd();base=root/'docs/artifacts/recovery-aot';out=Path(sys.argv[4]);assert not out.exists()
inputs={}
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
def read(p):
 p=Path(p).resolve();inputs[str(p)]=sha(p);return p.read_bytes()
def fields(p):return {k:int(v,16)for k,v in (l.split('=',1)for l in read(p).decode().splitlines())if re.fullmatch('[0-9a-fA-F]+',v)}
state=fields(sys.argv[2]);c={k:v for k,v in state.items()if re.fullmatch('pc|pr|sr|fpscr|vbr|spc|gbr|mach|macl|fpul|dbr|ssr|sgr|r[0-9]+|fr[0-9]+|xf[0-9]+|bank_r[0-9]+',k)}
t={k:v for k,v in fields(sys.argv[3]).items()if k.startswith('tmu')};ram=bytearray(read(sys.argv[1]));elapsed=0;old=c.copy()
assert c['pc']==0x8cecfa98 and c['r3']==0x8c5ffae0
assert all(state.get('holly_pending'+str(i),0)==0 for i in range(3))
def w(a):
 assert a&0xff000000==0x8c000000
 return struct.unpack_from('<I',ram,a&0xffffff)[0]
def put(a,v):
 assert a&0xff000000==0x8c000000
 struct.pack_into('<I',ram,a&0xffffff,v)
def push(v):c['r15']-=4;put(c['r15'],v)
def pop():v=w(c['r15']);c['r15']+=4;return v
def tick(n):
 global elapsed
 elapsed+=n
 for i in range(3):
  if t['tmu_tstr']&(1<<i):
   k=f'tmu{i}_';assert t[k+'lazy_sh4_cycles']==0
   div=16<<(2*(t[k+'tcr']&7));ticks,left=divmod(t[k+'cycle_accumulator']+n,div);assert ticks<=t[k+'tcnt'];t[k+'tcnt']-=ticks;t[k+'ticks']+=ticks;t[k+'cycle_accumulator']=left
image=read(base/'20261005-080717/BOOT.BIN')
for name in ['5FFAE0-20261009-movie-frame-service','5FFB70-20261009-movie-frame-context','5FFA24-20261009-movie-context-helper','5FFA1C-20261009-movie-context-helper']:
 review=root/f'work/dreamcast-recovery/loader-review/startup-producers/{name}/review/function.txt'
 for a in [int(a,16)for a in re.findall(r'^(0x[0-9A-F]+) ',read(review).decode(),re.M)]:assert image[a-0x8c010000:a-0x8c010000+2]==ram[a&0xffffff:(a&0xffffff)+2]
out.mkdir()
def save(stage):
 (out/(stage+'-ram.bin')).write_bytes(ram)
 for name,data in [('state',c),('timers',t)]: (out/(stage+'-'+name+'.txt')).write_text(''.join(f'{k}={v:x}\n'for k,v in data.items()))

# Retained, hash-identified entry arithmetic.
push(c['r0']);push(c['r1']);push(c['sr']);c['r0']=c['sr'];c['r1']=w(0x8c5ffb0c);put(c['r1'],c['r0']);c['r1']=w(0x8c5ffb10);c['r0']|=c['r1'];c['sr']=c['r0'];c['r0']=pop();c['ssr']=c['r0'];c['r0']=c['pr'];c['spc']=c['r0'];c['r0']=w(0x8c5ffb14);put(c['r0'],c['r4']);c['r0']=w(0x8c5ffb18);c['r1']=w(0x8c5ffb1c);put(c['r0'],c['r1']);c['r1']=pop();c['r0']=w(0x8c5ffd90);c['pc']=0x8c5ffae0
assert c['r0']==0x8c5ffb70 and c['r15']==old['r15']-4 and c['ssr']==old['sr'] and c['spc']==old['pr']

tick(29);save('entry')

# Retained, hash-identified save arithmetic.
c['r0']=pop()
for key in ['fpul','fpscr','pr','macl','mach','gbr','spc','ssr']:push(c[key])
push(c['r14'])
for i in range(14,-1,-1):push(c[f'r{i}'])
for i in range(7,-1,-1):push(c[f'bank_r{i}'])
c['r0']=w(w(0x8c5ffcd0));assert c['r0']==0;c['sr']|=1
# FPSCR.FR selects which physical array FMOV accesses; arrays themselves do not swap.
c['r0']=c['fpscr'];c['r1']=w(0x8c5ffcd4);c['r0']&=c['r1'];c['fpscr']=c['r0'];assert c['fpscr']&0x300000==0
for i in range(15,-1,-1):push(c[f'fr{i}'])
c['r1']=w(0x8c5ffcd8);c['r0']|=c['r1'];c['fpscr']=c['r0'];assert c['fpscr']&0x300000==0x200000
for i in range(15,-1,-1):push(c[f'xf{i}'])
c['r0']=w(0x8c5ffd80);push(c['r0']);c['r0']=w(0x8c5ffcdc);c['r1']=w(c['r0']);c['r2']=w(0x8c5ffce0);assert c['r1']!=c['r2'];c['sr']&=~1
c['r4']=c['r1'];c['r5']=c['r15'];c['r0']=w(0x8c5ffce4);assert c['r0']==0x8c5ffa24;c['pr']=0x8c5ffc20
put(c['r4'],c['r5']) # original retained RTS delay slot,3 cycles
c['r0']=w(0x8c5ffce8);c['r4']=w(c['r0']);c['r0']=w(0x8c5ffcdc);put(c['r0'],c['r4']);c['r0']=w(0x8c5ffcec);c['pr']=0x8c5ffc2e;c['pc']=0x8c5ffc20
assert c['r0']==0x8c5ffa1c

tick(111);save('save')

# Retained, hash-identified restore arithmetic.
c['r0']=w(c['r4']) # original5FFA1C return delay slot
c['r15']=c['r0'];new_sp=c['r15'];c['r0']=pop();c['r1']=w(0x8c5ffcf0);assert c['r0']==c['r1']==0;c['sr']|=1
c['r0']=c['fpscr'];c['r1']=w(0x8c5ffcf4);c['r0']&=c['r1'];c['r1']=w(0x8c5ffcd8);c['r0']|=c['r1'];c['fpscr']=c['r0'];assert c['fpscr']&0x300000==0x200000
for i in range(16):c[f'xf{i}']=pop()
c['r1']=w(0x8c5ffcf8);c['r0']&=c['r1'];c['fpscr']=c['r0'];assert c['fpscr']&0x300000==0
for i in range(16):c[f'fr{i}']=pop()
for i in range(8):c[f'bank_r{i}']=pop()
for i in range(15):c[f'r{i}']=pop()
c['r14']=pop()
for key in ['ssr','spc','gbr','mach','macl','pr','fpscr','fpul']:c[key]=pop()
assert (c['sr']^c['ssr'])&0x20000000==0
c['sr']=c['ssr'];c['pc']=c['spc'];assert c['pc']==0x8cecf9f2 and c['r15']==new_sp+260

tick(89);save('restore')
assert elapsed==229
metadata=root/'work/dreamcast-recovery/live-movie-context-model-fragments.json'
for f in json.loads(read(metadata)):
 assert sha(f['path'])==f['source_sha256'];inputs[f['path']]=f['source_sha256']
inputs[str(Path(__file__).resolve())]=sha(__file__)
r=dict(passed=True,cycles=elapsed,next_missing=f"{c['pc']:08X}",restored_stack=f"{c['r15']:08X}",guest_execution=False,inputs_sha256=inputs)
(out/'report.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items()if k!='inputs_sha256'}))
