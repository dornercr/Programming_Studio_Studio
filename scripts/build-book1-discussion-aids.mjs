import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import {instance} from '@viz-js/viz';

const root = path.resolve(import.meta.dirname, '..');
const read = file => fs.readFile(path.join(root,file),'utf8');
const json = async file => JSON.parse(await read(file));
const hash = text => crypto.createHash('sha256').update(text).digest('hex');
const owned = s => s.id.startsWith('b01-discussion-program-');
export const needsBookOneAid = s => !owned(s) && !(s.code || s.diagram || s.visual || s.stages?.length || s.sequence?.length || s.walkthrough?.length);

const glossaryChapters = {
  'Objects and lifetime':18, 'Names and program structure':9, 'Access and indirection':14,
  'Containers and ranges':12, 'Contracts and evidence':20, 'Design discipline':17,
  Aggregate:16, Assignment:13, Borrow:13, Capacity:12, 'Const correctness':13,
  Constructor:18, 'Dangling access':14, Declaration:8, Definition:8, Destructor:18
};
const referenceChapters = {ARRAY:12, CLANG:2, EXPR:5, GCC:2, IO:3, MSVCINSTALL:2, NUMERIC:4, STRING:11};

function contextChapter(chapter,title) {
  if (chapter === 0) return 1;
  if (chapter === 25) return glossaryChapters[title] || 1;
  if (chapter === 26) {
    if (/destructor/i.test(title)) return 18;
    if (/sanitizer/i.test(title)) return 14;
    return referenceChapters[title.split(/\s|:/)[0]] || 2;
  }
  return chapter;
}

function selectListing(source,slide,chapter) {
  const candidates = source.listings.filter(l => l.language === 'cpp');
  if (chapter === 1) {
    // Keep the original 26-slide chapter intact; use its two existing complete demonstrations.
    return candidates.find(l => l.id === (/sensor|data|container|value|sort/i.test(slide.title) ? 'B01-L0003' : 'B01-L0010'));
  }
  const topic = source.topics.find(t => t.title === slide.title || (slide.title.length > 30 && t.title.startsWith(slide.title.replace(/\.$/,''))));
  const exact = topic && candidates.find(l => l.topicId === topic.id);
  if (exact) return exact;
  const group = topic && source.teaching?.[chapter]?.sections?.find(s=>s.topics.includes(topic.id));
  if (group) {
    const matches=candidates.filter(l=>group.topics.includes(l.topicId));
    if(matches.length)return matches.find(l=>l.validation?.status==='ran')||matches[0];
  }
  const section = slide.title.match(/^\d+\.\d+(?:\.\d+)?/)?.[0];
  if (section) {
    const matches = candidates.filter(l => source.topics.find(t=>t.id===l.topicId)?.title.startsWith(section));
    if (matches.length) return matches.find(l=>l.validation?.status==='ran') || matches[0];
  }
  return null;
}

async function referenceGraph(viz,chapter) {
  const models = {
    0: {title:'From source to observed behavior', caption:'A compiler translates source into object code; the linker combines object code and required library definitions. Only the executable runs. Compare its output and process status with the requirement; a successful build alone does not prove correct behavior.',
      nodes:['C++ source','Compile to object code','Link executable','Run with input','Check output and status'], edges:[[0,1,'translate'],[1,2,'resolve definitions'],[2,3,'execute'],[3,4,'compare with requirement']]},
    25: {title:'Names, objects, and borrowing', caption:'A name can identify an object directly or through an alias. A reference or pointer does not extend the lifetime of the object it designates. A const access path restricts mutation through that path; it does not make every other access path const.',
      nodes:['Owning scope or container','Live object','Direct object name','Reference alias','Pointer value'], edges:[[0,1,'controls lifetime'],[2,1,'identifies'],[3,1,'aliases, does not own'],[4,1,'may designate, does not own']]},
    26: {title:'Use a reference to settle a concrete question', caption:'Language rules and library contracts explain what is permitted. Compiler diagnostics and execution provide evidence about one build. A successful test run is not a substitute for checking the relevant lifetime, bounds, or precondition rule.',
      nodes:['Concrete C++ question','Language or library contract','Compiler options and diagnostics','Small focused program','Observation and limits'], edges:[[0,1,'find the applicable rule'],[0,2,'record active toolchain'],[1,3,'construct a valid experiment'],[2,3,'build under stated options'],[3,4,'run and interpret']]}
  };
  const model = models[chapter];
  const dot = `digraph reference { graph [rankdir=TB,bgcolor="white",pad="0.2",ranksep="0.5"]; node [shape=box,style="rounded,filled",fontname="Arial",fontsize=13,fillcolor="#f2f6f5",color="#42695b",margin="0.15,0.12"]; edge [fontname="Arial",fontsize=10,color="#52616b"]; ${model.nodes.map((label,i)=>`n${i} [label=${JSON.stringify(label)}];`).join('\n')} ${model.edges.map(([a,b,label])=>`n${a} -> n${b} [label=${JSON.stringify(label)}];`).join('\n')} }`;
  return {title:model.title,caption:model.caption,image:'data:image/svg+xml;base64,'+Buffer.from(viz.renderString(dot,{format:'svg'})).toString('base64')};
}

