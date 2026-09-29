#!/usr/bin/env python3
"""Create a hierarchical teaching layer without discarding source topics or IDs."""
import json,re
from pathlib import Path
from teaching_examples import generate,ROWS
ROOT=Path(__file__).resolve().parents[1]
def clean_title(s):return re.sub(r'^(?:\d+\.\d+\s+|Worked [Ee]xample\s*[—–:]\s*)','',s)
def sentences(text):
 # Keep source prose intact; split only at sentence boundaries followed by a capital.
 return [p.strip() for p in re.split(r'(?<=[.!?])\s+(?=[A-Z])',text) if p.strip()]
def brief(text,limit=150):
 parts=sentences(text) if text else []
 s=''
 for part in parts:
  s=(s+' '+part).strip()
  if len(s)>=80:break
 return s if len(s)<=limit else s[:limit].rsplit(' ',1)[0]+'…'
def substantive(t):
 if t['id'].startswith('SP02.'):
  text=re.sub(r'^Slide \d+: .*?\. The focus here is .*?\. ','',t['summary'])
  return text.split(' The systems-programming point is')[0]
 return t['summary']
def build(data):
 generate()
 for c in data['courses']:
  if c['id'] not in ['design-patterns-cpp','systems-programming']:continue
  c['teaching']={};c['readingOrder']=[]
  for t in c['topics']:
   for key in ['outlineParent','sourceCompanions','reading','sectionId']:t.pop(key,None)
  is_sp=c['id']=='systems-programming'
  for ch in c['chapters']:
   n=ch['number'];topics=[t for t in c['topics'] if t['chapter']==n]
   if is_sp:
    workshop=ROWS[n]
    objectives=[f'Explain the idea of {clean_title(t["title"]).lower()} using the chapter example.' for t in topics if re.match(r'^\d+\.\d+ ',t['title']) and not t['title'].endswith('Deep Dive')][:4]
    if not objectives:objectives=['Follow data from source code through execution.','Separate language promises from platform behavior.','Use a concrete trace to explain and test a claim.']
    sections=[dict(title='Start with the problem',purpose=workshop['problem'],topics=[]),dict(title='Build the concepts',purpose='Learn the mechanisms in order, and connect each one to a concrete state change.',topics=[]),dict(title='Work through examples',purpose='Predict the result, follow the steps, and compare with the stated outcome.',topics=[]),dict(title='Discuss and practice',purpose='Explain the design choices, then attempt the lab and application case.',topics=[]),dict(title='Connect and review',purpose='Check what transfers to real programs and what depends on the model.',topics=[])]
    previous=None
    for t in topics:
     title=t['title']
     if title.endswith('— Deep Dive') and previous and title[:-12].strip()==previous['title'].strip():
      t['outlineParent']=previous['id'];previous.setdefault('sourceCompanions',[]).append(t['id']);continue
     previous=t
     if t.get('scenario') or re.search(r'Guided lab|Exercise|Lab:|Discussion|Practice|Assessment|Review Questions',title,re.I):idx=3
     elif re.search(r'Worked [Ee]xample|Demonstration|Case Study|Trace:|Walkthrough|Walk-through',title):idx=2
     elif re.search(r'Synthesis|Review and Transition|Takeaways|References|Closing|Next Chapter|Recap',title,re.I):idx=4
     elif (t.get('slide',99)<=3 or re.search(r'Roadmap|Learning Objectives|What This Chapter|Course Map|Course Road',title,re.I)):idx=0
     else:idx=1
     sections[idx]['topics'].append(t['id'])
     t['reading']={'title':clean_title(title),'paragraphs':sentences(substantive(t)),'preview':brief(substantive(t))}
    if n in (1,2):
     specs = [(3,'Start with the problem'),(13,'Hardware, boundaries, and contracts'),(18,'Source and preprocessing'),(25,'Compilation, assembly, and linking'),(28,'Loading and runtime'),(36,'Follow a complete build and execution'),(41,'Measure performance claims'),(47,'Ownership, invariants, and failure'),(52,'Portability and evidence'),(55,'Guided laboratory'),(59,'Discuss and defend your answer'),(999,'Connect and review')] if n==1 else [(3,'Start with the problem'),(13,'Fundamental types and representation'),(19,'Arrays, structures, and other compound types'),(27,'Functions, parameters, and scope'),(36,'Pointers, references, and bounds'),(41,'Object lifetime and storage'),(45,'Work through examples'),(46,'Guided laboratory'),(999,'Discuss and review')]
     sections=[dict(title=title,purpose='Study these related ideas together, then apply them to the chapter workshop.',topics=[]) for _,title in specs]
     for t in topics:
      idx=next(i for i,(end,_) in enumerate(specs) if t.get('slide',999)<=end)
      sections[idx]['topics'].append(t['id'])
    for t in topics:
     body=next((b['text'] for b in t['blocks'] if b['type']=='reveal'),'')
     if not t.get('outlineParent'):
      if n==1:
       narration=body.split('Graduate lecture narration\n',1)[-1].split('Sixth-grade explanation\n')[0].strip()
      elif n>2:
       narration=body.split('LECTURE NARRATION\n',1)[-1].split('SIXTH-GRADE EXPLANATION\n')[0].strip()
       narration='\n\n'.join(p for p in narration.split('\n\n') if not p.startswith('At graduate level, do not treat this as a term to memorize.'))
      else:narration=substantive(t)
      if t.get('slide'):t['reading']['paragraphs']=[p for p in narration.split('\n\n') if p.strip()]
      simple=re.search(r'(?:Sixth-grade explanation|SIXTH-GRADE EXPLANATION)\n(.+?)(?:\n\n|$)',body,re.S)
      if simple and not simple[1].startswith('In simpler terms,'):t['reading']['plain']=simple[1].strip()
     m=re.search(r'Discussion / pause\n(.+?)\n\nExpected response: (.+?)(?:\n\n|$)',body,re.S)
     if m:t.setdefault('reading',{})['discussion']=dict(question=m[1].strip(),answer=m[2].strip())
    # Corrections are visible teaching notes; the original transcript stays intact.
    for t in topics:
     if n==45 and 'Lost update race' in t['title']:
      t.setdefault('reading',{})['correction']='C++ correction: the source describes a conceptual read/add/write interleaving. Two unsynchronized ordinary counter++ operations in real C++ have a data race and undefined behavior; a final value of 1 is not guaranteed. The atomic chapter workshop below provides a defined, deterministic demonstration of the logical lost update.'
    for t in topics:
     if t.get('reading',{}).get('correction'):
      for card in t['cards']:card['answer']=t['reading']['correction']
    title=workshop['title'];objectives=objectives[:4]
   else:
    if n:
     d=json.loads((ROOT/f'manuscript/ch{n:02}.json').read_text());ex=d['example'];source=(ROOT/'companion'/ex['file']).read_text();dg=c['diagrams'][str(n)]
     f=dg['focus'][0]
     question=next((x for x in d['exercises'] if re.search(r'design|justif',x['level'],re.I)),d['exercises'][-1])
     workshop=dict(title=ex['title'],problem=ex['title']+'. '+' '.join(ex['requirements']),code=source,excerpt=f['code'],excerptLabel=f"{ex['file']}:{f['lineStart']}–{f['lineEnd']}",output=c['examples'][str(n)]['examples']['output'],steps=[dict(action='Step '+str(x['step']),state=x['state'],why=x['reason']) for x in d['trace']],pitfall=' '.join([ex['failures'][0][k] for k in ['mistake','consequence','reason']]),discussion=dict(question=question['prompt'],answer=question['answer'],hint=question['hint']),alternative=ex['alternative'],kind='Complete C++17 program',filename='companion/'+ex['file'],design=ex['design'],invariants=ex['invariants'],maintenance=ex['maintenance'],verification=ex['verification'])
     objectives=d['objectives'];title=ex['title']
    else:
     workshop=dict(title='Change a rule without copying the work',problem='A monitor must test the same reading against different limits. Start by making the changing value an input; a class hierarchy is not needed for this requirement.',code='#include <cassert>\n#include <iostream>\n\nbool above_limit(int value, int limit) {\n    return value > limit;\n}\n\nint main() {\n    const int reading = 8;\n    std::cout << above_limit(reading, 10) << " "\n              << above_limit(reading, 5) << "\\n";\n    assert(!above_limit(8, 10) && above_limit(8, 5));\n}\n',output='0 1\n',steps=[dict(action='First request',state='above_limit(8, 10) is false',why='The comparison > asks whether 8 is greater than 10. bool holds true or false; the default stream prints false as 0.'),dict(action='Second request',state='above_limit(8, 5) is true',why='The same operation uses a different input. No validation or call flow has been copied.'),dict(action='New kind of change',state='A rolling-average policy would need more than a limit',why='When the algorithm itself varies, Chapter 20 compares a policy interface with simpler function-based choices.')],pitfall='Copying the whole monitor for every threshold spreads the same checks across several places. A bug fix can then reach one copy and miss another.',discussion=dict(question='Should this two-threshold requirement use Strategy already?',answer='A function with a limit parameter is enough. Strategy becomes useful when whole algorithms vary behind a stable promise. Name the change before choosing a pattern; do not introduce a hierarchy just to change one integer.'),alternative='A direct comparison is enough at one call site. A function is useful when the meaning is shared and deserves one named contract.',kind='Complete C++17 program',filename='companion/foundations/main.cpp',invariants=['A value equal to the limit is not above it.','The function does not change the reading.'])
     p=ROOT/workshop['filename'];p.parent.mkdir(parents=True,exist_ok=True);p.write_text(workshop['code']);p.with_name('expected.txt').write_text(workshop['output'])
     objectives=['Choose a design by the change it must support.','Distinguish a value, reference, owner, and interface.','Trace data, calls, and lifetime separately.'];title=workshop['title']
    sections=[dict(title='Start with the problem',purpose='Understand the first design and the new requirement that puts pressure on it.',topics=[]),dict(title='Understand the mechanism',purpose='Connect the pattern roles, C++ rules, and alternatives to the working program.',topics=[]),dict(title='Trace code and behavior',purpose='Follow real source lines and check the promised output and invariants.',topics=[]),dict(title='Discuss and practice',purpose='Predict, diagnose, extend, and defend a design before reading the answer.',topics=[]),dict(title='Connect and review',purpose='Summarize when this design is useful and when a simpler choice is enough.',topics=[])]
    for i,t in enumerate(topics):
     title=t['title']
     if re.search(r'^Exercise|Implementation lab|Application case',title):idx=3
     elif re.search(r'Chapter summary|Foundations recall',title):idx=4
     elif re.search(r'Execution trace|Failure cases|Code focus|Worked example:',title):idx=2
     elif i<3:idx=0
     else:idx=1
     sections[idx]['topics'].append(t['id'])
     paras=[t['summary']]+[b['text'] for b in t['blocks'] if b['type']=='paragraph']
     t['reading']={'title':t['title'],'paragraphs':paras,'preview':brief(t['summary'])}
   for idx,s in enumerate(sections):
    s['id']=f'{n:02}-{idx+1}';s['number']=idx+1
    for tid in s['topics']:
     next(t for t in topics if t['id']==tid)['sectionId']=s['id']
     c['readingOrder'].append(tid)
   # A source duplicate points back to the same outline section.
   for t in topics:
    if t.get('outlineParent'):t['sectionId']=next(x for x in topics if x['id']==t['outlineParent'])['sectionId']
   c['teaching'][str(n)]=dict(chapter=n,startTopicId=topics[0]['id'],title=ch['title'],objectives=objectives,sections=sections,workshop=workshop)
  for t in c['topics']:
   if t.get('reading') and 'roadmap' in t['title'].lower():t['reading']['preview']='See how the source lecture topics fit into the numbered sections of this chapter.'
  for n,g in c['teaching'].items():
   ts=[t for t in c['topics'] if t['chapter']==int(n) and not t.get('outlineParent')]
   if is_sp:g['firstLessonId']=next((t['id'] for t in ts if re.match(r'^\d+\.',t['title'])),ts[2]['id'])
   else:g['firstLessonId']=ts[3]['id'] if int(n)==0 else ts[1]['id']
  c['readingOrder'] += [t['id'] for t in c['topics'] if t['chapter']==-1]
  if is_sp:
   c['provenance']='All 61 supplied chapter presentations and full transcripts are retained with source slide references. The reading layer groups paired Deep Dive slides and adds original C++17 chapter workshops, traces, and explained discussions. These new demonstration programs are independently compiled and tested; models are explicitly labeled. The supplied systems lecture package did not contain the companion source listings it names, so those absent original listings have not been compiled. Original source transcripts remain unchanged. A visible C++ race-condition correction accompanies the affected lesson and recall answer.'
   c['description']='Learn systems through chapter maps, C++ demonstrations, step-by-step traces, and explained discussions. Keep the original lectures beside your study.'
  else:c['description']='Learn the design patterns through concrete problems, working C++, UML, and explained design choices. Use the chapter map to follow the lesson.'
 report={c['id']:{'chapters':len(c['teaching']),'sections':sum(len(x['sections']) for x in c['teaching'].values()),'readingEntries':len(c['readingOrder']),'sourceTopicsRetained':len(c['topics']),'workshops':len(c['teaching'])} for c in data['courses'] if c.get('teaching')}
 (ROOT/'docs/teaching-coverage.json').write_text(json.dumps(report,indent=2)+'\n')
 print(report)
 return data
if __name__=='__main__':
 p=ROOT/'src/content.json';p.write_text(json.dumps(build(json.loads(p.read_text())),ensure_ascii=False,indent=2))
