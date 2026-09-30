#!/usr/bin/env python3
"""Build an explicit editorial ledger and additive, executable study pack."""
import copy,json,re
from pathlib import Path
from design_patterns_expansion.specs import S
ROOT=Path(__file__).resolve().parents[1]
def read(p):return json.loads((ROOT/p).read_text())
def write(p,x):
 p=ROOT/p;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(x,ensure_ascii=False,indent=2)+'\n')
pack={'version':1,'courseId':'design-patterns-cpp','cards':[],'questions':[],'workedPrograms':[],'diagrams':[],'edits':[]}
old=read('content/design-patterns-expansion.json') if (ROOT/'content/design-patterns-expansion.json').exists() else None
problem_discussions=read('scripts/design_patterns_expansion/problem_discussions.json')
recall={};n=0
for line in (ROOT/'scripts/design_patterns_expansion/recall.txt').read_text().splitlines():
 if line.startswith('# '):n=int(line[2:]);recall[n]=[]
 elif line:recall[n].append(line.split('|',1))
helper='''// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}
'''
def edit(scope,id,field,before,after):
 if before!=after:pack['edits'].append(dict(scope=scope,id=id,field=field,before=copy.deepcopy(before),after=copy.deepcopy(after)))
for n in range(1,24):
 spec=S[n];m=read(f'manuscript/ch{n:02}.json');chunk=read(f'content/design-patterns-cpp/ch{n}.json');title=m['title'];topics=chunk['topics']
 if old:
  ids={c['id'] for c in old['cards']}
  for t in topics:
   t['cards']=[c for c in t['cards'] if c['id'] not in ids]
   for e in old['edits']:
    if e['scope']=='topic' and e['id']==t['id']:t[e['field']]=copy.deepcopy(e['before'])
 overview=topics[0];solution=(ROOT/f'companion/solutions/ch{n:02}/main.cpp').read_text();prefix,driver=solution.split('int main()',1);driver='int main()'+driver
 code=prefix+helper;expected=(ROOT/f'companion/solutions/ch{n:02}/expected.txt').read_text()
 assert len(recall[n])==(18 if n==23 else 7),(n,len(recall[n]))
 tests=[]
 for i,row in enumerate(spec['rows'],1):
  expression='([]{ '+row['body']+' })()'
  tests.append(dict(id=f't{i}',label=row['label'],expression=expression,expected=json.loads(row['expected']) if row['expected'].startswith(chr(34)) else row['expected'],hint=row['why']))
  pack['cards'].append(dict(id=f'dpx-c{n:02}-trace-{i}',chapter=n,topicId=next(t['id'] for t in topics if t['title']=='Execution trace'),category='trace',question=f'{title}: {row["label"]}. What does this expression return? Use the extended chapter implementation in the linked lab. dpx_rejects<E> returns true only if its callable throws E.',code=expression,answer=row['expected']+'. '+row['why'],labId=f'dpx-{n:02}-a'))
 for j,(label,correct,broken,reason) in enumerate(spec['repairs']):
  assert correct in code,(n,correct)
  qid=f'dpx-{n:02}-{chr(97+j)}';starter=code.replace(correct,broken,1)
  q=dict(id=qid,courseId=pack['courseId'],chapter=n,chapterTitle=title,title=label,level='Integration' if n==23 else 'Repair / extend',prompt=f'{label}. {reason} Repair the compiling defect in the supplied extended chapter implementation. Preserve the other behavior covered by the public checks. '+ ' '.join(m['example']['requirements']),signature='Repair the supplied classes; keep their public signatures.',starter=starter,solution=code,driver=driver,sampleInput='',sampleOutput=expected,tests=tests,hints=[reason,'Use the named public checks to compare state before and after rejection. The sample driver is the complete extension demonstration.'],explanation=reason+' '+m['example']['design'],guide=dict(problem=' '.join(m['example']['requirements']),design=m['example']['design'],invariant=' '.join(m['example']['invariants']),trace=spec['rows'][min(j*2,7)]['why'],failure='Defective excerpt: '+broken+' Correct behavior: '+reason,maintenance=m['example']['maintenance']),sourceFilename=f'companion/solutions/ch{n:02}/main.cpp')
  pack['questions'].append(q)
  for kind in ['starter','solution']:
   p=ROOT/f'coding_lab/design_patterns_expansion/ch{n:02}/{chr(97+j)}_{kind}.cpp';p.parent.mkdir(parents=True,exist_ok=True);p.write_text(q[kind]+'\n'+driver)
  if n!=23:pack['cards'].append(dict(id=f'dpx-c{n:02}-repair-{j+1}',chapter=n,topicId=next(t['id'] for t in topics if 'Common incorrect' in t['title'] or 'failure' in t['title'].lower()),category='debug',question=f'{title}: diagnose this replacement line in the extended implementation. Which contract does it break?',code=broken,answer=reason+' Correct excerpt: '+correct,labId=qid))
 for i,(question,answer) in enumerate(recall[n],1):
  pack['cards'].append(dict(id=f'dpx-c{n:02}-reason-{i}',chapter=n,topicId=overview['id'],category='design',question=question,answer=answer,labId=f'dpx-{n:02}-'+('c' if n==23 and i in [5,18] else 'b')))
 # Register exact original resources, preserving their distinct expected behavior.
 for variant,folder in [('examples','examples'),('exercises','exercises'),('solutions','solutions')]:
  filename=f'companion/{folder}/ch{n:02}/main.cpp';src=(ROOT/filename).read_text();out=(ROOT/filename.replace('main.cpp','expected.txt')).read_text();sid=f'DP-C{n:02}-{variant}'
  pack['workedPrograms'].append(dict(id='worked-'+sid,sourceId=sid,sourcePartIds=[],sourceKind='program',courseId=pack['courseId'],chapter=n,chapterTitle=title,title=title+' — '+{'examples':'original example','exercises':'extension starting point','solutions':'completed extension'}[variant],sourceFilename=filename,sourceVariant=variant,sourceStatus='ran',source=src,originalSource=src,adapted=False,adaptationNote='',experiment=m['lab']['task'] if variant!='solutions' else 'Explain the extension, then change one boundary and run the original checks.',sampleInput='',sampleOutput=out,checks=[dict(id='original-output',label='Original checks and exact printed output',input='',stdout=out,stderr='',exitCode=0,hint='Keep the original main checks enabled. This verifies recorded behavior, not every possible extension.')],guide=dict(problem=' '.join(m['example']['requirements']),design=m['example']['design'],invariant=' '.join(m['example']['invariants']),trace='This is the '+variant+' file. Its own complete main and recorded output define this check; extension starters are not claimed to implement the extension yet.',failure='A mismatch in output, an exception, or a nonzero exit fails this original-behavior check.',maintenance=m['example']['maintenance'])))
 # Correct displayed connections; retain all prior wording in a reversible ledger.
 for t in topics:
  before=copy.deepcopy(t);summary=t['summary'];reading=copy.deepcopy(t.get('reading',{}))
  if t is overview:
   intent=m.get('attachment_alignment',{}).get('intent',summary).split(' The attached')[0]
   intent=intent.split('. ')[0].rstrip('.')+'.'
   intent=intent.replace('which attached relationships survive','which pattern relationships are used')
   summary=intent+' Worked example: '+m['example']['title'].rstrip('.')+'. '+m['example']['design']
  elif t['title']=='Roles in the implementation':
   names=[c['term'].split('→')[-1].strip() for c in t['concepts']]
   summary='In '+title+', '+', '.join(names[:3])+ ' carry the main responsibilities. Read each role below against the named types in '+m['example']['file']+'.'
  elif t['title']=='Execution trace':summary=m['example']['title']+'. Begin with this concrete state: '+m['trace'][0]['state']+' The steps below explain each change and its cause.'
  elif summary=='A compiling program can still violate its contract. Predict the actual consequence before reading the reason.':summary='Debug '+m['example']['title'].lower()+'. Check the failing operation against this rule: '+m['example']['invariants'][0]+' The cases below distinguish the observed consequence from its cause.'
  t['summary']=summary
  if reading:
   reading['paragraphs']=[summary if p==before['summary'] else p for p in reading.get('paragraphs',[])]
   reading['paragraphs']=[p for p in reading['paragraphs'] if p!='Open Scenarios to select an answer and compare the reasoning for all four choices.']
   reading['preview']=' '.join(summary.split()[:34])
   if t['title'] in [s['heading'] for s in m['sections']]:
    index=[s['heading'] for s in m['sections']].index(t['title'])
    if index==0 or index==len(m['sections'])-1:
     qa=problem_discussions[str(n)] if index==0 else recall[n][-1]
     reading['discussion']={'question':qa[0],'answer':qa[1],'hint':'Use this chapter’s stated requirement and the observable behavior of its example.'}
   if n==18 and t is overview:reading['correction']='Two Observer policies appear in Coding Lab. Feed rejects subscription changes during publication; the older Signal snapshot exercise permits additions for the next publication. Each is intentional. Follow the contract of the named implementation, not a universal Observer rule.'
   if n==23 and t is overview:reading['correction']='The capstone uses an enum and guarded transitions for its lifecycle. That is a state machine, not the polymorphic State implementation from Chapter 19.'
   t['reading']=reading
  # Remove a stray foreign API name without changing the role itself.
  for c in t.get('concepts',[]):c['definition']=c['definition'].replace('in SomeOperation or run','in ArchiveJob::run')
  for c in t['cards']:
   if c['answer']==before['summary'] and t is overview:c['answer']=summary
   c['answer']=c['answer'].replace('in SomeOperation or run','in ArchiveJob::run')
  for key in ['summary','reading','concepts','cards']:edit('topic',t['id'],key,before.get(key),t.get(key))
 # Additional activity view traces the complete original main, with three genuine source ranges.
 if n<=22:
  src=(ROOT/f'companion/examples/ch{n:02}/main.cpp').read_text();lines=src.splitlines();start=next(i for i,l in enumerate(lines) if 'int main()' in l)+1
  # First 18 lines form a chronological excerpt, not a fabricated call graph.
  end=min(start+18,len(lines)-1);width=(end-start+2)//3;nodes=[]
  for j in range(3):
   a=start+j*width;b=min(a+width,end)
   if a>=b:break
   nodes.append(dict(id=f's{j}',label=['Enter the demonstration','Follow the next operations','Observe checks and effects'][j],code=lines[a:b],lineStart=a+1,lineEnd=b))
  pack['diagrams'].append(dict(chapter=n,expansionId=f'dpx-{n:02}-a',source=f'companion/examples/ch{n:02}/main.cpp',overview=dict(kind='activity',title=title+' — chronological driver trace',nodes=nodes,edges=[dict(**{'from':nodes[j]['id'],'to':nodes[j+1]['id']},kind='flow',label='continue in source order') for j in range(len(nodes)-1)],explanation='Solid arrows show execution order between contiguous excerpts of main, not inheritance, ownership or calls between types. Branches and loops inside a block follow the displayed C++ syntax. The existing relationship view explains the object design; this complementary view follows its actual demonstration. Excerpts are not standalone programs.',expansionId=f'dpx-{n:02}-a',source=f'companion/examples/ch{n:02}/main.cpp'),focus=[]))