export async function buildBookOneDiscussionAids() {
  const entries = (await json('lectures/manifest.json')).filter(e=>e.courseId==='cpp-book-01');
  if (entries.length!==27 || new Set(entries.map(e=>Number(e.chapter))).size!==27)
    throw Error('Install the authored Book I slides for chapters 0-26 before adding discussion aids.');
  const sources = new Map();
  for (let n=0;n<27;n++) sources.set(n,await json(`content/cpp-book-01/ch${n}.json`));
  const questions = (await json('content/book-one-expansion.json')).questions;
  const workshops=(await json('content/cpp-book-01/coding.json')).workedPrograms;
  const viz = await instance(), pending=[], downloads=new Map();
  const report={version:1,chapters:[],totals:{pages:0,addedPrograms:0}};
  for (const entry of entries) {
    const chapter=Number(entry.chapter);
    if (!Number.isInteger(chapter)||chapter<0||chapter>26||entry.path!==`lectures/cpp-book-01/ch${chapter}.json`) throw Error('Invalid Book I lecture entry.');
    const deck=await json(entry.path);
    if (deck.courseId!==entry.courseId||Number(deck.chapter)!==chapter||!deck.slides?.length) throw Error('Invalid Book I deck.');
    deck.slides=deck.slides.filter(s=>!owned(s));
    const originalSlides=[...deck.slides], examples={}, graphs={}, added=[];
    if ([0,25,26].includes(chapter)) graphs.reference=await referenceGraph(viz,chapter);
    let pages=0;
    for (const slide of originalSlides) {
      delete slide.bookOneDiscussionAid;
      delete slide.bookOneDiagram;
      if(slide.diagram?.image && !slide.diagram.chapter){
        const asset=slide.diagram.image.$asset,key='embedded-'+slide.id;
        if(!/^assets\/[a-f0-9]+\.svg$/.test(asset))throw Error('Invalid embedded Book I diagram.');
        graphs[key]={title:slide.diagram.title,caption:slide.diagram.explanation,
          image:'data:image/svg+xml;base64,'+Buffer.from(await read('content/'+asset)).toString('base64')};
        slide.bookOneDiagram=key;
      }
      if (!needsBookOneAid(slide)) continue;
      const context=contextChapter(chapter,slide.title), source=sources.get(context), bundle=source.diagrams?.[context];
      const graphKey=graphs.reference?'reference':String(context);
      if (!graphs[graphKey]) {
        const asset=bundle.overview.image.$asset;
        if (!/^assets\/[a-f0-9]+\.svg$/.test(asset)) throw Error('Invalid diagram asset.');
        graphs[graphKey]={title:bundle.overview.title,caption:bundle.overview.explanation,
          image:'data:image/svg+xml;base64,'+Buffer.from(await read('content/'+asset)).toString('base64')};
      }
      const listing=selectListing(source,slide,context);
      // Section-specific source comes first; the chapter's checked operation is the fallback.
      const focus=!listing && bundle.focus[context===1?0:bundle.focus.length-1];
      const file=listing?.filename||focus.file;
      if (!/^(cpp_series\/book_01\/(listings|projects)\/|coding_lab\/book_one_expansion\/ch\d+\/)[a-zA-Z0-9_./-]+\.cpp$/.test(file)||file.includes('..')) throw Error('Invalid C++ source path.');
      const fullCode=await read(file), key=hash(file).slice(0,16);
      if (listing?.sha256 && hash(fullCode)!==listing.sha256) throw Error(`Original listing hash mismatch: ${file}`);
      if (!examples[key]) {
        let lineStart=focus?.lineStart||1, lineEnd=focus?.lineEnd||Math.min(fullCode.trimEnd().split('\n').length,22);
        const lines=fullCode.split('\n');
        if (!focus && lines.length>24) {const main=lines.findIndex(l=>/\bmain\s*\(/.test(l)); if(main>=0){lineStart=main+1;lineEnd=Math.min(lines.length,main+18);}}
        const text=lines.slice(lineStart-1,lineEnd).join('\n');
        if (focus && text!==focus.code) throw Error(`Focused source mismatch: ${file}`);
        const question=focus?.expansionId && questions.find(q=>q.id===focus.expansionId);
        if (question && fullCode!==question.solution+question.driver && fullCode!==question.solution+'\n'+question.driver)
          throw Error(`Driver/source mismatch: ${file}`);
        const workshop=listing?.validation?.project&&workshops.find(w=>w.sourceFilename===file&&w.adapted);
        if(listing?.validation?.project&&(!workshop||workshop.originalSource.trim()!==fullCode.trim()))throw Error(`Missing verified project adaptation: ${file}`);
        const editorCode=workshop?.source||fullCode;
        let fullSlide=deck.slides.find(s=>s.code?.runAllowed && s.code.text.trim()===editorCode.trim());
        const runnable=!!question||listing?.validation?.status==='ran';
        const downloadPath=`lectures/examples/book1-discussion/ch${chapter}/${key}.cpp`;
        downloads.set(downloadPath,fullCode);
        const editorPath=workshop?downloadPath.replace('.cpp','-standalone.cpp'):downloadPath;
        if(workshop)downloads.set(editorPath,editorCode);
        if (!fullSlide && runnable) {
          const id=`b01-discussion-program-ch${chapter}-${key}`;
          fullSlide={id,title:listing?.title||focus.title,eyebrow:'BOOK I / COMPLETE SOURCE EXAMPLE',kind:'code',
            keyPoints:[question?.prompt||'Run this complete original listing and trace its actual behavior.'],
            narration:workshop?.adaptationNote||question?.explanation||listing.validation.detail||'Compare the source with the observed result. Platform-dependent output is an observation, not a universal constant.',
            actions:['Predict the output before running.','Change one input or expression and explain the result.'],sourceRef:file,
            question:{prompt:'Which line establishes the promised result?',answer:focus?.invariant||'Trace the original source and check its recorded behavior under the stated toolchain.'},
            code:{filename:path.basename(editorPath),language:'cpp',runAllowed:true,text:editorCode,input:question?.sampleInput||'',
              expectedPhase:'run',expectedStdout:question?.sampleOutput??listing.validation.stdout,expectedStderr:listing?.validation?.stderr||'',
              expectedExitCode:listing?.validation?.exitCode??0,downloadPath:editorPath,
              recordedObservation:!question,buildCommand:`g++ -std=c++20 -Wall -Wextra -Wpedantic -pthread ${path.basename(editorPath)} -o example\n./example`}};
          // Chapter 1's historical 26-slide contract must not change.
          if(chapter!==1){added.push(fullSlide);deck.slides.push(fullSlide);}else fullSlide=null;
        }
        examples[key]={title:listing?.title||focus.title,filename:file,lineStart,lineEnd,text,explain:focus?.explain||[],
          invariant:focus?.invariant||'This is an exact excerpt of the delivered listing, not a substitute standalone program.',
          sourceHash:hash(fullCode),downloadPath,fullSlideId:fullSlide?.id||null,sourceText:fullSlide&&!workshop?undefined:fullCode,
          adaptationNote:workshop?.adaptationNote,editorSourceHash:hash(editorCode),
          sampleInput:question?.sampleInput||'',output:question?.sampleOutput??listing?.validation?.stdout??null,
          recordedObservation:!question};
      }
      slide.bookOneDiscussionAid={graph:graphKey,example:key}; pages++;
    }
    for(const slide of originalSlides){
      delete slide.bookOneProjectExample;
      if(!slide.code)continue;
      const workshop=workshops.find(w=>w.adapted&&w.originalSource.trim()===slide.code.text.trim());
      if(workshop){const match=Object.entries(examples).find(([,e])=>e.filename===workshop.sourceFilename);if(match)slide.bookOneProjectExample=match[0];}
    }
    deck.bookOneDiscussionAids={graphs,examples};
    pending.push({file:entry.path,text:JSON.stringify(deck,null,2)+'\n'});
    report.chapters.push({chapter,pages,examples:Object.keys(examples).length,addedPrograms:added.length});
    report.totals.pages+=pages;report.totals.addedPrograms+=added.length;
  }
  for(const [file,text] of downloads){await fs.mkdir(path.dirname(path.join(root,file)),{recursive:true});await fs.writeFile(path.join(root,file),text);}
  for(const item of pending)await fs.writeFile(path.join(root,item.file),item.text);
  await fs.writeFile(path.join(root,'docs/book1-discussion-aids-coverage.json'),JSON.stringify(report,null,2)+'\n');
  console.log(`Added source examples and diagrams to ${report.totals.pages} Book I discussion pages across all 27 decks (${report.totals.addedPrograms} additional complete program slides).`);
  return report;
}

await buildBookOneDiscussionAids();
