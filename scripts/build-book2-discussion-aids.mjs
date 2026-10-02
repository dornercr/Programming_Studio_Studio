import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import {instance} from '@viz-js/viz';
import {bookTwoModels} from './book2-discussion-models.mjs';

const root=path.resolve(import.meta.dirname,'..');
const read=file=>fs.readFile(path.join(root,file),'utf8');
const json=async file=>JSON.parse(await read(file));
const hash=text=>crypto.createHash('sha256').update(text).digest('hex');
const owned=s=>s.id.startsWith('b02-discussion-program-');
export const needsBookTwoAid=s=>!owned(s)&&!(s.code||s.diagram||s.visual||s.stages?.length||s.sequence?.length||s.walkthrough?.length);

function contextChapter(chapter,title){
  if(chapter===0)return 1;
  if(chapter===22){
    if(/ownership|lifetime|resource/i.test(title))return 6;
    if(/generic|template|constraint/i.test(title))return 13;
    if(/range|iterator/i.test(title))return 18;
    return 2;
  }
  if(chapter===27){
    if(/interface|guarantee/i.test(title))return 2;
    if(/traversal|algorithm|result/i.test(title))return 19;
    if(/external|evidence/i.test(title))return 20;
    return 6;
  }
  if(chapter===28){
    const mappings=[[/associative|array|vector modifiers/i,16],[/time library|file.streams|numeric.limits/i,20],
      [/template|constraint/i,13],[/copy|move/i,5],[/construction|destruction|exception/i,3],
      [/iterator/i,15],[/sequence operations/i,17],[/ostream/i,7],[/range/i,18],
      [/shared.ownership|smartptr/i,6],[/tuple|utility/i,19],[/resource/i,3]];
    return mappings.find(([re])=>re.test(title))?.[1]||1;
  }
  return chapter;
}

function selectListing(source,slide){
  const candidates=source.listings.filter(l=>l.language==='cpp');
  const title=slide.title.replace(/^Explain /,'').replace(/ in your own words\.$/,'').replace(/\.$/,'');
  const topic=source.topics.find(t=>t.title===title||(title.length>30&&t.title.startsWith(title)));
  const exact=topic&&candidates.find(l=>l.topicId===topic.id);
  if(exact)return exact;
  const groups=Object.values(source.teaching||{}).flatMap(t=>t.sections||[]);
  const group=topic&&groups.find(s=>s.topics.includes(topic.id));
  if(group){const matches=candidates.filter(l=>group.topics.includes(l.topicId));if(matches.length)return matches.find(l=>l.validation)||matches[0];}
  const section=title.match(/^(?:\d+|[A-F])\.\d+/)?.[0];
  if(section){const g=groups.find(g=>g.title.startsWith(section));const matches=g&&candidates.filter(l=>g.topics.includes(l.topicId));if(matches?.length)return matches.find(l=>l.validation)||matches[0];}
  // Worked/failure/practice discussions use the chapter's complete contract lab;
  // opening explanations use the first source-backed demonstration.
  if(/worked|failure|evidence|requirement|guided|practice|complete|commit/i.test(title))
    return candidates.find(l=>l.title==='Complete implementation'&&l.validation)||candidates.find(l=>/tests|audit|pipeline/i.test(l.title)&&l.validation)||candidates[0];
  return candidates[0];
}

function graph(viz,chapter){
  const m=bookTwoModels[chapter];
  if(!m)throw Error('Missing Book II chapter model.');
  const wrap=(text,width)=>{const rows=[''];for(const word of text.split(' ')){const i=rows.length-1;if(rows[i]&&rows[i].length+word.length+1>width)rows.push(word);else rows[i]+=(rows[i]?' ':'')+word;}return rows.join('\n');};
  // Three ranks with at most two boxes per rank keep labels readable in the
  // right pane. Invisible ordering edges control layout, not the explanation.
  const layout='{rank=min;n0;}{rank=same;n1;n2;}{rank=same;n3;n4;}n0->n1[style=invis,weight=100];n1->n3[style=invis,weight=100];';
  const dot=`digraph discussion {graph [rankdir=TB,bgcolor="white",pad="0.2",ranksep="0.65",nodesep="0.4"];node [shape=box,style="rounded,filled",fontname="Arial",fontsize=16,fillcolor="#f2f6f5",color="#42695b",margin="0.15,0.12"];edge [fontname="Arial",fontsize=12,color="#52616b"];${m.nodes.map((label,i)=>`n${i} [label=${JSON.stringify(wrap(label,20))}];`).join('\n')}${layout}${m.edges.map(([a,b,label])=>`n${a}->n${b} [label=${JSON.stringify(wrap(label,22))}];`).join('\n')}}`;
  return {title:m.title,caption:m.caption,image:'data:image/svg+xml;base64,'+Buffer.from(viz.renderString(dot,{format:'svg'})).toString('base64')};
}

