"""Original callback's flag-zero prefix through its registered timer helper; no native guest execution."""
from pathlib import Path
import hashlib,json,re,struct,sys
assert len(sys.argv)==5,'usage RAM STATE TIMERS OUT'
root=Path.cwd();base=root/'docs/artifacts/recovery-aot';out=Path(sys.argv[4]);assert not out.exists()
inputs={}
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
def read(p):
 p=Path(p).resolve();inputs[str(p)]=sha(p);return p.read_bytes()
def fields(p):return {k:int(v,16)for k,v in (l.split('=',1)for l in read(p).decode().splitlines())if re.fullmatch('[0-9a-fA-F]+',v)}
allstate=fields(sys.argv[2]);c={k:v for k,v in allstate.items()if re.fullmatch('pc|pr|sr|fpscr|vbr|spc|gbr|mach|macl|fpul|dbr|ssr|sgr|r[0-9]+|fr[0-9]+|xf[0-9]+|bank_r[0-9]+',k)}
t={k:v for k,v in fields(sys.argv[3]).items()if k.startswith('tmu')};ram=bytearray(read(sys.argv[1]));cycles=0
assert c['pc']==0x8c645d98 and c['pr']==0x8c645da0 and c['r2']==0x8cecfa68
assert all(allstate.get('holly_pending'+str(i),0)==0 for i in range(3))
boot=read(base/'20261005-080717/BOOT.BIN');movie=read(base/'20261008-startup-movie-instance-resource-complete-startup/MOVIE.BIN')
reviews=root/'work/dreamcast-recovery/loader-review/startup-producers'
for name in ['60634E','ECFA68-20261009-movie-loop-callback/review']:
 for line in read(reviews/name/'function.txt').decode().splitlines():
  if line.startswith('0x'):
   a=int(line.split()[0],16);image,offset=(movie,a-0x8ceb0000)if a>=0x8ceb0000 else(boot,a-0x8c010000)
   assert image[offset:offset+2]==ram[a&0xffffff:(a&0xffffff)+2]
w=lambda a:struct.unpack_from('<I',ram,a&0xffffff)[0]
def put(a,v):struct.pack_into('<I',ram,a&0xffffff,v&0xffffffff)
def push(v):c['r15']-=4;put(c['r15'],v)
def tick(n):
 global cycles
 cycles+=n
 for i in range(3):
  if t['tmu_tstr']&(1<<i):
   p=f'tmu{i}_';assert t[p+'lazy_sh4_cycles']==0
   div=16<<(2*(t[p+'tcr']&7));total=t[p+'cycle_accumulator']+n;ticks=total//div
   assert ticks<=t[p+'tcnt'];t[p+'tcnt']-=ticks;t[p+'ticks']+=ticks;t[p+'cycle_accumulator']=total%div
# 8 instructions + JSR issue increment, including its delay-slot store.
c['pc']=0x8cecfa68;tick(9);c['r1']=w(0x8cecfb64);push(c['r14']);push(c['pr']);c['r14']=w(0x8cecfb6c);c['r0']=w(0x8cecfb68);c['r3']=w(c['r1']);c['pr']=0x8cecfa78;put(c['r0'],c['r3']);assert c['r14']==0x8c60634e
# Existing helper reads TMU0 after its six-cycle block charge.
c['pc']=0x8c60634e;tick(6);c['r2']=w(0x8c60645c);assert c['r2']==0xffd8000c
c['r0']=0xffffffff;c['r3']=t['tmu0_tcnt'];c['r0']=(c['r0']-c['r3'])&0xffffffff
# Flag is zero on both the retained control and actual live input.
c['pc']=0x8cecfa78;tick(6);c['r1']=w(0x8cecfb64);c['r2']=w(0x8cecfb70);put(c['r1'],c['r0']);c['r0']=w(c['r2']);assert c['r0']==0;c['sr']&=~1
# Bounded at the actual missing service; the callback has not returned.
c['pc']=0x8cecfa98;tick(5);c['r3']=w(0x8cecfb84);c['r2']=w(0x8cecfb80);c['pr']=0x8cecfaa0;c['r4']=w(c['r2']);assert c['r3']==0x8c5ffae0
assert cycles==26
out.mkdir();(out/'expected-ram.bin').write_bytes(ram)
for name,data in [('state',c),('timers',t)]:
 (out/('expected-'+name+'.txt')).write_text(''.join(f'{k}={v:x}\n'for k,v in data.items()))
inputs[str(Path(__file__).resolve())]=sha(__file__)
r=dict(passed=True,scope=__doc__,cycles=cycles,next_missing='8C5FFAE0',guest_execution=False,inputs_sha256=inputs)
(out/'report.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items()if k!='inputs_sha256'}))
