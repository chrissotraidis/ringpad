"""Source-derived owner state1 and common-tail service from captured live owner context."""
from pathlib import Path
import json,hashlib,struct,re
import sys
assert len(sys.argv)==5,'usage RAM STATE TIMERS OUTPUT'
root=Path.cwd();base=root/'docs/artifacts/recovery-aot';out=Path(sys.argv[4]);assert not out.exists()
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
inputs={}
def read(p):
 p=Path(p).resolve();inputs[str(p)]=sha(p);return p.read_bytes()
def fields(p):return {a:int(v,16)for a,v in(s.split('=',1)for s in read(p).decode().splitlines())if re.fullmatch('[0-9a-fA-F]+',v)}
ram=bytearray(read(sys.argv[1]));state=fields(sys.argv[2]);c={k:v for k,v in state.items()if re.fullmatch('pc|pr|sr|fpscr|vbr|spc|gbr|mach|macl|fpul|dbr|ssr|sgr|r[0-9]+|fr[0-9]+|xf[0-9]+|bank_r[0-9]+',k)}
timers={k:v for k,v in fields(sys.argv[3]).items()if k.startswith('tmu')}
assert len(c)==69 and c['pc']==0x8c64849e and c['pr']==0x8c6484a2
assert (c['sr']&0xf0)>=0x60 # Existing Holly levels2/4/6 masked; TMU model rejects underflow.
boot=read(base/'20261005-080717/BOOT.BIN');movie=read(base/'20261008-startup-movie-instance-resource-complete-startup/MOVIE.BIN')
original=bytes(ram);instructions={};blocks={};reviews=root/'work/dreamcast-recovery/loader-review/startup-producers'
names=['660754-20261009-live-movie-owner-service/review', '6601A8-20261009-live-movie-owner-state-one/review', '660562-20261009-live-movie-owner-tail/review', '65F4A0-20261009-live-movie-task-status/review', '60634E', '606358', '60635E-20261009-movie-worker-time/review', '10E8A0', '10E730-20261009-movie-worker-time/review', '10E67C', '6621A2-20261009-movie-stream-helper/review']
for name in names:
 for line in read(reviews/name/'function.txt').decode().splitlines():
  if line.startswith('BLOCK '):block=int(line.split()[1],16);blocks[block]=[]
  elif line.startswith('0x'):
   addr,body=line.split(' ',1);a=int(addr,16)
   if name.startswith('EB0000') and not 0x8ceb0254<=a<=0x8ceb025c:continue
   text=body.split(' delay=')[0];text=text.split('  ;')[0];op,_,operands=text.partition(' ')
   source,offset=(movie,a-0x8ceb0000)if a>=0x8ceb0000 else(boot,a-0x8c010000)
   assert source[offset:offset+2]==original[a&0xffffff:(a&0xffffff)+2]
   instructions[a]=(op,operands);blocks[block].append(a)
blocks={b:a for b,a in blocks.items()if a}
# Match the established AOT issue-cycle contract, not physical SH-4 latency.
weights={'mul.l':2,'sts':1,'jmp':2,'jsr':2,'bsr':2,'rts':2,'ldc':4,'stc':2}
timing_source=read(root/'work/dreamcast-recovery/arm64-fpu-domain-candidate/cpp_emitter.cpp').decode()
timing_contract=timing_source.split('std::uint64_t sh4_issue_cycles_for_dcir(DCIROp op) {',1)[1].split('\n}')[0]
assert 'case DCIROp::StsPr:' in timing_contract and 'case DCIROp::StsMacl:' not in timing_contract and 'return 1u;' in timing_contract
# STS MACL uses default one-cycle issue; STS PR alone has the two-cycle override.
blockcycles={b:sum(weights.get(instructions[a][0],1)for a in addrs)for b,addrs in blocks.items()}
mmio_writes=[];packets=[];call_targets=[];block_trace=[];cycles=0;steps=0;pc=0x8c660754
u=lambda n:n&0xffffffff
signed=lambda n:n-(1<<32)if n&0x80000000 else n

