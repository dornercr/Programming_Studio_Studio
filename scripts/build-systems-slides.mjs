// Systems lectures are chapter shards. Authoring is additive; all existing
// source lessons remain available verbatim in the expandable source panels.
import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
const root=path.resolve(import.meta.dirname,'..'),z=n=>String(n).padStart(2,'0');
const read=async p=>JSON.parse(await fs.readFile(path.join(root,p),'utf8'));
const write=async(p,t)=>{await fs.mkdir(path.dirname(path.join(root,p)),{recursive:true});await fs.writeFile(path.join(root,p),t);};
const hash=b=>crypto.createHash('sha256').update(b).digest('hex');
const unique=a=>[...new Set(a.filter(Boolean))];
function paragraphs(t){return t.reading?.paragraphs?.length?t.reading.paragraphs:[t.summary].filter(Boolean);}
function points(ts){
 const list=ts.flatMap(t=>(t.blocks||[]).filter(b=>b.type==='list').flatMap(b=>b.items||[]));
 const titles=new Set(ts.map(t=>t.title));
 const clean=unique(list.filter(x=>!titles.has(x)&&x.length>18&&!/^(Source:|\d+\s*\/|CHAPTER \d+|SOURCE TOPIC|GRADUATE LECTURE)/.test(x)));
 return clean.length?clean.slice(0,4):unique(ts.flatMap(paragraphs)).slice(0,3);
}
function sourceSlide(ts,chapter){
 const t=ts[0],p=unique(ts.flatMap(paragraphs)),correction=unique(ts.map(x=>x.reading?.correction)).join('\n\n');
 const discussion=t.reading?.discussion,card=t.cards?.find(c=>!c.code),scenario=t.scenario;
 const reveals=(t.blocks||[]).filter(b=>b.type==='reveal'),answer=reveals.find(b=>/answer|solution works/i.test(b.title))?.text;
 const hint=discussion?.hint||reveals.find(b=>/hint/i.test(b.title))?.text;
 const plain=unique(ts.map(x=>x.reading?.plain));
 const question=scenario?{prompt:'Reveal and defend the design decision',answer:scenario.options[scenario.correctIndex]+'. '+scenario.explanation}:discussion?{prompt:discussion.question,answer:discussion.answer}:answer?{prompt:'Reveal the explained answer',answer}:card?{prompt:card.question,answer:card.answer}:{prompt:`Explain “${t.reading?.title||t.title}” using a concrete state or operation.`,answer:p.join('\n\n')};
 const s={title:t.reading?.title||t.title,eyebrow:t.slide?'SOURCE CONCEPT · REASON THROUGH THE MECHANISM':'WORKSHOP · CONTRACTS AND EVIDENCE',kind:'concept',keyPoints:correction?[correction]:scenario?[scenario.prompt]:points(ts),narration:correction?`${correction}\n\nThe retained source panel includes the original interleaving description. Use it only as a conceptual model; it does not bound the behavior of an unsynchronized ordinary C++ counter. The chapter's atomic demonstration gives a defined experiment for the logical lost update.`:unique([...p,...plain.map(x=>'In plain English: '+x)]).join('\n\n'),actions:scenario?['Ask students to choose an option and name the requirement that decides it.','Reveal the rationale for each alternative after discussion.']:answer?['Allow an independent attempt before opening the hint.','Reveal the explanation and compare the proposed change with the acceptance checks.']:['Ask learners to identify the state, operation, and guarantee named on this page.','Use the source panel to inspect the complete original lesson and its exact wording.'],sourceRef:`content/systems-programming/ch${chapter}.json → ${ts.map(t=>t.id).join(', ')}`,topicIds:ts.map(t=>t.id),sourceLessons:ts,question,hint,scenario,correction:correction||undefined};
 if(!answer&&!scenario)s.reading=p;
 if(t.concepts?.length)s.comparisons=t.concepts.map(x=>({label:x.term,text:x.definition}));
 return s;
}
function groupOriginal(topics){
 const groups=[],byId=new Map();
 for(const t of topics){const parent=t.outlineParent&&byId.get(t.outlineParent);if(parent){parent.push(t);byId.set(t.id,parent);continue;}
  const previous=groups.at(-1),same=previous&&JSON.stringify(paragraphs(previous[0]))===JSON.stringify(paragraphs(t));
  if(same){previous.push(t);byId.set(t.id,previous);}else{const group=[t];groups.push(group);byId.set(t.id,group);}
 }return groups;
}
const program=e=>({filename:path.basename(e.filename),sourcePath:e.filename,language:'cpp',runAllowed:true,text:e.code,expectedPhase:'run',expectedStdout:e.output,expectedStderr:'',expectedExitCode:0,platform:'C++20; see chapter assumptions',buildCommand:`g++ -std=c++20 -Wall -Wextra -Wpedantic -pthread ${path.basename(e.filename)} -o example\n./example`});
const current=await read('lectures/manifest.json'),manifest=current.filter(e=>e.courseId!=='systems-programming'),counts=[],integrity=[];
await fs.rm(path.join(root,'lectures/examples/systems-programming'),{recursive:true,force:true});
for(let n=1;n<=61;n++){
 const file=`content/systems-programming/ch${n}.json`,c=await read(file),a=await read(`lectures/authoring/systems-programming/ch${n}.json`),e=c.examples[String(n)],g=c.teaching[String(n)],w=g.workshop,d=c.diagrams[String(n)],slides=[];
 const add=(s,section)=>slides.push({...s,section:s.section||section});
 const authored=(list,section)=>list.forEach(s=>add(s,section));
 const source=t=>sourceSlide([t],n),sys=suffix=>c.topics.find(t=>t.id===`SYS${z(n)}.${z(suffix)}`);
 authored(a.opening,'Problem and first experiment');slides[0].kind='cover';
 for(const i of [1,2,3])add(source(sys(i)),'The chapter problem');
 for(const ts of groupOriginal(c.topics.filter(t=>t.slide)))add(sourceSlide(ts,n),'Systems mechanisms and source lecture');
 authored(a.mechanism,'Reason about the implementation');
 const full=source(sys(4));full.title='Run the complete chapter demonstration';full.code=program(e.examples);full.reading=undefined;full.keyPoints=[w.title,w.problem];full.narration=[w.problem,...w.steps.map(x=>`${x.action}. ${x.state}. ${x.why}`),w.verification].join('\n\n');full.sourceRef=e.examples.filename;add(full,'Code, diagrams, and debugging');
 add({title:'Trace the observed output',eyebrow:'DATA AND CONTROL · ONE STEP AT A TIME',kind:'concept',keyPoints:[w.title,...w.steps.map(x=>x.action)],narration:w.steps.map(x=>`${x.action}. ${x.state}. ${x.why}`).join('\n\n'),actions:['Before each step, predict the next state and observable text.','Separate what the demonstration verifies from what depends on a real platform.'],walkthrough:w.steps.map(x=>({label:x.action,text:x.state,reason:x.why})),sourceRef:e.examples.filename,question:{prompt:w.discussion.question,answer:w.discussion.answer}},'Code, diagrams, and debugging');
 for(const [i,v] of [d.overview,...(d.extra_overviews||[])].entries())add({title:v.title,eyebrow:`${v.kind.toUpperCase()} DIAGRAM · ACTUAL CHAPTER PROGRAM`,kind:'concept',keyPoints:[w.title,...w.invariants],narration:[v.explanation,w.design,w.maintenance].join('\n\n'),actions:['Follow the arrow labels and name the state changed by each block.','Match the C++ in the blocks to the complete chapter program.'],diagram:{chapter:n,view:i?'extra':'overview',index:i-1},theory:{what:w.title,where:e.examples.filename,why:w.design,invariant:w.invariants.join(' '),risk:w.pitfall},sourceRef:`${e.examples.filename}:1-${e.examples.code.trimEnd().split('\n').length}`,question:{prompt:'Do these arrows show execution, ownership, or inheritance?',answer:v.explanation}},'Code, diagrams, and debugging');
 for(const [i,f] of d.focus.entries()){
  const s=i===0?source(sys(5)):{kind:'concept',actions:['Explain each source line against the state it changes.']};
  Object.assign(s,{title:f.title,eyebrow:'FOCUSED C++ · LINE BY LINE',keyPoints:[f.what,f.invariant],narration:[f.what,f.where,f.why,...f.explain,f.invariant,f.risk].join('\n\n'),diagram:{chapter:n,view:'focus',index:i},theory:f,reading:undefined,sourceRef:`${f.file}:${f.lineStart}-${f.lineEnd}`,question:{prompt:'Which rule does this block preserve, and what breaks if it is bypassed?',answer:f.invariant+' '+f.risk}});add(s,'Code, diagrams, and debugging');
 }
 const failure=source(sys(6));failure.keyPoints=[w.pitfall];add(failure,'Code, diagrams, and debugging');
 for(let i=7;i<=12;i++)add(source(sys(i)),'Practice and extension');
 const starter=source(sys(13));starter.title='Implement the extension and preserve the checks';starter.code=program(e.exercises);starter.reading=undefined;starter.keyPoints=[e.lab.task];starter.sourceRef=e.exercises.filename;add(starter,'Practice and extension');
 add({title:'Review the extension solution',eyebrow:'COMPLETE C++ · REFERENCE SOLUTION',kind:'code',keyPoints:e.lab.checks,narration:[e.lab.answer,w.verification,w.maintenance].join('\n\n'),actions:['Compare your result with the exact expected output.','Review a boundary test and a failure path before accepting the design.'],code:program(e.solutions),question:{prompt:'Why does the extension preserve the original behavior?',answer:e.lab.answer},sourceRef:e.solutions.filename},'Practice and extension');
 for(const t of c.topics.filter(t=>!t.slide&&!t.id.startsWith('SYS')))add(source(t),'Practice and extension');
 add(source(sys(14)),'Recall and transfer');authored(a.closing,'Recall and transfer');
 for(const [i,s] of slides.entries()){
  // Code drafts use this ID: adding an earlier page must not attach an old
  // draft to a different program. Source identity wins over display order.
  const identity=s.code?.runAllowed?'code:'+String(s.code.sourcePath||s.code.filename):s.topicIds?.length?'lesson:'+s.topicIds[0]:s.diagram?'diagram:'+s.diagram.view+':'+s.diagram.index:'page:'+s.title;
  s.id=`sys-ch${z(n)}-${identity.startsWith('code:')?'code':identity.startsWith('lesson:')?'lesson':'page'}-${hash(identity).slice(0,16)}`;
  if(s.code?.runAllowed){s.code.downloadPath=`lectures/examples/systems-programming/ch${n}/${s.id}.cpp`;s.code.buildCommand||=`g++ -std=c++20 -Wall -Wextra -Wpedantic -pthread ${s.code.filename} -o example\n./example`;await write(s.code.downloadPath,s.code.text);}
 }
 const deck={schemaVersion:1,courseId:'systems-programming',courseTitle:'Systems Programming and Machine Organization',chapter:n,title:a.title,author:'Dr. Charles Dorner',sourceTopics:c.topics.map(t=>({id:t.id,title:t.title})),slides};
 const dest=`lectures/systems-programming/ch${n}.json`;await write(dest,JSON.stringify(deck,null,2)+'\n');manifest.push({courseId:deck.courseId,chapter:n,title:deck.title,path:dest});
 const narration=slides.map((s,i)=>`${i+1}. ${s.title}\n${s.section}\n\nSAY\n${s.narration}\n\nDO\n${s.actions.join('\n')}\n\nSOURCE\n${s.sourceRef}\n`).join('\n'+'-'.repeat(60)+'\n\n');
 await write(`lectures/transcripts/systems-programming/ch${n}.txt`,`${deck.courseTitle}\nChapter ${n}: ${deck.title}\nDr. Charles Dorner\n\n${narration}`);
 counts.push({chapter:n,title:a.title,slides:slides.length,authoredSlides:a.opening.length+a.mechanism.length+a.closing.length,runnablePrograms:slides.filter(s=>s.code?.runAllowed).length,diagramViews:slides.filter(s=>s.diagram).length,originalSlides:c.topics.filter(t=>t.slide).length,originalConceptPages:slides.filter(s=>s.sourceLessons?.some(t=>t.slide)).length,sourceTopics:c.topics.length,coveredTopics:new Set(slides.flatMap(s=>s.topicIds||[])).size,bytes:Buffer.byteLength(JSON.stringify(deck,null,2)+'\n')});
 for(const f of [file,e.examples.filename,e.exercises.filename,e.solutions.filename,`systems_source/Chapter_${z(n)}/${c.lectures[String(n)].presentationName}`,`systems_source/Chapter_${z(n)}/${c.lectures[String(n)].transcriptName}`])integrity.push({file:f,sha256:hash(await fs.readFile(path.join(root,f)))});
}
await write('lectures/manifest.json',JSON.stringify(manifest,null,2)+'\n');
await write('docs/systems-slides-counts.json',JSON.stringify({chapters:counts,totals:Object.fromEntries(['slides','authoredSlides','runnablePrograms','diagramViews','originalSlides','originalConceptPages','sourceTopics','coveredTopics'].map(k=>[k,counts.reduce((v,c)=>v+c[k],0)]))},null,2)+'\n');
await write('lectures/systems-source-integrity.json',JSON.stringify(integrity,null,2)+'\n');
console.log(`Built ${counts.length} Systems lectures with ${counts.reduce((v,c)=>v+c.slides,0)} interactive pages; every original topic retained.`);
