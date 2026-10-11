"""Offline instruction model of movie frame and Holly interrupts; scoped to reviewed path, no native guest execution."""
from pathlib import Path
import hashlib,json,re,struct,sys
root=Path.cwd();base=root/'docs/artifacts/recovery-aot'
assert len(sys.argv)==8,'usage RAM STATE TIMERS SQ SQSTATE MMIO OUTPUT'
out=Path(sys.argv[7]);assert not out.exists()
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
inputs={}
def read(p):
 p=Path(p);inputs[str(p)]=sha(p);return p.read_bytes()
def fields(p):return {a:int(v,16)for a,v in (s.split('=')for s in read(p).decode().splitlines())}
ram=bytearray(read(sys.argv[1]));c=fields(sys.argv[2]);timers=fields(sys.argv[3]);sq=bytearray(read(sys.argv[4]));sqstate=fields(sys.argv[5]);boot=read(base/'20261005-080717/BOOT.BIN');movie=read(base/'20261008-startup-movie-instance-resource-complete-startup/MOVIE.BIN')
assert c['pc']==0x8ceb0254
mmio={int(k,16):v for k,v in fields(sys.argv[6]).items()};irq_trace=[];write_trace=[];ta_list=7
original=bytes(ram);instructions={};blocks={};reviews=root/'work/dreamcast-recovery/loader-review/startup-producers'
names=['EB0000-20261007-movie-entry','608DF0','60634E','646180','65A220','645C40','645B40','654908','65627E-20261009-movie-frame-copy/review']
names+=['662518','655F50','656294']
names+=['../live-frame-irq-20261009/'+a for a in ['662698','654B80','655C5E','6082A8']]
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
def weight(a):
 op,args=instructions[a]
 if op in ['jsr','bsr','jmp','rts']:return 2
 if op=='rte':return 5
 if op in ['sts','lds'] and 'pr' in args:return 2
 if op.startswith('stc'):return 2
 if op.startswith('ldc'):
  if args.endswith(',sr'):return 4
  if args.endswith(',gbr'):return 3
 return 1
blockcycles={b:sum(weight(a)for a in addrs)for b,addrs in blocks.items()}
mmio_writes=[];packets=[];call_targets=[];block_trace=[];cycles=0;steps=0;pc=0x8ceb0254
u=lambda n:n&0xffffffff
signed=lambda n:n-(1<<32)if n&0x80000000 else n

def write_sr(v):
 old=c['sr'];v&=0x700083f3
 if (old&0x60000000==0x60000000)!=(v&0x60000000==0x60000000):
  for i in range(8):c[f'r{i}'],c[f'bank_r{i}']=c[f'bank_r{i}'],c[f'r{i}']
 c['sr']=v

def packet(source,data):
 global ta_list
 packets.append([c['pc'] if source==2 else pc,source,data]);pcw=int.from_bytes(data[:4],'little');kind=(pcw>>29)&7
 if kind==4:ta_list=(pcw>>24)&7
 elif kind==0:
  ended=(pcw>>24)&7 if ta_list==7 else ta_list
  assert ended in [0,2]
  c['holly_pending0']|=1<<(7+ended);ta_list=7
 else:raise AssertionError('Packet outside audited list header/end path')