def tick(n):
 global cycles
 cycles+=n
 for i in range(3):
  if timers['tmu_tstr']&(1<<i):
   prefix=f'tmu{i}_';assert timers[prefix+'lazy_sh4_cycles']==0
   div=16<<(2*(timers[prefix+'tcr']&7));total=timers[prefix+'cycle_accumulator']+n;ticks=total//div
   assert ticks<=timers[prefix+'tcnt'];timers[prefix+'tcnt']-=ticks;timers[prefix+'ticks']+=ticks;timers[prefix+'cycle_accumulator']=total%div

def load(a,size=4):
 a=u(a);phys=a&0x1fffffff
 if 0xc000000<=phys<0xd000000:return int.from_bytes(ram[phys&0xffffff:(phys&0xffffff)+size],'little')
 if a==0xffd8000c:return timers['tmu0_tcnt']
 raise AssertionError(f'Unmodeled read {a:08x}/{size} at {pc:08x}')

def store(a,v,size=4):
 a=u(a);v&=(1<<(size*8))-1;phys=a&0x1fffffff;data=v.to_bytes(size,'little')
 if 0xc000000<=phys<0xd000000:ram[phys&0xffffff:(phys&0xffffff)+size]=data
 else:raise AssertionError(f'Unmodeled write {a:08x}/{size} at {pc:08x}')

def split(operand):return re.split(r',(?![^()]*\))',operand)
def value(x):return int(x[1:])if x.startswith('#')else c[x]
def address(x):
 assert x.startswith('@')
 x=x[1:]
 if x.startswith('('):
  a,b=x[1:-1].split(',')
  if b=='pc':return ((pc+4)&~3)+int(a) if instructions[pc][0]=='mov.l' else pc+4+int(a)
  return u((c[a]if a.startswith('r')else int(a))+c[b])
 if x.startswith('-'):c[x[1:]]=u(c[x[1:]]-4);return c[x[1:]]
 return c[x.rstrip('+')]
def condition(v):c['sr']=(c['sr']&~1)|int(v)

def operation(a):
 global pc
 pc=a;op,args=instructions[a];ab=split(args);x=ab[0];y=ab[-1]
 if op=='nop':pass
 elif op=='mov':c[y]=u(value(x))
 elif op=='extu.b':c[y]=c[x]&0xff
 elif op in ['mov.l','mov.w','mov.b','sts.l','lds.l']:
  size=1 if op=='mov.b'else 2 if op=='mov.w'else 4
  if x.startswith('@'):
   v=load(address(x),size)
   if size<4 and v&(1<<(size*8-1)):v|=0xffffffff^((1<<(size*8))-1)
   if x.endswith('+') and x[1:-1]!=y:c[x[1:-1]]=u(c[x[1:-1]]+size)
   c[y]=v
  else:store(address(y),value(x),size)
 elif op in ['stc','sts']:c[y]=c[x]
 elif op=='mul.l':c['macl']=u(c[x]*c[y])
 elif op=='ldc':
  v=c[x];assert ((v^c[y])&0x20000000)==0;c[y]=v
 elif op=='add':c[y]=u(c[y]+value(x))
 elif op=='sub':c[y]=u(c[y]-value(x))
 elif op=='addc':
  total=c[y]+c[x]+(c['sr']&1);c[y]=u(total);condition(total>0xffffffff)
 elif op=='and':c[y]&=u(value(x))
 elif op=='or':c[y]|=u(value(x))
 elif op=='tst':condition((value(x)&c[y])==0)
 elif op=='dt':c[x]=u(c[x]-1);condition(c[x]==0)
 elif op.startswith('cmp/'):
  left=c[y];right=u(value(x))
  condition({'eq':lambda:left==right,'hi':lambda:left>right,'hs':lambda:left>=right,'ge':lambda:signed(left)>=signed(right),'gt':lambda:signed(left)>signed(right),'pz':lambda:signed(left)>=0,'pl':lambda:signed(left)>0}[op[4:]]())
 elif op in ['shll2','shll8','shll16']:c[x]=u(c[x]<<int(op[4:]))
 elif op=='shlr2':c[x]>>=2
 elif op in ['shll','shlr','shar']:
  old=c[x];condition(bool(old&(0x80000000 if op=='shll'else 1)))
  c[x]=u(old<<1)if op=='shll'else u(signed(old)>>1)if op=='shar'else old>>1
 elif op=='movt':c[x]=c['sr']&1
 elif op=='xor':c[y]^=c[x]
 elif op=='subc':
  difference=c[y]-c[x]-(c['sr']&1);c[y]=u(difference);condition(difference<0)
 elif op=='div0u':c['sr']&=~0x301
 elif op=='div0s':
  m=c[x]>>31;q=c[y]>>31;c['sr']=(c['sr']&~0x301)|(m<<9)|(q<<8)|(m^q)
 elif op in ['rotcl','rotcr']:
  old=c[x];incoming=c['sr']&1
  c[x]=u((old<<1)|incoming)if op=='rotcl'else(old>>1)|(incoming<<31)
  condition(bool(old>>31)if op=='rotcl'else bool(old&1))
 elif op=='div1':
  # Reviewed positive divisors: time conversion100 and ring size0x71000; M=0.
  divisor=c[x];remainder=signed(c[y]);assert divisor in (100,60,10) and not c['sr']&0x200
  assert -divisor<=remainder<divisor and bool(c['sr']&0x100)==(remainder<0)
  remainder=2*remainder+(c['sr']&1)+(divisor if remainder<0 else -divisor)
  assert -divisor<=remainder<divisor
  c[y]=u(remainder);c['sr']=(c['sr']&~0x101)|(0x100 if remainder<0 else 1)
 elif op=='shad':
  amount=signed(c[x]);c[y]=u(c[y]<<(amount&31))if amount>=0 else u(signed(c[y])>>(32 if amount&31==0 else (-amount)&31))
 else:raise AssertionError(f'Unsupported {a:08x} {op} {args}')

