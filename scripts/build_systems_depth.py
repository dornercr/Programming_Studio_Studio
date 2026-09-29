#!/usr/bin/env python3
"""Add the Systems teaching, UML and lab layers without replacing source lectures."""
import json,re
from pathlib import Path
from systems_labs import generate_labs,LABS
from systems_focus import FOCUS,INVARIANTS
ROOT=Path(__file__).resolve().parents[1]
def build(data):
 generate_labs()
 c=next(c for c in data['courses'] if c['id']=='systems-programming')
 c['topics']=[t for t in c['topics'] if t.get('origin')!='systems-depth']
 c['examples']={};c['diagrams']={};c['glossary']=[];c['readingOrder']=[]
 authored=[]
 for ch in c['chapters']:
  n=ch['number'];key=str(n);g=c['teaching'][key];w=g['workshop'];lab=LABS[n]
  source=w['code'].splitlines();needle,length,explain=FOCUS[n];start=next(i for i,l in enumerate(source) if needle in l);selected=source[start:start+length]
  assert len(selected)==len(explain)
  focus=dict(title=w['title'],file=w['filename'],lineStart=start+1,lineEnd=start+length,code='\n'.join(selected),explain=explain,what=w['title']+'. '+explain[0],where=w['problem'],why=' '.join(s['why'] for s in w['steps']),invariant=INVARIANTS[n],risk=w['pitfall']+' In the extended lab: '+lab['bug'])
  main=source.index('int main() {');initial=source[main+1:min(main+3,start)] or source[main+1:main+2]
  output_index=next((i for i in range(start+length,len(source)) if 'std::cout' in source[i]),None)
  if output_index is None:output_index=next((i for i in range(start+length,len(source)) if 'assert(' in source[i]),None)
  final=source[output_index:output_index+1] if output_index is not None else ['// Observe the result shown in the focused block.']
  if output_index is not None:
   while not final[-1].rstrip().endswith(';') and output_index+len(final)<len(source)-1:final.append(source[output_index+len(final)])
  nodes=[dict(id='input',label='Establish the starting state',code=['// Inputs or state for this execution.']+[s.strip() for s in initial]),dict(id='mechanism',label=w['title'],code=['// Focus: follow the operation below.']+[s.strip() for s in selected]),dict(id='observe',label='Observe the promised result',code=['// Check the result; retain the exact output.']+[s.strip() for s in final])]
  edges=[dict(from_='input',to='mechanism',kind='flow',label='Execute the selected operation'),dict(from_='mechanism',to='observe',kind='flow',label='Observe after the operation')]
  for e in edges:e['from']=e.pop('from_')
  overview=dict(title='From starting state to observable result',kind='activity',nodes=nodes,edges=edges,explanation='Solid arrows show the selected execution path through this example, not ownership or inheritance. Blocks contain exact C++ excerpts plus // teaching comments; intervening declarations are in the complete program. Branches and loops within a block keep their C++ meaning. The focused panel explains the same operation line by line.')
  c['diagrams'][key]=dict(source=w['filename']+' (chapter demonstration); every focused source reference is checked during the build',overview=overview,extra_overviews=[],focus=[focus])
  c['examples'][key]=dict(examples=dict(filename=w['filename'],code=w['code'],output=w['output']),exercises=dict(filename=lab['starter'],code=(ROOT/lab['starter']).read_text(),output=w['output']),solutions=dict(filename=lab['solution'],code=lab['code'],output=lab['output']),verification='The complete program is compiled as C++17 with warnings treated as errors; actual standard output is compared byte for byte with expected.txt. '+w['kind']+'. '+w['pitfall'],lab=dict(task=lab['task'],checks=lab['checks'],starter=lab['starter'],solution=lab['solution'],answer=lab['reason']+' Repair principle: '+lab['repair']))
  w['excerpt']=focus['code'];w['excerptLabel']=f"{w['filename']}:{focus['lineStart']}–{focus['lineEnd']}"
  w['design']=lab['reason'];w['invariants']=lab['checks'];w['maintenance']=lab['repair'];w['verification']='Build the complete demonstration and compare every output character. The separate lab solution includes assertions for its named acceptance checks.'
  # The workshop invariants must describe its own program, not the generalized lab.
  w['invariants']=[INVARIANTS[n]]
  sections=[dict(title='Start with the problem',purpose='Identify the concrete requirement and the limitation that creates a need for this mechanism.',topics=[]),dict(title='Understand the mechanism',purpose='Connect the source lecture concepts with exact C++ operations, contracts, and alternatives.',topics=[]),dict(title='Trace code and behavior',purpose='Run the demonstration, follow the UML view, and diagnose a specific failure.',topics=[]),dict(title='Discuss and practice',purpose='Work through six independent exercises, then build and test the extension lab.',topics=[]),dict(title='Connect and review',purpose='Explain what transfers to a real system, which assumptions remain, and what to test after a change.',topics=[])]
  counter=0
  def topic(section,title,paragraphs,blocks=None,discussion=None,concepts=None,exercise=None):
   nonlocal counter
   counter+=1;tid=f'SYS{n:02}.{counter:02}'
   t=dict(id=tid,chapter=n,chapterTitle=ch['title'],domain=next(t['domain'] for t in c['topics'] if t['chapter']==n),title=title,summary=paragraphs[0],blocks=blocks or [],concepts=concepts or [],cards=[],sourceRef=f'teaching/systems-depth.json → Chapter {n}; {w["filename"]}; {lab["solution"]}',origin='systems-depth',reading=dict(title=title,paragraphs=paragraphs,preview=paragraphs[0][:150]))
   if discussion:t['reading']['discussion']=discussion
   if exercise:t['exercise']=exercise
   authored.append(t);sections[section]['topics'].append(tid)
   return t
  L=lambda title,items:dict(type='list',title=title,items=items)
  C=lambda title,code:dict(type='code',title=title,code=code)
  R=lambda title,text:dict(type='reveal',title=title,text=text)
  intro=topic(0,'Problem and first implementation',[w['problem'],'Begin with the complete chapter demonstration. It fixes the input so you can follow each change in state. After that trace is clear, the lab expands the contract: '+lab['task']], [L('The extension must preserve',lab['checks'])])
  topic(0,'Where the simple approach fails',[w['pitfall'],'A related failure in the generalized operation is concrete: '+lab['bug'],'The repair changes the operation at the point where the rule can be enforced: '+lab['repair']],discussion=dict(question='Which requirement fails first, and where should its check or protection live?',answer=lab['repair']))
  mechanism=topic(1,'Mechanism, guarantees, and limits',[lab['reason'],'A plausible alternative: '+w['alternative']],concepts=lab['terms'],discussion=w['discussion'])
  mechanism['cards']=[dict(id=f'{mechanism["id"]}-c1',question=lab['terms'][0]['term']+' — what does it mean here?',answer=lab['terms'][0]['definition']),dict(id=f'{mechanism["id"]}-c2',question='What assumption matters in '+lab['title']+'?',answer=lab['reason'])]
  topic(2,'Execution trace and checked output',[w['problem']+' Predict the output before running the complete example.',' '.join(s['action']+': '+s['state']+' '+s['why'] for s in w['steps'])],[C('Complete C++17 demonstration — '+w['filename'],w['code']),C('Build from the source-package root',f'g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread {w["filename"]} -o example\n./example'),C('Expected standard output',w['output'])])
  topic(2,'Code focus: syntax and meaning',[w['title']+'. The following excerpt is part of the complete program, not a separate compilable file.','Open UML for the activity overview and its adjacent theory panel. The source range below is checked against the packaged file.'],[C(f'Exact excerpt — {focus["file"]}:{focus["lineStart"]}–{focus["lineEnd"]}',focus['code']),L('Read each source line', [f'Line {start+i+1}: {x}' for i,x in enumerate(explain)])])
  topic(2,'Failure diagnosis and repair',[lab['bug'],lab['repair'],'The reference solution makes the contract executable with assertions. '+lab['reason']],[L('Meaningful verification',lab['checks'])])
  exercises=[
   ('Guided modification',f'Start independently from {lab["starter"]}. Implement the first acceptance case for {lab["title"].lower()}. State the function inputs and result before adding code. First case: '+lab['checks'][0], 'Use the first check as a tiny contract. Follow the check-before-use or state-update order explained in the code focus.',lab['repair']+' The reference implementation is '+lab['solution']+'. Its first required behavior is: '+lab['checks'][0]),
   ('Trace and predict',f'Without running {w["filename"]}, predict its exact output and explain the state after each step. Work from the complete demonstration in “Execution trace and checked output.”','Track each value independently. Distinguish copied values, shared state, and the point where output is produced.','Expected output:\n'+w['output']+'\n'+'\n'.join(s['action']+': '+s['state']+'. '+s['why'] for s in w['steps'])),
   ('Diagnose a bug','An implementation makes this mistake: '+lab['bug']+' Explain the failure and repair it. Identify one check that would expose the mistake.','Find the rule that must hold before the operation touches data or commits state.',lab['repair']+' Relevant checks: '+' '.join(lab['checks'])),
   ('Choose a design',w['discussion']['question']+' Compare the selected design with this plausible alternative: '+w['alternative'],'Name the expected change or workload first. Compare guarantees, ownership, and the cost of added machinery.',w['discussion']['answer']+' The extended lab uses this reasoning: '+lab['reason']+' Another solution is reasonable if it meets the same contract with fewer moving parts for its actual workload.'),
   ('Extend without breaking tests',f'Start independently from {lab["starter"]}. '+lab['task']+' Preserve the starter’s behavior where the contract still applies, and add every acceptance check listed below. Compare your completed result with '+lab['solution']+'.','Keep the original example as a regression fixture. Add rejection and boundary checks before broadening valid input ranges. Do not delete checks merely to make the build pass.',lab['reason']+' Full reference source: '+lab['solution']+'.\nExpected solution output:\n'+lab['output']),
   ('Justify the boundary','In plain English, explain why the checks or synchronization in '+lab['title']+' belong where the solution puts them. Name an assumption that would need a new test if it changed.','Use one successful case, one rejected or boundary case, and one ownership or state rule. A claim about speed requires measurements, not just a diagram.',lab['reason']+' The maintenance decision follows from this repair: '+lab['repair'])
  ]
  for i,(level,prompt,hint,answer) in enumerate(exercises,1):
   blocks=[L('Acceptance checks',lab['checks'])] if i in (1,3,5) else []
   blocks += [R('Hint — open after your first attempt',hint),R('Explained answer — compare your reasoning',answer)]
   topic(3,f'Exercise {i}: {level}',[prompt,'This exercise is independent: use the named source file and the chapter discussion; you do not need a previous exercise’s edits.'],blocks,discussion=dict(question=prompt,hint=hint,answer=answer),exercise=dict(level=level,prompt=prompt,hint=hint,answer=answer))
  topic(3,'Implementation lab: '+lab['title'],[lab['task'],'The C++ tab contains three separate complete files: the chapter example, a starter that preserves its initial behavior, and the reference solution. The starter is not a completed extension.'],[L('Acceptance checks',lab['checks']),C('Build the reference solution from the package root',f'g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread {lab["solution"]} -o solution\n./solution'),C('Expected solution output',lab['output']),R('Why this solution works',lab['reason'])])
  topic(4,'Chapter summary and maintenance',[lab['reason'],'Choose a simpler approach when its narrower contract is enough. '+w['alternative'],'When this system changes, retain a check that exposes the known failure: '+lab['bug']+' '+lab['repair']], [L('Recall without looking', [s['action']+': explain '+s['state'] for s in w['steps']])])
  # Retain the source sequence and original sections as grouped related readings.
  oldtopics=[t for t in c['topics'] if t['chapter']==n and not t.get('outlineParent')]
  for t in oldtopics:
   title=t['title']
   if t.get('scenario') or re.search(r'Guided lab|Exercise|Lab:|Discussion|Practice|Assessment|Review Questions',title,re.I):idx=3
   elif re.search(r'Worked [Ee]xample|Demonstration|Case Study|Trace:|Walkthrough|Walk-through',title):idx=2
   elif re.search(r'Synthesis|Review and Transition|Takeaways|References|Closing|Next Chapter|Recap',title,re.I):idx=4
   elif (t.get('slide',99)<=3 or re.search(r'Roadmap|Learning Objectives|What This Chapter|Course Map|Course Road',title,re.I)):idx=0
   else:idx=1
   sections[idx]['topics'].append(t['id'])
  lookup={t['id']:t for t in c['topics']+authored}
  for i,s in enumerate(sections,1):
   s['id']=f'{n:02}-{i}';s['number']=i
   for tid in s['topics']:lookup[tid]['sectionId']=s['id']
   c['readingOrder']+=s['topics']
  for t in c['topics']:
   if t['chapter']==n and t.get('outlineParent'):t['sectionId']=lookup[t['outlineParent']]['sectionId']
  # Keep startTopicId stable for existing notes and remembered course position.
  g.update(sections=sections,firstLessonId=intro['id'],objectives=[f'Explain {lab["terms"][0]["term"].lower()} and {lab["terms"][1]["term"].lower()} in context.','Trace the demonstration and explain its output.',f'Complete and test the extension: {lab["title"].lower()}.'])
  for j,term in enumerate(lab['terms'],1):c['glossary'].append(dict(id=f'SYS{n:02}-g{j}',chapter=n,**term))
 c['topics']+=authored
 glossary=dict(id='REF.GLOSSARY',kind='glossary',chapter=-1,chapterTitle='Glossary',domain='REF',title='Systems glossary',summary='Systems terms with concrete chapter context.',concepts=[],blocks=[],cards=[dict(id=g['id']+'-c1',question=g['term']+' — what does it mean?',answer=g['definition']) for g in c['glossary']],origin='systems-depth')
 c['topics'].append(glossary);c['readingOrder'].append(glossary['id'])
 c['description']='Learn systems through the same problem, mechanism, trace, UML, C++, and practice structure as Design Patterns. All 61 chapters include a complete demonstration, a boundary-tested extension lab, and explained answers.'
 c['provenance']='All 61 supplied presentations and transcripts remain intact, with all 1775 slide records and existing study IDs. The added study curriculum supplies 61 original C++17 demonstrations, 61 lab starters, 61 reference solutions, exact source-linked diagram panels, six independent exercises per chapter, and 122 glossary entries. Models of hardware and OS behavior are labeled; they do not make real system calls. Original companion listings named by some supplied lectures were absent from the upload and are not claimed as tested here. The race-condition correction remains visible beside its source lesson.'
 from systems_relationships import add_views
 add_views(c)
 (ROOT/'teaching/systems-depth.json').write_text(json.dumps(dict(topics=authored,glossary=c['glossary']),ensure_ascii=False,indent=2)+'\n')
 (ROOT/'docs/systems-depth-coverage.json').write_text(json.dumps(dict(chapters=61,sourceSlides=1775,originalTopics=1836,addedTopics=len(authored)+1,examples=61,starters=61,solutions=61,exercises=366,glossary=122,diagramViews=sum(1+len(d['extra_overviews']) for d in c['diagrams'].values()),focusedBlocks=sum(len(d['focus']) for d in c['diagrams'].values())),indent=2)+'\n')
 report={course['id']:dict(chapters=len(course['teaching']),sections=sum(len(g['sections']) for g in course['teaching'].values()),readingEntries=len(course['readingOrder']),sourceTopicsRetained=len(course['topics']),workshops=len(course['teaching'])) for course in data['courses'] if course.get('teaching')}
 (ROOT/'docs/teaching-coverage.json').write_text(json.dumps(report,indent=2)+'\n')
 return data
if __name__=='__main__':
 p=ROOT/'src/content.json';p.write_text(json.dumps(build(json.loads(p.read_text())),ensure_ascii=False,indent=2))
