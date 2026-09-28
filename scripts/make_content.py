#!/usr/bin/env python3
import json,re,random
from pathlib import Path
from scenarios import SCENARIOS
R=Path(__file__).resolve().parents[1]
load=lambda p:json.loads(p.read_text())
D=[{'id':'FOUND','title':'Foundations'},{'id':'CREATE','title':'Creational patterns'},{'id':'STRUCT','title':'Structural patterns'},{'id':'BEHAVE','title':'Behavioral patterns'},{'id':'CAP','title':'Capstone'},{'id':'REF','title':'Glossary'}]
c=dict(id='design-patterns-cpp',kind='textbook',title='Design Patterns in C++',subtitle='Dr. Charles Dorner',description='Understand the problem. Recall the pattern. Trace the C++. Study every chapter, then combine the lessons in the capstone.',provenance='Study companion to Design Patterns in C++ by Dr. Charles Dorner. Lessons retain the supplied editable textbook’s chapter sections, example contracts, traces, focused explanations, exercises, lab instructions, and answers. Flashcards use those chapter explanations. Multiple-choice scenarios are original teaching questions. UML views render the supplied diagram definitions and exact focused source excerpts. This is a study companion, with the full runnable source in the C++ view and download package.',domains=D,topics=[],chapters=[],glossary=[],examples={},diagrams={})
chlookup={0:'Foundations and course orientation'}
def topic(n,title,summary,blocks=None,concepts=None,cards=None,**kw):
 dom='FOUND' if n==0 else 'CREATE' if n<=5 else 'STRUCT' if n<=12 else 'BEHAVE' if n<=22 else 'CAP'
 no=sum(t['chapter']==n for t in c['topics'])+1
 tid=f'C{n:02}.{no:02}'
 t=dict(id=tid,chapter=n,chapterTitle=chlookup[n],domain=dom,title=title,summary=summary,blocks=blocks or [],concepts=concepts or [],cards=[],sourceRef=f'manuscript/{"primer" if n==0 else f"ch{n:02}"}.json',origin='textbook-companion');t.update(kw)
 for q,a in cards or []:t['cards'].append(dict(id=f'{tid}-c{len(t["cards"])+1}',question=q,answer=a))
 c['topics'].append(t);return t
P=lambda x:dict(type='paragraph',text=x)
L=lambda title,items:dict(type='list',title=title,items=items)
C=lambda label,code:dict(type='code',title=label,code=code)
for fn in ['front','primer']:
 d=load(R/'manuscript'/f'{fn}.json')
 for s in d['sections']:
  if 'biograph' in s['heading'].lower():continue
  paras=s.get('paragraphs',[])
  if not paras:continue
  blocks=[P(p) for p in paras[1:]]
  if s.get('code'):blocks.append(C(s.get('code_label','C++ excerpt'),s['code']))
  topic(0,s['heading'],paras[0],blocks,sourceRef=f'manuscript/{fn}.json → {s["heading"]}')