while True:
 assert steps<4000,'Reference instruction cap exceeded'
 if pc==0x8c6484a2:
  c['pc']=pc # Original emitted RTS restores PC from PR before the return observer.
  break
 assert pc in instructions,f'Out of reviewed path: {pc:08x}'
 if pc in blocks:c['pc']=pc;tick(blockcycles[pc]);block_trace.append([pc,blockcycles[pc]])
 a=pc;op,args=instructions[a];steps+=1
 if op in ['jmp','jsr','bsr','rts','bra','bt','bf','bt/s','bf/s']:
  t=bool(c['sr']&1)
  if op=='jsr':target=c[args[1:]];c['pr']=a+4
  elif op=='jmp':target=c[args[1:]]
  elif op=='bsr':target=int(args,16);c['pr']=a+4
  elif op=='rts':target=c['pr']
  elif op=='bra':target=int(args,16)
  else:target=int(args,16)if (t if op.startswith('bt')else not t)else a+(4 if op.endswith('/s')else 2)
  if op in ['jmp','jsr','bsr','rts','bra']or op.endswith('/s'):operation(a+2);steps+=1
  if op in ['jsr','bsr']:call_targets.append(target)
  pc=target
 else:operation(a);pc=a+2
assert load(0x8c6774f0+1,1)==1 and load(0x8c6774f0+106,2)==1
out.mkdir()
for name,data in [('state',c),('timers',timers)]:
 (out/('expected-'+name+'.txt')).write_text(''.join(f'{k}={v:x}\n'for k,v in data.items()))
(out/'expected-ram.bin').write_bytes(ram);(out/'blocks.json').write_text(json.dumps(block_trace)+'\n')
inputs[str(Path(__file__).resolve())]=sha(Path(__file__).resolve())
result=dict(scope=__doc__,new_cycles=cycles,instructions=steps,next_target='8C6484A2',diagnostic_return_only=True,calls=[hex(t)for t in call_targets],executed=False,inputs_sha256=inputs)
(out/'report.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='inputs_sha256'},indent=2))
