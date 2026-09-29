#!/usr/bin/env python3
"""Import all supplied EPUB spine content into structured, inert study data."""
import base64,collections,hashlib,json,posixpath,re,zipfile
from pathlib import Path
import xml.etree.ElementTree as E
ROOT=Path(__file__).resolve().parents[1]
ROMANS=['I','II','III','IV','V','VI','VII','VIII']
def tag(e):return e.tag.rsplit('}',1)[-1]
def plain(e):return ''.join(e.itertext())
def normalized(s):return ' '.join(s.split())
def inline(e,file):
 out=[]
 if e.text:out.append(e.text)
 for ch in e:
  t=tag(ch)
  if t in ['script','style']:continue
  out.append(dict(tag=t if t in ['strong','b','em','i','code','a','sub','sup','br','span','p','ul','ol','li','dl','dt','dd'] else 'span',children=inline(ch,file),**({'href':ch.get('href',''),'file':file} if t=='a' else {})))
  if ch.tail:out.append(ch.tail)
 return out

def events(body,file):
 counter=0
 def walk(e,parents=(),context=''):
  nonlocal counter
  t=tag(e);ident=e.get('id');parents=parents+((ident,) if ident else ())
  cls=e.get('class','')
  if 'material-example' in cls or 'section-correction' in cls:context=ident or f'{file}:example:{counter}'
  if t in ['script','style','svg','img']:return
  if t in ['h1','h2','h3','h4','h5','h6','p','pre','ul','ol','table','blockquote','dl']:
   counter+=1
   b=dict(id='B'+hashlib.sha256(f'{file}:{counter}'.encode()).hexdigest()[:14],kind=t,sourceFile=file,anchors=list(parents),context=context,text=plain(e),rich=inline(e,file))
   if t=='pre':
    code=next((n for n in e.iter() if tag(n)=='code'),e);language=(e.get('class','')+' '+code.get('class','')).strip();language=re.sub(r'language-','',language).split();b.update(code=plain(e),language=language[0] if language else 'text');b.pop('rich')
   if t in ['ul','ol']:b['items']=[inline(n,file) for n in e if tag(n)=='li']
   if t=='table':b['rows']=[[dict(header=tag(td)=='th',rich=inline(td,file)) for td in tr if tag(td) in ['th','td']] for tr in e.iter() if tag(tr)=='tr']
   yield b;return
  if e.text and e.text.strip():
   synthetic=E.Element('p');synthetic.text=e.text;yield from walk(synthetic,parents,context)
  for ch in e:
   yield from walk(ch,parents,context)
   if ch.tail and ch.tail.strip():
    synthetic=E.Element('p');synthetic.text=ch.tail;yield from walk(synthetic,parents,context)
 yield from walk(body)

def read_epub(path):
 z=zipfile.ZipFile(path);container=E.fromstring(z.read('META-INF/container.xml'));opf=next(x.get('full-path') for x in container.iter() if tag(x)=='rootfile');root=E.fromstring(z.read(opf));base=posixpath.dirname(opf)
 items={e.get('id'):e for e in root.iter() if tag(e)=='item'}
 title=next(plain(e) for e in root.iter() if tag(e)=='title');author=next(plain(e) for e in root.iter() if tag(e)=='creator')
 stream=[];spine=[]
 for e in root.iter():
  if tag(e)!='itemref':continue
  item=items[e.get('idref')];file=posixpath.normpath(posixpath.join(base,item.get('href')))
  if 'nav' in item.get('properties',''):continue
  doc=E.fromstring(z.read(file));body=next((n for n in doc.iter() if tag(n)=='body'),None)
  if body is None:continue
  es=list(events(body,file));spine.append(dict(file=file,sha256=hashlib.sha256(z.read(file)).hexdigest(),blocks=len(es)));stream.extend(es)
 return title,author,stream,spine