export async function buildBookTwoDiscussionAids(){
  const entries=(await json('lectures/manifest.json')).filter(e=>e.courseId==='cpp-book-02');
  const chapters=entries.map(e=>Number(e.chapter)).sort((a,b)=>a-b);
  if(JSON.stringify(chapters)!==JSON.stringify(Array.from({length:29},(_,i)=>i)))
    throw Error('Install the authored Book II lectures for chapters 0-28 first.');
  const sources=new Map();for(let n=0;n<29;n++)sources.set(n,await json(`content/cpp-book-02/ch${n}.json`));
  const workshops=(await json('content/cpp-book-02/coding.json')).workedPrograms;
  const viz=await instance(),pending=[],downloads=new Map();
  const report={version:1,chapters:[],totals:{pages:0,addedPrograms:0}};
  for(const entry of entries){
    const chapter=Number(entry.chapter);
    if(entry.path!==`lectures/cpp-book-02/ch${chapter}.json`)throw Error('Invalid Book II lecture path.');
    const deck=await json(entry.path);
    if(deck.courseId!==entry.courseId||Number(deck.chapter)!==chapter||!deck.slides?.length)throw Error('Invalid Book II deck.');
    deck.slides=deck.slides.filter(s=>!owned(s));
    const originals=[...deck.slides],examples={},graphs={chapter:graph(viz,chapter)};
    let pages=0,addedPrograms=0;
    async function example(listing){
      if(!listing)return null;
      const file=listing.filename,key=hash(file).slice(0,16);
      if(examples[key])return key;
      if(!/^cpp_series\/book_02\/listings\/L\d+\.cpp$/.test(file))throw Error('Invalid Book II C++ source path.');
      const fullCode=await read(file);
      if(listing.sha256&&hash(fullCode)!==listing.sha256)throw Error(`Original listing hash mismatch: ${file}`);
      const workshop=workshops.find(w=>w.sourceFilename===file);
      if(workshop&&workshop.originalSource.trim()!==fullCode.trim())throw Error(`Workshop/source mismatch: ${file}`);
      const editorCode=workshop?.source||fullCode;
      const lines=fullCode.trimEnd().split('\n');
      let lineStart=1,lineEnd=Math.min(lines.length,22);
      if(lines.length>24){const main=lines.findIndex(l=>/\bmain\s*\(/.test(l));if(main>=0){lineStart=main+1;lineEnd=Math.min(lines.length,main+18);}}
      const text=lines.slice(lineStart-1,lineEnd).join('\n');
      const downloadPath=`lectures/examples/book2-discussion/ch${chapter}/${key}.cpp`;
      downloads.set(downloadPath,fullCode);
      const editorPath=workshop?.adapted?downloadPath.replace('.cpp','-standalone.cpp'):downloadPath;
      if(workshop?.adapted)downloads.set(editorPath,editorCode);
      let fullSlide=deck.slides.find(s=>s.code?.runAllowed&&s.code.text.trim()===editorCode.trim());
      const runnable=workshop?.sourceKind==='program'||listing.validation?.status==='ran';
      const check=workshop?.checks?.[0];
      const output=workshop?.sampleOutput??listing.validation?.stdout??null;
      const recordedObservation=!workshop;
      if(!fullSlide&&runnable){
        fullSlide={id:`b02-discussion-program-ch${chapter}-${key}`,title:listing.title,eyebrow:'BOOK II / COMPLETE SOURCE EXAMPLE',kind:'code',
          keyPoints:[workshop?.experiment||'Predict the result, then trace this complete original program.'],
          narration:workshop?.adaptationNote||listing.validation.detail||'Compare the actual source with its recorded result.',
          actions:['Predict the output before running.','Change one input or expression and explain the resulting behavior.'],sourceRef:file,
          question:{prompt:'Which source operation establishes the promised result?',answer:'Trace the state transition and check the contract at its boundary.'},
          code:{filename:path.basename(editorPath),language:'cpp',runAllowed:true,text:editorCode,input:workshop?.sampleInput||'',
            expectedPhase:'run',expectedStdout:output,expectedStderr:check?.stderr??listing.validation?.stderr??'',
            expectedExitCode:check?.exitCode??listing.validation?.exitCode??0,downloadPath:editorPath,
            bookTwoRecordedObservation:recordedObservation,
            buildCommand:`g++ -std=c++20 -Wall -Wextra -Wpedantic -pthread ${path.basename(editorPath)} -o example\n./example`}};
        deck.slides.push(fullSlide);addedPrograms++;
      }
      examples[key]={title:listing.title,filename:file,lineStart,lineEnd,text,explain:[],
        invariant:workshop?.experiment||'This exact source excerpt requires the context of its original listing; it is not a substitute standalone program.',
        sourceHash:hash(fullCode),downloadPath,fullSlideId:fullSlide?.id||null,
        sourceText:fullSlide&&!workshop?.adapted?undefined:fullCode,
        adaptationNote:workshop?.adapted?workshop.adaptationNote:undefined,editorSourceHash:hash(editorCode),
        sampleInput:workshop?.sampleInput||'',output,recordedObservation,
        checks:workshop?.checks||null};
      return key;
    }
    // Include every C++ listing: headers and implementation units remain exact
    // downloadable excerpts; verified complete workshops get their own editor.
    for(const listing of sources.get(chapter).listings.filter(l=>l.language==='cpp'))await example(listing);
    for(const slide of originals){
      delete slide.bookTwoDiscussionAid;delete slide.bookTwoDiagram;delete slide.bookTwoProjectExample;
      if(slide.diagram?.image&&!slide.diagram.chapter){
        const asset=slide.diagram.image.$asset,key='embedded-'+slide.id;
        if(!/^assets\/[a-f0-9]+\.svg$/.test(asset))throw Error('Invalid embedded Book II diagram.');
        graphs[key]={title:slide.diagram.title,caption:slide.diagram.explanation,image:'data:image/svg+xml;base64,'+Buffer.from(await read('content/'+asset)).toString('base64')};
        slide.bookTwoDiagram=key;
      }
      if(slide.code){
        const w=workshops.find(w=>w.adapted&&w.originalSource.trim()===slide.code.text.trim());
        if(w){const listing=sources.get(chapter).listings.find(l=>l.filename===w.sourceFilename);slide.bookTwoProjectExample=await example(listing);}
      }
      if(!needsBookTwoAid(slide))continue;
      const context=contextChapter(chapter,slide.title),listing=selectListing(sources.get(context),slide);
      const key=await example(listing);
      // Even diagnostic/reference material with no C++ listing receives its
      // own substantive graphical model, rather than an unrelated program.
      slide.bookTwoDiscussionAid={graph:'chapter',example:key};pages++;
    }
    deck.bookTwoDiscussionAids={graphs,examples};
    pending.push({file:entry.path,text:JSON.stringify(deck,null,2)+'\n'});
    report.chapters.push({chapter,pages,examples:Object.keys(examples).length,addedPrograms});
    report.totals.pages+=pages;report.totals.addedPrograms+=addedPrograms;
  }
  for(const [file,text] of downloads){await fs.mkdir(path.dirname(path.join(root,file)),{recursive:true});await fs.writeFile(path.join(root,file),text);}
  for(const p of pending)await fs.writeFile(path.join(root,p.file),p.text);
  await fs.writeFile(path.join(root,'docs/book2-discussion-aids-coverage.json'),JSON.stringify(report,null,2)+'\n');
  console.log(`Added diagrams and exact C++ excerpts to ${report.totals.pages} Book II discussion pages across all 29 decks (${report.totals.addedPrograms} additional complete program slides).`);
  return report;
}
await buildBookTwoDiscussionAids();
