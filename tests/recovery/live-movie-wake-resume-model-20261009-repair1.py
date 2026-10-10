"""Original wake-resume trampoline and retained context save/restore model; no guest execution."""
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
assert c['pc']==0x8c5ffb60 and c['sr']==0x50000001
assert state['holly_pending0']==0x1000
assert state['holly_pending0']&state['holly_mask1_0']==0x1000
assert c['sr']&0x10000000 # BL blocks the pending priority4 interrupt until RTE.
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
for name in ['5FFB60-20261009-live-movie-wake-resume','5FFB70-20261009-movie-frame-context','5FFA24-20261009-movie-context-helper','5FFA1C-20261009-movie-context-helper']:
 review=root/f'work/dreamcast-recovery/loader-review/startup-producers/{name}/review/function.txt'
 for a in [int(a,16)for a in re.findall(r'^(0x[0-9A-F]+) ',read(review).decode(),re.M)]:assert image[a-0x8c010000:a-0x8c010000+2]==ram[a&0xffffff:(a&0xffffff)+2]
out.mkdir()
def save(stage):
 (out/(stage+'-ram.bin')).write_bytes(ram)
 for name,data in [('state',c),('timers',t)]: (out/(stage+'-'+name+'.txt')).write_text(''.join(f'{k}={v:x}\n'for k,v in data.items()))

# Eight original instructions: push r0, restore saved SSR/SPC, jump5FFB70.
# LDC SSR/SPC have default1 issue cycle; dynamic JMP has2, hence9 total.
runtime_source=read(root/'work/dreamcast-recovery/arm64-fpu-audio-domain-candidate/dc_runtime.cpp')
emitter=read(root/'work/dreamcast-recovery/arm64-fpu-domain-candidate/cpp_emitter.cpp')
assert b'if ((ctx.sr & kSrBl) != 0u) return false;' in runtime_source
common=read(base/'20261009-startup-movie-context-save-repair1/wrapper/generated_program.cpp').decode()
assert common.index('dc_runtime_tick_fast(ctx, runtime, 40u)')<common.index('dc_write_sr(ctx, ctx.ssr)')
push(c['r0']);c['r0']=w(0x8c5ffd84);c['ssr']=c['r0'];c['r0']=w(0x8c5ffd88);c['spc']=c['r0'];c['r0']=w(0x8c5ffd90);c['pc']=0x8c5ffb70
assert c['r0']==0x8c5ffb70 and c['r15']==old['r15']-4 and c['ssr']==0x40000001 and c['spc']==0x8ced9c48
assert w(w(0x8c5ffcdc))==0x8cede190 and w(0x8c5ffd7c)==0x8cede188
tick(9);save('entry')

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
c['sr']=c['ssr'];c['pc']=c['spc'];assert c['pc']==0x8cecfaa0 and c['r15']==new_sp+260

tick(89);save('restore')
assert elapsed==209
assert c['sr']==0x40000100 and c['r15']==0x8cffff10
for stage in ['state','timers','ram']:
 suffix='.bin'if stage=='ram'else'.txt';(out/('expected-'+stage+suffix)).write_bytes((out/('restore-'+stage+suffix)).read_bytes())
metadata=root/'work/dreamcast-recovery/live-movie-context-model-fragments.json'
for f in json.loads(read(metadata)):
 assert sha(f['path'])==f['source_sha256'];inputs[f['path']]=f['source_sha256']
inputs[str(Path(__file__).resolve())]=sha(__file__)
r=dict(passed=True,cycles=elapsed,next_missing=f"{c['pc']:08X}",restored_stack=f"{c['r15']:08X}",guest_execution=False,inputs_sha256=inputs)
(out/'report.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items()if k!='inputs_sha256'}))