def build(data):
 data['courses']=[c for c in data['courses'] if not c.get('series')]
 reports=[]
 verification_path=ROOT/'docs/cpp-series-code-verification.json'
 verification=json.loads(verification_path.read_text()) if verification_path.exists() else {}
 checks={e['id']:e for e in verification.get('results',[])}
 for number,roman in enumerate(ROMANS,1):
  epub=next((ROOT/'cpp_series/books').glob(f'Book_{roman}_*.epub'));pdf=epub.with_suffix('.pdf')
  title,author,stream,spine=read_epub(epub)
  maxchapter=max(int(m[1]) for b in stream if b['kind']=='h1' and (m:=re.match(r'^Chapter (\d+)\b',normalized(b['text']))))
  course=dict(id=f'cpp-book-{number:02}',kind='textbook',title=f'Book {roman} · {title}',subtitle='C++ From Beginner to Expert',author=normalized(author),description='Read the complete book by chapter and section. Follow its worked examples, practice with source answers, inspect and download the code, and keep your own notes.',coverageLabel=f'Book {roman} · {maxchapter} chapters + front matter and reference',familyLabel='Reading areas',domains=[dict(id='CORE',title='Chapters'),dict(id='FRONT',title='Using this book'),dict(id='REF',title='Appendices and references')],topics=[],chapters=[],glossary=[],teaching={},readingOrder=[],examples={},diagrams={},series=dict(number=number,roman=roman,sourceTitle=title,spine=spine,listings=[],links={},assets={}),provenance='Imported from the supplied corrected EPUB and PDF pair. The reading preserves the EPUB spine, sections, examples, tables, and source wording. Extracted listings retain their source text; validation status distinguishes complete compiled programs from excerpts and platform-dependent material. The ZIP contains ebooks, not the separate original companion directories named by some book commands. The added study diagrams are identified as teaching views.')
  current=0;nextref=maxchapter+1;g=None;section=None;topic=None;index=0;blockcount=0;last_heading='';context_blocks=collections.defaultdict(list)
  def new_guide(n,title,role):
   nonlocal g,section,topic,current
   current=n;g=dict(chapter=n,title=title,sections=[],startTopicId=None,firstLessonId=None,role=role)
   course['teaching'][str(n)]=g;course['chapters'].append(dict(number=n,title=title,part=0 if role=='front' else 1 if role=='chapter' else 2,partTitle={'front':'Using this book','chapter':'Chapters','reference':'Appendices and reference'}[role],role=role))
   section=None;topic=None
  def new_section(title):
   nonlocal section,topic
   section=dict(id=f'B{number:02}-{current:02}-{len(g["sections"])+1:02}',number=len(g['sections'])+1,title=title,purpose='',topics=[]);g['sections'].append(section);topic=None
  def new_topic(title,kind='lesson'):
   nonlocal topic,index
   if section is None:new_section('Start here')
   index+=1;tid=f'B{number:02}.C{current:02}.T{index:04}'
   topic=dict(id=tid,chapter=current,chapterTitle=g['title'],domain='CORE' if g['role']=='chapter' else 'FRONT' if g['role']=='front' else 'REF',title=title,summary='',concepts=[],cards=[],blocks=[],sourceRef=epub.name,origin='cpp-series-epub',sectionId=section['id'],seriesKind=kind,practice=[],reading=dict(title=title,paragraphs=[],preview=''))
   course['topics'].append(topic);section['topics'].append(tid);course['readingOrder'].append(tid)
   if not g['startTopicId']:g['startTopicId']=tid
   if not g['firstLessonId']:g['firstLessonId']=tid
  new_guide(0,'Before you begin','front')
  for b in stream:
   heading=normalized(b['text']);kind=b['kind']
   if kind=='h1':
    if m:=re.match(r'^Chapter (\d+)\s*[—–:-]?\s*(.*)',heading):new_guide(int(m[1]),m[2] or heading,'chapter');new_section('The chapter problem');new_topic('Chapter overview')
    elif heading.startswith(('Appendix','Glossary','Primary reference','Technical reference','Shared source','Closing Perspective','Section example revision')):
     new_guide(nextref,heading,'reference');nextref+=1;new_section('Read and apply');new_topic(heading)
    else:
     if current!=0:new_guide(nextref,heading,'reference');nextref+=1
     new_section(heading);new_topic(heading)
   elif kind=='h2':new_section(heading);new_topic(heading,'section')
   elif kind=='h3':new_topic(heading,'example' if b['context'] or re.search(r'worked|example|complete.*source|main.cpp',heading,re.I) else 'lesson')
   elif topic is None:new_topic(g['title'])
   last_heading=heading if kind.startswith('h') else last_heading
   b['headingContext']=last_heading
   topic['blocks'].append(b);blockcount+=1
   context_blocks[b['context'] or topic['id']].append(b)
   for anchor in b['anchors']:course['series']['links'].setdefault(b['sourceFile']+'#'+anchor,dict(topicId=topic['id'],blockId=b['id']))
   course['series']['links'].setdefault(b['sourceFile'],dict(topicId=topic['id'],blockId=b['id']))
   if kind=='pre':
    idx=len(course['series']['listings'])+1;language=b['language'];code=b['code'];is_cpp=language in ['cpp','c++','c'] or ('#include' in code and 'int main' in code and language=='text');is_cuda=language in ['cuda','cu'] or '__global__' in code
    language='cuda' if is_cuda else 'cpp' if is_cpp else language
    is_main=bool(re.search(r'\b(?:int|auto)\s+main\s*\(',code))
    suffix={'cpp':'cpp','cuda':'cu','bash':'sh','sh':'sh','python':'py','cmake':'cmake','yaml':'yaml','json':'json','protobuf':'proto'}.get(language,'txt')
    fn=f'cpp_series/book_{number:02}/listings/L{idx:04}.{suffix}';dest=ROOT/fn;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_text(code+'\n' if not code.endswith('\n') else code)
    entry=dict(id=f'B{number:02}-L{idx:04}',topicId=topic['id'],chapter=current,blockId=b['id'],language=language,title=topic['title'] if last_heading in ['main.cpp','Code or command','Complete worked source','Run or inspect'] else last_heading,filename=fn,sourceFile=b['sourceFile'],context=b['context'] or topic['id'],completeCandidate=is_main and language in ['cpp','cuda'],kind='program' if is_main and language in ['cpp','cuda'] else 'excerpt' if language in ['cpp','cuda'] else 'output' if re.search('expected|output|result',last_heading,re.I) and language=='text' else 'commands' if language in ['bash','sh'] else 'configuration' if language in ['cmake','yaml','json'] else 'listing',sha256=hashlib.sha256(dest.read_bytes()).hexdigest(),sourceLabel=last_heading,sourceRef=f'{epub.name} → {b["sourceFile"]} → {last_heading}')
    b['listingId']=entry['id'];b['language']=language;course['series']['listings'].append(entry)
  # Each source block is stored once in the reader. Catalog entries point to it.
  for t in course['topics']:
   paras=[normalized(b['text']) for b in t['blocks'] if b['kind'] in ['p','blockquote'] and normalized(b['text'])]
   t['summary']=paras[0] if paras else 'Open the linked subsections and source listings below.'
   t['reading'].update(paragraphs=paras,preview=t['summary'][:170])
   t['sourceRef']=epub.name+' → '+t['blocks'][0]['sourceFile'] if t['blocks'] else epub.name
   for b in t['blocks']:
    if b['kind']!='p':continue
    rich=b.get('rich',[]);first=next((n for n in rich if isinstance(n,dict) and n['tag'] in ['strong','b']),None)
    if not first:continue
    def flatten(nodes):return ''.join(n if isinstance(n,str) else flatten(n.get('children',[])) for n in nodes)
    q=normalized(flatten(first['children']));alltext=normalized(b['text']);a=alltext[len(q):].strip() if alltext.startswith(q) else ''
    if q.endswith('?') and len(a)>20:
     t['cards'].append(dict(id=t['id']+f'-q{len(t["cards"])+1}',question=q,answer=a))
     t['practice'].append(dict(id=t['id']+f'-p{len(t["practice"])+1}',title='Explain the concept',prompt=q,promptBlocks=[],answer=a,answerBlocks=[],answerLabel='Explained answer from the book',hint='State your answer, then support it with the chapter’s example.',sourceTopicId=t['id']))
    if 'glossary' in t['chapterTitle'].lower() and q and a and len(q)<140:
     course['glossary'].append(dict(id=f'B{number:02}-G{len(course["glossary"])+1}',chapter=t['chapter'],term=q.rstrip(':.'),definition=a))
  # Retain each chapter lab as a self-reviewed practice item, with source answers kept separate.
  for ch in course['chapters']:
   guide=course['teaching'][str(ch['number'])]
   for sec in guide['sections']:
    members=[t for t in course['topics'] if t['id'] in sec['topics']]
    if not members:continue
    sec['purpose']=next((t['summary'][:190] for t in members if not t['summary'].startswith('Open the linked')), 'Follow the book’s subsections in order.')
    if re.search(r'Practice and implementation lab|Guided laboratory|Review exercise',sec['title'],re.I):
     prompt=[];answer=[];is_answer=False;answer_kind='Review guidance from the book'
     for t in members:
      for b in t['blocks']:
       if b['kind'].startswith('h'):
        if re.search(r'solution|answer',b['text'],re.I):is_answer=True;answer_kind='Explained solution notes from the book'
        elif re.search('guided lab|exercise',b['text'],re.I):is_answer=False
       if b['kind']=='p' and re.match(r'\s*(?:Solution reasoning|Review reasoning|Review answers)',b['text'],re.I):is_answer=True;answer_kind='Explained solution notes from the book' if 'Solution' in b['text'] else answer_kind
       if b['kind'].startswith('h') or re.match(r'\s*(Chapter takeaway|Technical references)',b['text']):continue
       (answer if is_answer else prompt).append(b['id'])
     if prompt:
      t=members[0];t['practice'].insert(0,dict(id=t['id']+'-lab',title=ch['title']+' — implementation lab',prompt='Complete the chapter lab, record your prediction, and compare your evidence with the source guidance.',promptBlocks=prompt,answer='',answerBlocks=answer,answerLabel=answer_kind if answer else 'Verification guidance',hint='Use the chapter’s complete example as your starting point. State the acceptance criteria before changing it.',sourceTopicId=t['id']))
   # Chapter recall is useful even where the original offers open review prompts.
   chapter_topics=[t for t in course['topics'] if t['chapter']==ch['number']]
   if ch['role']=='chapter' and not any(t['cards'] for t in chapter_topics):
    for t in chapter_topics:
     if re.match(r'^\d+\.\d+\.\d+',t['title']) and len(t['summary'])>80:
      t['cards'].append(dict(id=t['id']+'-recall',question='Explain '+re.sub(r'^\d+(?:\.\d+)+\s*','',t['title'])+' in your own words.',answer=t['summary']))
  for entry in course['series']['listings']:
   group=context_blocks[entry['context']];expected=[];run=[];explanations=[];mode=''
   for b in group:
    if b['kind'].startswith('h'):
     h=b['text'].lower();mode='expected' if 'expected' in h else 'run' if 'run or inspect' in h else 'explanation' if h=='explanation' else ''
    elif mode=='expected':expected.append(b['id'])
    elif mode=='run':run.append(b['id'])
    elif mode=='explanation':explanations.append(b['id'])
    elif b['kind']=='p' and re.match(r'\s*Expected behavior',b['text'],re.I):expected.append(b['id'])
   entry.update(expectedBlocks=expected,runBlocks=run,explanationBlocks=explanations)
   # A textual output block from a corrected example is an exact fixture; prose is not.
   exact=[b['code'] for b in group if b['id'] in expected and b['kind']=='pre' and b['language']=='text']
   if exact and entry['completeCandidate']:entry['expectedExact']='\n'.join(exact).rstrip('\n')+'\n'
  seen_contexts=set()
  for entry in course['series']['listings']:
   ctx=entry['context']
   if ctx in seen_contexts:continue
   seen_contexts.add(ctx);group=context_blocks[ctx];experiment=[];collect=False
   for b in group:
    if b['kind'].startswith('h'):collect=bool(re.search(r'Concrete experiment|^Experiment',b['text'],re.I));continue
    if collect or (b['kind']=='p' and re.match(r'\s*Experiment[.:]',b['text'],re.I)):experiment.append(b['id'])
   if experiment:
    target=next(t for t in course['topics'] if t['id']==entry['topicId'])
    target['practice'].append(dict(id=entry['id']+'-experiment',title='Experiment: '+target['title'],prompt='Predict what changes, then perform the source experiment.',promptBlocks=experiment,answer='',answerBlocks=entry['explanationBlocks']+entry['expectedBlocks'],answerLabel='Baseline explanation and verification guidance',hint='Read the original program and its stated behavior first. Change only the condition requested, then explain why the result changes.',sourceTopicId=target['id'],listingId=entry['id']))
  if course['glossary']:
   course['topics'].append(dict(id='REF.GLOSSARY',kind='glossary',chapter=-1,chapterTitle='Glossary',domain='REF',title='Book glossary',summary='Vocabulary from the source glossary.',concepts=[],blocks=[],cards=[dict(id=e['id']+'-c',question=e['term']+' — what does it mean?',answer=e['definition']) for e in course['glossary']],origin='cpp-series-epub'))
   course['readingOrder'].append('REF.GLOSSARY')
  # EPUB and PDF remain byte-exact, downloadable offline.
  for kind,path in [('epub',epub),('pdf',pdf)]:course['series']['assets'][kind]=dict(name=path.name,base64=base64.b64encode(path.read_bytes()).decode(),sha256=hashlib.sha256(path.read_bytes()).hexdigest())
  report=dict(book=roman,id=course['id'],title=title,chapters=maxchapter,readingGroups=len(course['chapters']),sections=sum(len(g['sections']) for g in course['teaching'].values()),topics=len(course['topics']),sourceBlocks=len(stream),readerBlocks=sum(len(t['blocks']) for t in course['topics']),sourceTextSHA256=hashlib.sha256('\n'.join(b['text'] for b in stream).encode()).hexdigest(),listings=len(course['series']['listings']),completeCandidates=sum(x['completeCandidate'] for x in course['series']['listings']),cards=sum(len(t['cards']) for t in course['topics']),practice=sum(len(t.get('practice',[])) for t in course['topics']),glossary=len(course['glossary']))
  assert report['sourceBlocks']==report['readerBlocks']
  assert sum(ch['role']=='chapter' for ch in course['chapters'])==maxchapter
  course['series']['counts']=report;reports.append(report)
  course['series']['verification']={k:v for k,v in verification.items() if k!='results'}
  for entry in course['series']['listings']:
   check=checks.get(entry['id'])
   if check and check['sha256']==entry['sha256']:entry['validation']=check
  data['courses'].insert(-1,course)
  (ROOT/f'cpp_series/book_{number:02}/catalog.json').write_text(json.dumps(course['series']['listings'],ensure_ascii=False,indent=2)+'\n')
  print(report)
 (ROOT/'docs/cpp-series-coverage.json').write_text(json.dumps(reports,indent=2)+'\n')
 return data
if __name__=='__main__':
 p=ROOT/'src/content.json';p.write_text(json.dumps(build(json.loads(p.read_text())),ensure_ascii=False,indent=2))