# Label older independent mini-labs so their names and policies do not masquerade as the chapter implementation.
coding=read('content/design-patterns-cpp/coding.json')
for q in coding['questions']:
 if q['id'].startswith('dpx-'):continue
 for e in (old or {}).get('edits',[]):
  if e['scope']=='question' and e['id']==q['id']:q[e['field']]=e['before']
 prefix={'dp-observer':'Alternative policy exercise: Signal takes a listener snapshot. Unlike chapter Feed, it permits new subscriptions during dispatch and delivers to them only on the next publication. ', 'dp-strategy':'Independent mini-example: this exercise uses pricing strategies, while the chapter uses reading estimates. Their class names and domain rules are separate. ', 'dp-decorator':'Independent mini-example: this exercise wraps a renderer with brackets. The chapter instead uses Message, Prefix and Limit; both illustrate ordered wrapping under their own contracts. '}.get(q['id'],'')
 if prefix:edit('question',q['id'],'prompt',q['prompt'],prefix+q['prompt'])
assert len(pack['cards'])==400,len(pack['cards'])
assert len(pack['questions'])==47
assert len({c['question']+'\n'+c.get('code','') for c in pack['cards']})==400
write('content/design-patterns-expansion.json',pack)
print('Authored 400 cards, 47 eight-check repair labs, 69 exact worked programs, 22 trace diagrams,',len(pack['edits']),'reversible field edits.')
