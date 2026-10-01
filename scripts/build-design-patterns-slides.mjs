// Compile editable lecture authoring and the existing chapter sources into lazy shards.
// Existing study content is read only. No lecture is fetched until Slides opens it.
import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import {instance} from '@viz-js/viz';
const root=path.resolve(import.meta.dirname,'..');
const read=async p=>JSON.parse(await fs.readFile(path.join(root,p),'utf8'));
const write=async(p,text)=>{await fs.mkdir(path.dirname(path.join(root,p)),{recursive:true});await fs.writeFile(path.join(root,p),text);};
const hash=x=>crypto.createHash('sha256').update(x).digest('hex');
const z=n=>String(n).padStart(2,'0');
const first=p=>p.match(/^.*?[.!?](?:\s|$)/)?.[0].trim()||p;
const excerpt=(text,filename,explain=[])=>({text,filename,explain});
const asProgram=(e)=>({filename:path.basename(e.filename),sourcePath:'companion/'+e.filename,language:'cpp',runAllowed:true,text:e.code,expectedPhase:'run',expectedStdout:e.output,expectedStderr:'',expectedExitCode:0});
const blockText=t=>[t.summary,...(t.blocks||[]).flatMap(b=>b.text?[b.text]:b.items||[]),...(t.concepts||[]).map(x=>x.term+': '+x.definition)].filter(Boolean);
function formatCpp(code){
 let out='',indent=0,line='',quote='',escaped=false,parens=0;const blocks=[];
 const flush=()=>{if(line.trim()){out+='  '.repeat(Math.max(0,indent))+line.trim()+'\n';line='';}};
 for(const ch of code){if(quote){line+=ch;if(escaped)escaped=false;else if(ch==='\\')escaped=true;else if(ch===quote)quote='';continue;}
  if(ch==='"'||ch==="'"){quote=ch;line+=ch;continue;}if(ch==='(')parens++;if(ch===')')parens--;
  if(ch==='{'){blocks.push(parens);line+=ch;flush();indent++;}else if(ch==='}'){flush();indent--;blocks.pop();line+=ch;}else if(ch===';'&&parens===(blocks.at(-1)??0)){line+=ch;flush();}else line+=ch;
 }flush();return out.trimEnd();
}
function traces(c,n){
 const source=c.examples[String(n)].solutions,prefix=source.code.split('int main()')[0];
 const helper='// Return true only for the requested exception type.\n#include <utility>\ntemplate<class E, class F> bool dpx_rejects(F&& action) {\n  try { std::forward<F>(action)(); }\n  catch (const E&) { return true; }\n  return false;\n}\n';
 return c.topics.flatMap(t=>t.cards||[]).filter(x=>x.category==='trace'&&x.id.startsWith('dpx-')).map((card,i)=>{
  const end=card.answer.indexOf('. '),literal=card.answer.slice(0,end),why=card.answer.slice(end+2),output=(literal.startsWith('"')?JSON.parse(literal):literal)+'\n';
  const call=formatCpp(`std::cout << std::boolalpha << ${card.code} << '\\n';`);
  return {title:`Trace ${i+1}: ${card.question.split('. What does')[0]}`,eyebrow:'PREDICT · EXECUTE · EXPLAIN',kind:'code',keyPoints:[card.question,'Explain the object state that produces your prediction.'],narration:`${card.question}\n\n${why}\n\n${i===0?"The bracket pair and braces describe a small function, called a lambda. Its final parentheses call it now. std::cout prints the returned value. std::boolalpha prints true or false rather than one or zero, and the quoted backslash-n adds a newline. The full editor includes the chapter's extension types; the focused panel isolates the experiment.":""}${card.code.includes('dpx_rejects')?" The dpx_rejects helper returns true only if this operation throws the named exception type. Returning normally or throwing a different type does not satisfy that rejection check.":""}`,actions:['Ask for the exact result before opening Expected behavior.','Read the focused call, then find the invoked member in the complete editor.','Change one input and explain which contract or branch should change.'],sourceRef:`content/design-patterns-cpp/ch${n}.json → ${card.id}; companion/${source.filename}`,question:{prompt:card.question,answer:card.answer},excerpt:excerpt(call,'Focused call from the complete program',['The expression between << operators computes one result.','The lambda uses the actual extension types copied into this complete source.','The output statement prints that result followed by one newline.']),code:{filename:`dp_ch${z(n)}_trace_${i+1}.cpp`,language:'cpp',runAllowed:true,text:prefix+helper+'\nint main() {\n'+call+'\n}\n',expectedPhase:'run',expectedStdout:output,expectedStderr:'',expectedExitCode:0},relatedLab:card.labId};
 });
}
const viz=await instance();
await write('lectures/diagrams/design-patterns-ch0.svg',viz.renderString(await fs.readFile(path.join(root,'lectures/diagrams/design-patterns-ch0.dot'),'utf8'),{format:'svg'}));
const manifest=await read('lectures/manifest.json');
const keep=manifest.filter(e=>e.courseId!=='design-patterns-cpp'),stats=[],integrity=[];
// These are generated outputs, never authoritative authoring or original companion code.
await fs.rm(path.join(root,'lectures/examples/design-patterns-cpp'),{recursive:true,force:true});
for(let n=0;n<=23;n++){
 const c=await read(`content/design-patterns-cpp/ch${n}.json`),a=await read(`lectures/authoring/design-patterns/ch${n}.json`),slides=[];
 const add=(s,section)=>slides.push({...s,section:s.section||section});
 const authored=(arr,section)=>(arr||[]).forEach(s=>add(s,section));
 const sourceFile=`content/design-patterns-cpp/ch${n}.json`;
 const covered=new Set();
 const topicFor=title=>c.topics.find(t=>t.title===title);
 const mark=(s,t)=>{if(t){s.topicIds=[t.id];covered.add(t.id);}return s;};
 const base={eyebrow:'DESIGN PATTERNS · CHAPTER '+n,kind:'concept',actions:['Ask students for a prediction before revealing the answer.'],sourceRef:sourceFile};
 authored(a.opening,'Problem and first design');
 if(slides.length)slides[0].kind='cover';
 if(n>0){
  const m=await read(`manuscript/ch${z(n)}.json`),e=c.examples[String(n)],d=c.diagrams[String(n)],ref=`manuscript/ch${z(n)}.json`;
  const overview=c.topics[0];covered.add(overview.id);slides[0].topicIds=[overview.id];
  add({...base,title:'The contract we must preserve',keyPoints:m.example.requirements,narration:`${m.example.title}. ${m.example.design}\n\n${m.example.requirements.join(' ')}\n\n${m.example.invariants.join(' ')} These are the rules that define a correct extension, even if it has the right C++ types. ${m.example.alternative}`,comparisons:[{label:'Chosen design',text:m.example.design},{label:'A reasonable alternative',text:m.example.alternative}],question:{prompt:`Which promise would a new ${m.pattern||m.title} implementation be allowed to break?`,answer:`None of the stated requirements. The implementation may change, but these rules remain: ${m.example.invariants.join(' ')}`},sourceRef:ref,topicIds:[topicFor('Worked example: contracts and alternatives')?.id].filter(Boolean)},'Requirements and mechanism');
  covered.add(topicFor('Worked example: contracts and alternatives')?.id);
  for(const [i,s] of m.sections.entries()){
   const t=topicFor(s.heading),discussion=t?.reading?.discussion;
   add(mark({...base,title:s.heading,keyPoints:s.paragraphs.map(first),narration:s.paragraphs.join('\n\n'),sourceRef:ref+' → '+s.heading,excerpt:s.code?excerpt(s.code,s.code_label||'Chapter excerpt — not a complete program'):undefined,question:discussion?{prompt:discussion.question,answer:discussion.answer}:{prompt:`Explain the design choice in “${s.heading}” using this chapter's objects.`,answer:s.paragraphs.at(-1)},reading:s.paragraphs},t),'Requirements and mechanism');
  }
  authored(a.mechanism,'Mechanism in C++');
  const roles=topicFor('Roles in the implementation');
  if(roles){add(mark({...base,title:'Map the roles to the actual types',keyPoints:roles.concepts.map(x=>x.term),narration:roles.concepts.map(x=>x.term+'. '+x.definition).join('\n\n'),comparisons:roles.concepts.map(x=>({label:x.term,text:x.definition})),question:{prompt:roles.cards[0].question,answer:roles.cards[0].answer},sourceRef:sourceFile+' → '+roles.id+' concepts'},roles),'UML and source walkthrough');}
  for(const [i,v] of [d.overview,...(d.extra_overviews||[])].entries()){
   add({...base,title:v.title,eyebrow:`${v.kind.toUpperCase()} DIAGRAM · ACTUAL IMPLEMENTATION`,keyPoints:[`Read the ${v.kind} relationships in the named example.`,...m.example.invariants.slice(0,2)],narration:`${v.explanation}\n\n${m.example.design}\n\n${m.example.maintenance}`,diagram:{chapter:n,view:i===0?'overview':'extra',index:i-1},theory:{what:v.title,where:`companion/${d.source}`,why:m.example.design,invariant:m.example.invariants.join(' '),risk:m.example.failures.map(f=>f.mistake+' '+f.consequence).join(' ')},question:{prompt:'Which arrow or connection carries ownership, and which only describes use or order?',answer:v.explanation},sourceRef:`companion/${d.source}:1-${e.examples.code.trimEnd().split('\n').length}; ${sourceFile} → diagrams.${n}`},'UML and source walkthrough');
  }
  for(const [i,f] of d.focus.entries()){
   add(mark({...base,title:f.title,eyebrow:'FOCUSED SOURCE · LINE BY LINE',keyPoints:[f.what,f.why,f.invariant],narration:`${f.what} ${f.where} ${f.why}\n\n${f.explain.join(' ')}\n\n${f.invariant} ${f.risk}`,diagram:{chapter:n,view:'focus',index:i},theory:f,question:{prompt:'What can go wrong if this block is changed carelessly?',answer:f.risk},sourceRef:`companion/${f.file}:${f.lineStart}-${f.lineEnd}`},topicFor('Code focus: '+f.title)),'UML and source walkthrough');
  }
  add({...base,title:'Run the complete chapter program',eyebrow:'ORIGINAL CHECKED C++',keyPoints:[m.example.title,m.example.verification],narration:`We now join the small pieces into the complete working program. ${m.example.design}\n\n${m.example.verification}\n\n${m.example.maintenance} Download this source when you want to run it locally. Compare the actual output with the expected panel, then change one boundary case and keep the other checks in place.`,code:asProgram(e.examples),question:{prompt:'Does matching the demonstration output prove every promised behavior?',answer:`No. ${m.example.verification} A printed result shows that run; the checks and stated limits explain what was actually verified.`},sourceRef:`companion/${e.examples.filename}`},'Execute and debug');
  const traceTopic=topicFor('Execution trace');
  add(mark({...base,title:'Follow one request through the objects',keyPoints:m.trace.map(t=>t.state),narration:m.trace.map(t=>`Step ${t.step}. ${t.state}. ${t.reason}`).join('\n\n'),walkthrough:m.trace.map(t=>({label:`Step ${t.step}`,text:t.state,reason:t.reason})),question:{prompt:'Which operation makes the next step possible?',answer:m.trace.map(t=>t.state+' — '+t.reason).join('\n')},sourceRef:ref+' → trace'},traceTopic),'Execute and debug');
  for(const [i,f] of m.example.failures.entries())add(mark({...base,title:'Debug the promise: '+f.mistake,eyebrow:'INCORRECT DESIGN · OBSERVABLE CONSEQUENCE',keyPoints:[f.mistake,f.consequence],narration:`${f.mistake} ${f.consequence} ${f.reason}\n\n${m.example.verification}\n\n${m.example.maintenance}`,comparisons:[{label:'What goes wrong',text:f.consequence},{label:'Why it fails',text:f.reason}],question:{prompt:'Where would you look first, and what observation would distinguish this defect?',answer:f.reason+' '+f.consequence},sourceRef:ref+' → example.failures'},i===0?topicFor('Failure cases and repairs'):null),'Execute and debug');
  const cases=traces(c,n);if(cases.length!==8)throw Error(`Chapter ${n} needs all eight Design Patterns expansion traces`);cases.forEach(s=>add(s,'Trace experiments'));
  for(const x of m.exercises){const t=c.topics.find(t=>t.title.startsWith('Exercise '+x.id+':'));add(mark({...base,title:`Exercise ${x.id}: ${x.level}`,eyebrow:'TRY FIRST · THEN REVEAL',keyPoints:[x.prompt],narration:`${x.prompt}\n\nPause here and make a concrete prediction or sketch a change. The hint is: ${x.hint}\n\nAfter the discussion, compare the reasoning. ${x.answer}`,actions:['Give students time to write an answer before opening the hint.','Ask for a reason tied to the contract, then reveal the explained answer.'],hint:x.hint,question:{prompt:'Reveal the explained answer',answer:x.answer},sourceRef:ref+' → exercises.'+x.id},t),'Practice and extension');}
  add(mark({...base,title:'Extend the program without losing its tests',keyPoints:[m.lab.task],narration:`${m.lab.task}\n\nKeep the original checks as a baseline. The new behavior must also satisfy these checks: ${m.lab.checks.join(' ')}\n\nThe starter remains a complete program; running it establishes what was already true before your extension. The next slide contains the reference solution. Try your own change first, then compare design choices as well as output.`,code:asProgram(e.exercises),checks:m.lab.checks.map(s=>({prompt:s,answer:'This is a required acceptance check for the extension.'})),question:{prompt:'What must the extension add?',answer:m.lab.task},sourceRef:`companion/${e.exercises.filename}`},topicFor('Implementation lab')),'Practice and extension');
  add({...base,title:'Extension solution and design review',keyPoints:m.lab.checks,code:asProgram(e.solutions),narration:`${m.lab.answer}\n\n${m.example.maintenance}`,question:{prompt:'Why is this solution a reasonable extension, and could another design work?',answer:m.lab.answer},sourceRef:`companion/${e.solutions.filename}`},'Practice and extension');
  for(const t of c.topics.filter(t=>t.scenario)){const x=t.scenario;add(mark({...base,title:t.title+': defend your choice',keyPoints:[x.prompt],scenario:x,narration:`${x.prompt}\n\n${x.options.map((o,i)=>o+'. '+x.rationales[i]).join('\n\n')}`,actions:['Have learners choose and defend one option.','Reveal the answer and compare every alternative, including the tempting wrong ones.'],question:{prompt:'Reveal the design decision',answer:x.options[x.correctIndex]+'. '+x.explanation},sourceRef:sourceFile+' → '+t.id+' scenario'},t),'Practice and extension');}
  add(mark({...base,title:'Keep these rules when the program grows',keyPoints:m.summary,narration:`${m.summary.join(' ')}\n\n${m.example.maintenance}`,question:{prompt:`Explain ${m.pattern||m.title} without reciting its class names.`,answer:m.example.design+' '+m.example.alternative},sourceRef:ref+' → summary'},topicFor('Chapter summary')),'Recall and transfer');
 }
 authored(a.closing,'Recall and transfer');
 // Extra source lessons remain reachable even if a chapter gains a section later.
 // Their source text is shown in a separate reading panel, never silently dropped.
 for(const t of c.topics.filter(t=>!covered.has(t.id))){
  const p=blockText(t),card=t.cards?.[0],x=t.scenario;add(mark({...base,title:t.title,keyPoints:[t.summary],narration:p.join('\n\n')+(x?'\n\n'+x.options.map((o,i)=>o+'. '+x.rationales[i]).join('\n\n'):''),reading:p,excerpts:(t.blocks||[]).filter(b=>b.type==='code').map(b=>excerpt(b.code,b.title||'Chapter excerpt')),scenario:x,question:x?{prompt:'Reveal the design decision',answer:x.options[x.correctIndex]+'. '+x.explanation}:card?{prompt:card.question,answer:card.answer}:{prompt:'What design promise does this lesson add?',answer:p.join(' ')},sourceRef:sourceFile+' → '+t.id},t),'Chapter reference');
 }
 for(const [i,s] of slides.entries()){
  s.id=`dp-ch${z(n)}-${z(i+1)}`;
  if(s.code?.runAllowed){
   const filename=`lectures/examples/design-patterns-cpp/ch${n}/${s.id}.cpp`;
   s.code.downloadPath=filename;s.code.buildCommand=`g++ -std=c++20 -Wall -Wextra -Wpedantic -pthread ${path.basename(s.code.filename)} -o example\n./example`;
   await write(filename,s.code.text);
  }
 }
 for(const s of slides.filter(x=>x.visual?.sourceSlideTitle)){const codeSlide=slides.find(x=>x.title===s.visual.sourceSlideTitle&&x.code);if(!codeSlide)throw Error('Missing visual code source '+s.title);s.sourceRef=codeSlide.code.downloadPath+':1-'+codeSlide.code.text.trimEnd().split('\n').length;}
 const deck={schemaVersion:1,courseId:'design-patterns-cpp',courseTitle:'Design Patterns in C++',chapter:n,title:a.title,author:'Dr. Charles Dorner',sourceTopics:c.topics.map(t=>({id:t.id,title:t.title})),slides};
 const dest=`lectures/design-patterns-cpp/ch${n}.json`;await write(dest,JSON.stringify(deck,null,2)+'\n');
 keep.push({courseId:deck.courseId,chapter:n,title:deck.title,path:dest});
 const transcript=slides.map((s,i)=>`${i+1}. ${s.title}\n${s.section}\n\nSAY\n${s.narration}\n\nDO\n${s.actions.join('\n')}\n\nSOURCE\n${s.sourceRef}\n`).join('\n'+ '-'.repeat(60)+'\n\n');
 await write(`lectures/transcripts/design-patterns-cpp/ch${n}.txt`,`${deck.courseTitle}\nChapter ${n}: ${deck.title}\nDr. Charles Dorner\n\n${transcript}`);
 stats.push({chapter:n,title:deck.title,slides:slides.length,authoredSlides:(a.opening||[]).length+(a.mechanism||[]).length+(a.closing||[]).length,runnablePrograms:slides.filter(s=>s.code?.runAllowed).length,diagrams:slides.filter(s=>s.diagram||s.visual).length,sourceTopics:c.topics.length,coveredTopics:new Set(slides.flatMap(s=>s.topicIds||[])).size,bytes:Buffer.byteLength(JSON.stringify(deck,null,2)+'\n')});
 for(const f of [sourceFile,...(n?[`manuscript/ch${z(n)}.json`,...['examples','exercises','solutions'].map(k=>`companion/${k}/ch${z(n)}/main.cpp`)]:[])])integrity.push({file:f,sha256:hash(await fs.readFile(path.join(root,f)))});
}
await write('lectures/manifest.json',JSON.stringify(keep,null,2)+'\n');
await write('docs/design-patterns-slides-counts.json',JSON.stringify({chapters:stats,totals:{chapters:stats.length,slides:stats.reduce((a,x)=>a+x.slides,0),runnablePrograms:stats.reduce((a,x)=>a+x.runnablePrograms,0),diagramSlides:stats.reduce((a,x)=>a+x.diagrams,0),sourceTopics:stats.reduce((a,x)=>a+x.sourceTopics,0)}},null,2)+'\n');
await write('lectures/design-patterns-source-integrity.json',JSON.stringify(integrity,null,2)+'\n');
console.log(`Built ${stats.length} Design Patterns lectures from source, with ${stats.reduce((a,x)=>a+x.slides,0)} slides.`);
