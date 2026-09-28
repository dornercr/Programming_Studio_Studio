#!/usr/bin/env python3
"""Extract every lecture slide and transcript into a separate Study Studio course."""
import base64,csv,hashlib,json,re,random,zipfile,xml.etree.ElementTree as ET
from pathlib import Path
from systems_scenarios import ROWS
NS={'a':'http://schemas.openxmlformats.org/drawingml/2006/main'}
GROUPS=[(2,'FOUND','Systems foundations'),(11,'MEM','Data and memory'),(18,'CPU','Instructions and compilation'),(26,'VM','Kernel and virtual memory'),(32,'CACHE','Memory hierarchy and caches'),(42,'PROC','Files and processes'),(50,'SYNC','Threads and synchronization'),(53,'NET','Networks and servers'),(56,'PERF','Measurement and optimization'),(59,'SAFE','Security and robustness'),(61,'FULL','Whole-system integration')]
def build_systems(root,data):
 source=root/'systems_source';mapping=list(csv.DictReader((source/'Master_Course_Coverage_Map.csv').open()))
 titles={int(r['chapter']):r['chapter_title'] for r in mapping};titles.update({1:'Understanding Computer Systems',2:'C and C++ for Systems Programming'})
 c=dict(id='systems-programming',kind='textbook',title='Systems Programming and Machine Organization',subtitle='Dr. Charles Dorner',author='Dr. Charles Dorner',description='Follow all 61 chapters from C and C++ foundations through memory, machine code, operating systems, concurrency, networking, performance, and robustness.',coverageLabel='61 chapters · Complete supplied lecture series',familyLabel='Subject areas',provenance='Extracted from the supplied 61-chapter graduate lecture series. Every student slide has a lesson with its text and matching verbatim transcript section. Original PowerPoint decks and complete transcripts are available in Lectures. Recall cards are source-based; the 61 application cases are new study questions. Source code shown on lecture slides is excerpted teaching material unless the source explicitly identifies a complete program. This addition does not claim to compile or validate programs absent from the lecture package.',domains=[dict(id=g,title=t) for _,g,t in GROUPS],topics=[],chapters=[],glossary=[],examples={},diagrams={},lectures={})
 audit=[]
 for n in range(1,62):
  folder=source/f'Chapter_{n:02}';ppt=next(folder.glob('*.pptx'));txt=next(folder.glob('*.txt'));transcript=txt.read_text()
  pattern=r'^(\d{2})\. (.+)$' if n==1 else r'^SLIDE (\d+)\s*(?:—|:)\s*(.+)$'
  heads=list(re.finditer(pattern,transcript,re.M));assert heads,(n,'no transcript headings')
  chunks={int(h[1]):(h[2],transcript[h.end():heads[i+1].start() if i+1<len(heads) else len(transcript)].strip().strip('=\n').strip()) for i,h in enumerate(heads)}
  group=next((g,t) for stop,g,t in GROUPS if n<=stop);chaptertitle=titles[n]
  c['chapters'].append(dict(number=n,title=chaptertitle,part=next(i for i,x in enumerate(GROUPS) if n<=x[0]),partTitle=group[1]))
  with zipfile.ZipFile(ppt) as z:
   names=sorted([p for p in z.namelist() if re.fullmatch(r'ppt/slides/slide\d+\.xml',p)],key=lambda p:int(re.search(r'slide(\d+)',p)[1]))
   assert set(chunks)==set(range(1,len(names)+1)),(n,len(chunks),len(names))
   for slide,name in enumerate(names,1):
    xml=ET.fromstring(z.read(name));paragraphs=[''.join(t.text or '' for t in p.findall('.//a:t',NS)) for p in xml.findall('.//a:p',NS)]
    paragraphs=[p for p in paragraphs if p.strip()]
    title,body=chunks[slide]
    # Retain the full narration below; use its first substantive paragraph as the overview.
    if n==1:lead=body.split('Graduate lecture narration\n',1)[-1].split('\n\n')[0]
    elif n==2:lead=body.split('\n\n')[0]
    else:lead=body.split('LECTURE NARRATION\n',1)[-1].split('\n\n')[0]
    lead=lead.strip();assert lead
    tid=f'SP{n:02}.{slide:03}'
    filtered=[p.lstrip('• ').strip() for p in paragraphs if not p.startswith('SYSTEMS PROGRAMMING AND MACHINE ORGANIZATION') and p!='CHARLES DORNER' and not re.match(r'^CHAPTER \d+\s*\|',p)]
    blocks=[dict(type='list',title=f'Student slide {slide}: source text',items=filtered),dict(type='reveal',title='Full professor transcript for this slide',text=body)]
    cards=[]
    skip=bool(re.search(r'Chapter \d+ [—–]|Roadmap|What .*Builds|Review and Transition|Chapter Synthesis|References:|Takeaways|complete Chapter|route',title,re.I)) or title.endswith('— Deep Dive')
    if not skip:
     answer=lead
     # Chapter 2 narration starts with a slide label and a repeated methodological ending.
     if n==2:
      answer=re.sub(r'^Slide \d+: .*?\. The focus here is .*?\. ','',answer)
      answer=answer.split(' The systems-programming point is')[0]
     cards.append(dict(id=tid+'-C1',question=f'Explain {title}. What mechanism, rule, or distinction must you remember?',answer=answer))
     if 'Expected response:' in body:
      m=re.search(r'Discussion / pause\n(.+?)\n\nExpected response: (.+?)(?:\n\n|$)',body,re.S)
      if m:cards.append(dict(id=tid+'-C2',question=m[1].strip(),answer=m[2].strip()))
    c['topics'].append(dict(id=tid,chapter=n,chapterTitle=chaptertitle,domain=group[0],title=title,summary=lead,concepts=[],blocks=blocks,cards=cards,sourceRef=f'systems_source/Chapter_{n:02}/{ppt.name} · slide {slide}; {txt.name} · slide {slide}',slide=slide,origin='supplied-lecture-series'))
  c['lectures'][str(n)]={'title':chaptertitle,'slideCount':len(names),'presentationName':ppt.name,'presentationBase64':base64.b64encode(ppt.read_bytes()).decode(),'transcriptName':txt.name,'transcript':transcript}
  q,*other=ROWS[n];choices=other[:4];explanation=other[4]
  order=list(enumerate(choices));random.Random(n+1200).shuffle(order)
  tid=f'SP{n:02}.CASE'
  scenario=dict(id=tid+'-S1',prompt=q,options=[s for _,s in order],correctIndex=next(i for i,(k,_) in enumerate(order) if k==0),explanation=explanation)
  c['topics'].append(dict(id=tid,chapter=n,chapterTitle=chaptertitle,domain=group[0],title='Application case: '+chaptertitle,summary=q,concepts=[],blocks=[dict(type='paragraph',text='Open Scenarios to choose an answer, then compare your reasoning with the explained solution.')],cards=[],scenario=scenario,sourceRef=f'Original study question for lecture chapter {n}',origin='original-practice'))
  audit.append(dict(chapter=n,title=chaptertitle,slides=len(names),lessons=len(names)+1,cards=sum(len(t['cards']) for t in c['topics'] if t['chapter']==n),transcriptSections=len(chunks),presentationSHA256=hashlib.sha256(ppt.read_bytes()).hexdigest(),transcriptSHA256=hashlib.sha256(txt.read_bytes()).hexdigest()))
 data['courses']=[x for x in data['courses'] if x['id']!='systems-programming']
 data['courses'].insert(1,c)
 report={'chapters':61,'slides':sum(x['slides'] for x in audit),'lessons':len(c['topics']),'cards':sum(x['cards'] for x in audit),'scenarios':61,'chapterCoverage':audit}
 (root/'docs/systems-coverage.json').write_text(json.dumps(report,indent=2)+'\n')
 print({k:v for k,v in report.items() if k!='chapterCoverage'})
 return data
if __name__=='__main__':
 root=Path(__file__).resolve().parents[1];p=root/'src/content.json';p.write_text(json.dumps(build_systems(root,json.loads(p.read_text())),ensure_ascii=False,indent=2))