def interrupt():
 if c['sr']&0x10000000:return False
 for row,priority,event in [(2,6,0x320),(1,4,0x360),(0,2,0x3a0)]:
  if priority<=((c['sr']>>4)&15):continue
  if not any(c[f'holly_pending{bank}']&c[f'holly_mask{row}_{bank}']for bank in range(3)):continue
  irq_trace.append(dict(pc=f'{pc:08x}',cycle=cycles,pending=c['holly_pending0'],event=event,block_charge=blockcycles[pc]))
  c['ssr']=c['sr'];c['spc']=pc;c['sgr']=c['r15'];write_sr(c['sr']|0x70000000)
  c['holly_irq_taken']+=1;mmio[0x1f000028]=event
  # Original 662518 vector body was copied to VBR+600 at startup. Existing
  # runtime dispatch relocation resolves it to this body, no synthetic cycles.
  assert ram[0x600:0x626]==ram[0x662518:0x66253e]
  return True
 return False

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
 if phys in [0x5f6900,0x5f6904,0x5f6908]:return c['holly_pending'+str((phys-0x5f6900)//4)]
 if phys in mmio:return mmio[phys]
 raise AssertionError(f'Unmodeled read {a:08x}/{size} at {pc:08x}')

def store(a,v,size=4):
 a=u(a);v&=(1<<(size*8))-1;phys=a&0x1fffffff;data=v.to_bytes(size,'little')
 if 0xc000000<=phys<0xd000000:
  ram[phys&0xffffff:(phys&0xffffff)+size]=data;write_trace.append([pc,a,size,v])
 elif 0xe0000000<=a<0xe4000000:sq[a&63:(a&63)+size]=data
 elif a in [0xff000038,0xff00003c]:sqstate['qacr'+str((a-0xff000038)//4)]=v;sqstate['qacr_writes']+=1
 elif a in [0xffa00020,0xffa00024,0xffa00028,0xffa0002c,0xa05f6800,0xa05f6804,0xa05f6808]:
  mmio_writes.append([a,v]);mmio[phys]=v
  if a==0xa05f6808 and v==1:
   latest=dict(mmio_writes);source=latest[0xffa00020]&0x1fffffe0;length=latest[0xa05f6804];assert latest[0xa05f6800]==0x10000000 and length==64
   for offset in range(0,length,32):packet(2,bytes(ram[(source+offset)&0xffffff:((source+offset)&0xffffff)+32]))
   mmio[0x1fa00020]=source+length;mmio[0x1fa00028]=0;mmio[0x1fa0002c]|=2
   c['holly_pending0']|=1<<19
 elif phys in [0x5f6900,0x5f6904,0x5f6908]:
  c['holly_pending'+str((phys-0x5f6900)//4)]&=~v;mmio_writes.append([a,v])
 elif phys in mmio:raise AssertionError(f'Unaudited MMIO write {a:08x}/{v:08x} at {pc:08x}')
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
 elif op=='mova':c[y]=((pc+4)&~3)+int(x[2:].split(',')[0])
 elif op in ['sts','lds']:c[y]=c[x]
 elif op=='frchg':c['fpscr']^=1<<21
 elif op=='not':c[y]=u(~c[x])
 elif op=='xor':c[y]^=u(value(x))
 elif op=='fmov.s':
  assert not c['fpscr']&(1<<20)
  def fr(name):return ('xf' if c['fpscr']&(1<<21) else 'fr')+name[2:]
  if x.startswith('@'):
   c[fr(y)]=load(address(x))
   if x.endswith('+'):c[x[1:-1]]+=4
  else:store(address(y),c[fr(x)])
 elif op in ['mov.l','mov.w','sts.l','lds.l','stc.l','ldc.l']:
  size=2 if op=='mov.w' else 4
  if x.startswith('@'):
   v=load(address(x),size)
   if size==2 and v&0x8000:v|=0xffff0000
   if x.endswith('+') and x[1:-1]!=y:c[x[1:-1]]=u(c[x[1:-1]]+size)
   if y=='sr':write_sr(v)
   else:c[y]=v
  else:store(address(y),value(x),size)
 elif op=='stc':c[y]=c[x]
 elif op=='ldc':
  v=c[x]
  if y=='sr':write_sr(v)
  else:c[y]=v
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
  sqstate.update(last_pref_address=a,last_pref_target=target,last_pref_source_pc=pc);packet(1,bytes(sq[queue*32:queue*32+32]))
 else:raise AssertionError(f'Unsupported {a:08x} {op} {args}')

out.mkdir();result={'passed':False,'scope':__doc__}
try:
 while True:
  assert steps<4000,'Reference instruction cap exceeded'
  if pc==0x8cecfa68:break
  assert pc in instructions,f'Out of reviewed path: {pc:08x}'
  if pc in blocks:
   c['pc']=pc;tick(blockcycles[pc]);block_trace.append([pc,blockcycles[pc]])
   if interrupt():pc=0x8c662518;continue
  a=pc;op,args=instructions[a];steps+=1
  if op in ['jsr','jmp','bsr','rts','rte','bra','bt','bf','bt/s','bf/s']:
   t=bool(c['sr']&1)
   if op=='jsr':target=c[args[1:]];c['pr']=a+4
   elif op=='jmp':target=c[args[1:]]
   elif op=='rte':target=c['spc'];write_sr(c['ssr']);irq_trace[-1]['return_cycle']=cycles
   elif op=='bsr':target=int(args,16);c['pr']=a+4
   elif op=='rts':target=c['pr']
   elif op=='bra':target=int(args,16)
   else:target=int(args,16)if (t if op.startswith('bt')else not t)else a+(4 if op.endswith('/s')else 2)
   if op in ['jsr','jmp','bsr','rts','rte','bra']or op.endswith('/s'):operation(a+2);steps+=1
   if op in ['jsr','bsr']:call_targets.append(target)
   pc=target
  else:operation(a);pc=a+2
 # The model never reads the observed endpoint.
 (out/'ram-writes.json').write_text(json.dumps(write_trace)+'\n')
 (out/'irq-trace.json').write_text(json.dumps(irq_trace,indent=2)+'\n')
 for name,data in [('state',c),('timers',timers),('store-queue-state',sqstate)]:
  (out/('expected-'+name+'.txt')).write_text(''.join(f'{k}={v:x}\n'for k,v in data.items()))
 (out/'expected-ram.bin').write_bytes(ram);(out/'expected-store-queues.bin').write_bytes(sq);(out/'expected-ta-packets.bin').write_bytes(b''.join(p[2]for p in packets));(out/'blocks.json').write_text(json.dumps(block_trace)+'\n')
 (out/'mmio-writes.json').write_text(json.dumps(mmio_writes)+'\n');(out/'packets.json').write_text(json.dumps([dict(pc=p[0],source=p[1],hex=p[2].hex())for p in packets])+'\n')
 result.update(passed=True,cycles=cycles,instructions=steps,blocks=len(block_trace),interrupts=irq_trace,calls=[hex(t)for t in call_targets],next_missing='8CECFA68',guest_execution=False,G1_resolved=False)
except Exception as e:result.update(error=str(e),reference_pc=hex(pc),cycles=cycles,instructions=steps);print(e)
inputs[str(Path(__file__).resolve())]=sha(Path(__file__).resolve());result['inputs_sha256']=inputs;(out/'report.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='inputs_sha256'},indent=2));sys.exit(0 if result['passed']else 1)