c['chapters'].append(dict(number=0,title=chlookup[0],partTitle='Foundations',part=0))
for n in range(1,24):
 d=load(R/'manuscript'/f'ch{n:02}.json');chlookup[n]=d['title'];al=d['attachment_alignment'];ex=d['example']
 c['chapters'].append(dict(number=n,title=d['title'],part=1 if n<=5 else 2 if n<=12 else 3 if n<=22 else 4,partTitle=d['part']))
 a=al.get('intent','');a=' '.join(a) if isinstance(a,list) else a
 cards=[(f'What problem does {d["title"]} address?',a or ex['design'])]
 cards += [(f'What is {x["term"]}?',x['definition']) for x in d['glossary']]
 cards += [(x['question'],x['answer']) for x in al['checks']]
 topic(n,'Pattern overview' if n<23 else 'Capstone overview',a or ex['design'],[L('Learning objectives',d['objectives']),P(al.get('mechanism',''))],cards=cards)
 for s in d['sections']:
  b=[P(p) for p in s['paragraphs'][1:]]
  if s.get('code'):b.append(C(s.get('code_label','C++ excerpt'),s['code']))
  topic(n,s['heading'],s['paragraphs'][0],b,sourceRef=f'manuscript/ch{n:02}.json → {s["heading"]}')
 roles=[dict(term=r['role']+' → '+r['example'],definition=r['responsibility']) for r in al['roles']]
 topic(n,'Roles in the implementation','Map the design responsibilities to the types and functions in the working C++ example.',concepts=roles,cards=[(f'In {d["title"]}, which part plays {r["role"]} and what does it do?',r['example']+'. '+r['responsibility']) for r in al['roles']])
 topic(n,'Worked example: contracts and alternatives',ex['title'],[L('Requirements',ex['requirements']),P(ex['design']),L('Invariants',ex['invariants']),P('Alternative: '+ex['alternative']),P('Verification: '+ex['verification']),P('Maintenance: '+ex['maintenance'])],cards=[(f'What must stay true in the {d["title"]} example?','\n'.join(ex['invariants'])),(f'What simpler alternative should you compare with {d["title"]}?',ex['alternative'])])
 topic(n,'Execution trace','Follow the data, control, ownership, and lifetime through the chapter’s worked call.',concepts=[dict(term='Step '+str(t['step'])+': '+t['state'],definition=t['reason']) for t in d['trace']])
 topic(n,'Failure cases and repairs','A compiling program can still violate its contract. Predict the actual consequence before reading the reason.',concepts=[dict(term=f['mistake'],definition=f['consequence']+' '+f['reason']) for f in ex['failures']],cards=[('What goes wrong if you '+f['mistake'][0].lower()+f['mistake'][1:],f['consequence']+' '+f['reason']) for f in ex['failures']])
 dg=load(R/'diagrams'/f'ch{n:02}.json');source=(R/'companion'/ex['file']).read_text();lines=source.splitlines()
 for f in dg['focus']:
  # Resolve against the exact packaged file, never a reformatted listing.
  start=f['resolved_start']-1
  end=f['resolved_end']-1
  assert f['start'].strip() in lines[start] and f['end'].strip() in lines[end],(n,f['title'])
  f.update(lineStart=start+1,lineEnd=end+1,code='\n'.join(lines[start:end+1]),file=ex['file'])
  assert len(f['explain'])==end-start+1,(n,f['title'])
  topic(n,'Code focus: '+f['title'],f['what'],[C(f'{ex["file"]}:{start+1}–{end+1}',f['code']),P('Where: '+f['where']),P('Why: '+f['why']),P('Invariant: '+f['invariant']),P('Risk: '+f['risk']),L('Line-by-line explanation',[f'{start+i+1}: {x}' for i,x in enumerate(f['explain'])])])
 c['diagrams'][str(n)]=dg
 e={}
 for k in ['examples','exercises','solutions']:
  p=R/'companion'/k/f'ch{n:02}'
  e[k]={'filename':f'{k}/ch{n:02}/main.cpp','code':(p/'main.cpp').read_text(),'output':(p/'expected.txt').read_text()}
 e['verification']=ex['verification'];e['lab']=d['lab'];c['examples'][str(n)]=e
 for q in d['exercises']:
  topic(n,'Exercise '+q['id']+': '+q['level'],q['prompt'],[dict(type='reveal',title='Hint',text=q['hint']),dict(type='reveal',title='Explained answer',text=q['answer'])],cards=[(q['prompt'],q['answer'])])
 lab=d['lab'];topic(n,'Implementation lab',lab['task'],[L('Acceptance checks',lab['checks']),P('Starter: '+lab['starter']+'\nSolution: '+lab['solution']),dict(type='reveal',title='Solution reasoning',text=lab['answer'])])
 topic(n,'Chapter summary','Use these statements to explain the pattern from memory.',[L('Main points',d['summary'])],concepts=d['glossary'])
 for g in d['glossary']:c['glossary'].append(dict(id=f'GL{len(c["glossary"])+1:03}',chapter=n,**g))
