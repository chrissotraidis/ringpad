"""Offline reference for one whitelisted movie-frame prefix, never a game runner.
Review text supplies original instruction operands; this model uses no generated
C++, runtime calls or observed final state to compute its CPU/RAM/TMU expectation.
"""
from pathlib import Path
import hashlib,json,re,struct,sys
root=Path.cwd();base=root/'docs/artifacts/recovery-aot';parent=base/'20261009-startup-movie-resource-method-complete';native=base/'20261009-startup-movie-loop-graphics';out=base/'20261009-movie-frame-reference-repair1'
assert not out.exists()
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
inputs={}
def read(p):
 inputs[str(p)]=sha(p);return p.read_bytes()
def fields(p):return {a:int(v,16)for a,v in (s.split('=')for s in read(p).decode().splitlines())}
cp=parent/'resource-method-complete-checkpoint.json';assert sha(cp)=='8d1cfc6c093a554d44e1fb616a74d16c4c8a3afc9aa9cefc318076835a63a100';ids=json.loads(read(cp))['identities'];assert all(sha(k)==v for k,v in ids.items())
report=json.loads(read(parent/'report.json'));args=next(s['argv']for s in report['steps']if s['name']=='execute')
ram=bytearray(read(parent/'observed-ram.bin'));c=fields(parent/'observed-state.txt');timers=fields(parent/'observed-timers.txt');sq=bytearray(read(Path(args[4])));sqstate=fields(Path(args[5]));boot=read(base/'20261005-080717/BOOT.BIN');movie=read(Path(args[8]))
original=bytes(ram);instructions={};blocks={};reviews=root/'work/dreamcast-recovery/loader-review/startup-producers'
names=['EB0000-20261007-movie-entry','608DF0','60634E','646180','65A220','645C40','645B40','654908','65627E-20261009-movie-frame-copy/review']
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
weights={'jsr':2,'bsr':2,'rts':2,'ldc':4,'stc':2}
blockcycles={b:sum(weights.get(instructions[a][0],1)for a in addrs)for b,addrs in blocks.items()}
mmio_writes=[];packets=[];call_targets=[];block_trace=[];cycles=0;steps=0;pc=0x8ceb0254
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
 elif 0xe0000000<=a<0xe4000000:sq[a&63:(a&63)+size]=data
 elif a in [0xff000038,0xff00003c]:sqstate['qacr'+str((a-0xff000038)//4)]=v;sqstate['qacr_writes']+=1
 elif a in [0xffa00020,0xffa00024,0xffa00028,0xffa0002c,0xa05f6800,0xa05f6804,0xa05f6808]:
  mmio_writes.append([a,v])
  if a==0xa05f6808 and v==1:
   latest=dict(mmio_writes);source=latest[0xffa00020]&0x1fffffe0;length=latest[0xa05f6804];assert latest[0xa05f6800]==0x10000000 and length==64
   for offset in range(0,length,32):packets.append([c['pc'],2,bytes(ram[(source+offset)&0xffffff:((source+offset)&0xffffff)+32])])
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
 elif op in ['mov.l','mov.w','sts.l','lds.l']:
  size=2 if op=='mov.w' else 4
  if x.startswith('@'):
   v=load(address(x),size)
   if size==2 and v&0x8000:v|=0xffff0000
   if x.endswith('+') and x[1:-1]!=y:c[x[1:-1]]=u(c[x[1:-1]]+size)
   c[y]=v
  else:store(address(y),value(x),size)
 elif op=='stc':c[y]=c[x]
 elif op=='ldc':
  v=c[x];assert ((v^c[y])&0x20000000)==0;c[y]=v
 elif op=='add':c[y]=u(c[y]+value(x))
 elif op=='sub':c[y]=u(c[y]-value(x))
 elif op=='and':c[y]&=u(value(x))
 elif op=='or':c[y]|=u(value(x))
 elif op=='tst':condition((value(x)&c[y])==0)
 elif op=='dt':c[x]=u(c[x]-1);condition(c[x]==0)
 elif op.startswith('cmp/'):
  left=c[y];right=u(value(x))
  condition({'eq':lambda:left==right,'hi':lambda:left>right,'hs':lambda:left>=right,'ge':lambda:signed(left)>=signed(right),'pz':lambda:signed(left)>=0}[op[4:]]())
 elif op in ['shll2','shll8','shll16']:c[x]=u(c[x]<<int(op[4:]))
 elif op=='shlr2':c[x]>>=2
 elif op in ['shll','shlr','shar']:
  old=c[x];condition(bool(old&(0x80000000 if op=='shll'else 1)))
  c[x]=u(old<<1)if op=='shll'else u(signed(old)>>1)if op=='shar'else old>>1
 elif op=='shad':
  amount=signed(c[x]);c[y]=u(c[y]<<(amount&31))if amount>=0 else u(signed(c[y])>>(32 if amount&31==0 else (-amount)&31))
 elif op=='pref':
  a=address(x);assert 0xe0000000<=a<0xe4000000;queue=(a>>5)&1;target=((a&0x03ffffe0)|((sqstate['qacr'+str(queue)]&0x1c)<<24));assert target==0x10000000
  for key in ['pref_calls','pref_sq_calls','sq_commits','pref_ta_commits']:sqstate[key]+=1
  sqstate.update(last_pref_address=a,last_pref_target=target,last_pref_source_pc=pc);packets.append([pc,1,bytes(sq[queue*32:queue*32+32])])
 else:raise AssertionError(f'Unsupported {a:08x} {op} {args}')

out.mkdir();result={'passed':False,'scope':__doc__}
try:
 while True:
  assert steps<4000,'Reference instruction cap exceeded'
  if pc==0x8cecfa68:break
  assert pc in instructions,f'Out of reviewed path: {pc:08x}'
  if pc in blocks:c['pc']=pc;tick(blockcycles[pc]);block_trace.append([pc,blockcycles[pc]])
  a=pc;op,args=instructions[a];steps+=1
  if op in ['jsr','bsr','rts','bra','bt','bf','bt/s','bf/s']:
   t=bool(c['sr']&1)
   if op=='jsr':target=c[args[1:]];c['pr']=a+4
   elif op=='bsr':target=int(args,16);c['pr']=a+4
   elif op=='rts':target=c['pr']
   elif op=='bra':target=int(args,16)
   else:target=int(args,16)if (t if op.startswith('bt')else not t)else a+(4 if op.endswith('/s')else 2)
   if op in ['jsr','bsr','rts','bra']or op.endswith('/s'):operation(a+2);steps+=1
   if op in ['jsr','bsr']:call_targets.append(target)
   pc=target
  else:operation(a);pc=a+2
 # Freeze the computed expectation BEFORE reading any observed final state.
 for name,data in [('state',c),('timers',timers),('store-queue-state',sqstate)]:
  (out/('expected-'+name+'.txt')).write_text(''.join(f'{k}={v:x}\n'for k,v in data.items()))
 (out/'expected-ram.bin').write_bytes(ram);(out/'expected-store-queues.bin').write_bytes(sq);(out/'expected-ta-packets.bin').write_bytes(b''.join(p[2]for p in packets));(out/'blocks.json').write_text(json.dumps(block_trace)+'\n')
 ncp=native/'movie-loop-graphics-checkpoint.json';assert sha(ncp)=='38dfc88b5db63b1925436c86e7539461f478052f9e081de7db90551413d0fc51';nids=json.loads(read(ncp))['identities'];assert all(sha(k)==v for k,v in nids.items())
 comparisons={}
 for name,data in [('state',c),('timers',timers),('store-queue-state',sqstate)]:
  actual=fields(native/('observed-'+name+'.txt'));diff={k:[v,actual.get(k)]for k,v in data.items()if actual.get(k)!=v};comparisons[name]=diff
 actual=read(native/'observed-ram.bin');comparisons['ram_bytes']=[[i,ram[i],actual[i]]for i in range(len(ram))if ram[i]!=actual[i]]
 comparisons['packet_pcs']=struct.pack('<4I',*(p[0]for p in packets))!=read(native/'observed-ta-pcs.bin');comparisons['packet_sources']=bytes(p[1]for p in packets)!=read(native/'observed-ta-sources.bin')
 comparisons['sq']=bytes(sq)!=read(native/'observed-store-queues.bin');comparisons['packets']=b''.join(p[2]for p in packets)!=read(native/'observed-ta-packets.bin')
 result.update(cycles=cycles,instructions=steps,blocks=len(block_trace),calls=[hex(t)for t in call_targets],comparisons=comparisons,passed=not any(comparisons.values()),G1_resolved=False,candidate_only=True)
except Exception as e:result.update(error=str(e),reference_pc=hex(pc),cycles=cycles,instructions=steps);print(e)
inputs[str(Path(__file__).resolve())]=sha(Path(__file__).resolve());result['inputs_sha256']=inputs;(out/'report.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='inputs_sha256'},indent=2));sys.exit(0 if result['passed']else 1)