# Original scenarios are separate lessons with their answer feedback in Scenario mode.
for n,items in SCENARIOS.items():
 for i,(q,choices) in enumerate(items):
  indexed=list(enumerate(choices));random.Random(n*17+i+31).shuffle(indexed)
  sc=dict(prompt=q,options=[v[0] for _,v in indexed],correctIndex=next(i for i,(j,v) in enumerate(indexed) if j==0),explanation=choices[0][1],rationales=[v[1] for _,v in indexed])
  t=topic(n,f'Application case {i+1}',q,[P('Open Scenarios to select an answer and compare the reasoning for all four choices.')])
  sc['id']=t['id']+'-s1';t['scenario']=sc
# Foundations retrieve contracts and design distinctions, with full source explanations.
found=[('What is a design pattern?','A named arrangement of responsibilities that solves a recurring design problem. Identify the actual change pressure before choosing it.'),('How does a pattern differ from an algorithm?','An algorithm supplies computational steps. A pattern describes how responsibilities collaborate and vary.'),('What is an invariant?','A rule that must remain true at a stated program boundary, including after permitted operations.'),('What does ownership mean in C++?','Responsibility for an object or resource and its eventual release. Merely holding a pointer or reference does not establish ownership.'),('What does override check?','It asks the compiler to verify that the declaration overrides a base virtual function. It does not prove all behavioral promises.'),('What does const T& express?','A reference borrowing an existing T object, preventing mutation through that reference. Another alias may still change the object.'),('When is a simpler design enough?','When direct functions, a small branch, or a value type meet the requirements without needing the variation boundary a pattern introduces.'),('What must agree in a useful UML lesson?','The relationship legend, diagram blocks, source code, call trace, and lifetime rules must describe the same implementation.')]
topic(0,'Foundations recall','Review the words and decisions used across every pattern.',cards=found)
# One searchable glossary topic, plus cards with chapter context where wording overlaps.
g=dict(id='REF.GLOSSARY',kind='glossary',chapter=-1,chapterTitle='Glossary',domain='REF',title='Book glossary',summary='Search the terms used in the textbook and recall their meaning in context.',concepts=[],blocks=[],cards=[])
for entry in c['glossary']:g['cards'].append(dict(id=entry['id']+'-c1',question=entry['term']+' — what does it mean?',answer=entry['definition']))
c['topics'].sort(key=lambda t:(t['chapter'],int(t['id'].split('.')[1])))
c['topics'].append(g)
# Every field remains editable in the JSON study pack.
data={'schemaVersion':1,'courses':[c,{'id':'my-material','title':'My study material','subtitle':'Your own additions','description':'Add your own C++ notes, flashcards, and cases.','domains':[{'id':'1','title':'My topics'}],'topics':[]}]}
(R/'src/content.json').write_text(json.dumps(data,ensure_ascii=False,indent=2))
coverage={'chapters':len(c['chapters']),'patterns':22,'lessons':len(c['topics']),'cards':sum(len(t['cards']) for t in c['topics']),'scenarios':sum(bool(t.get('scenario')) for t in c['topics']),'glossary':len(c['glossary']),'diagramViews':sum(1+len(x['extra_overviews']) for x in c['diagrams'].values()),'focusedBlocks':sum(len(x['focus']) for x in c['diagrams'].values())}
(R/'docs/coverage.json').write_text(json.dumps(coverage,indent=2));print(coverage)

# Add the separate systems-programming lecture course from its supplied sources.
from make_systems import build_systems
data=build_systems(R,data)
(R/'src/content.json').write_text(json.dumps(data,ensure_ascii=False,indent=2))
