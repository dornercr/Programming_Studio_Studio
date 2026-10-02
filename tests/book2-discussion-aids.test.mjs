import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';
const root=new URL('../',import.meta.url);
const read=file=>fs.readFile(new URL(file,root),'utf8');
const json=async file=>JSON.parse(await read(file));
const hash=text=>crypto.createHash('sha256').update(text).digest('hex');
const entries=(await json('lectures/manifest.json')).filter(e=>e.courseId==='cpp-book-02');
const decks=await Promise.all(entries.map(e=>json(e.path)));

test('Book II aids cover all 29 decks and every original text-only discussion page',()=>{
  assert.deepEqual(entries.map(e=>Number(e.chapter)).sort((a,b)=>a-b),Array.from({length:29},(_,i)=>i));
  for(const d of decks){
    assert.ok(d.bookTwoDiscussionAids,d.title);
    for(const s of d.slides.filter(s=>!s.id.startsWith('b02-discussion-program-'))){
      if(!(s.code||s.diagram||s.visual||s.stages?.length||s.sequence?.length||s.walkthrough?.length))assert.ok(s.bookTwoDiscussionAid,s.title);
      if(s.diagram?.image&&!s.diagram.chapter)assert.ok(s.bookTwoDiagram,s.title);
    }
  }
  assert.equal(decks.length,29);
});

test('Book II discussion excerpts, downloads, and editor links resolve to exact original C++',async()=>{
  for(const d of decks)for(const e of Object.values(d.bookTwoDiscussionAids.examples)){
    const source=await read(e.filename);
    assert.equal(hash(source),e.sourceHash);
    assert.equal(source.split('\n').slice(e.lineStart-1,e.lineEnd).join('\n'),e.text);
    assert.equal(await read(e.downloadPath),source);
    if(e.fullSlideId){const s=d.slides.find(s=>s.id===e.fullSlideId);assert.ok(s?.code?.runAllowed);if(!e.adaptationNote)assert.equal(s.code.text.trim(),source.trim());else{assert.equal(hash(s.code.text),e.editorSourceHash);assert.equal(await read(s.code.downloadPath),s.code.text);}if(e.sampleInput)assert.equal(s.code.input,e.sampleInput);}
    else assert.equal(e.sourceText,source);
  }
});

test('Book II includes every C++ listing and links all complete verified workshops',async()=>{
  const included=new Set(decks.flatMap(d=>Object.values(d.bookTwoDiscussionAids.examples).map(e=>e.filename)));
  const listings=[];
  for(let n=0;n<29;n++)listings.push(...(await json(`content/cpp-book-02/ch${n}.json`)).listings.filter(l=>l.language==='cpp'));
  assert.equal(listings.length,84);
  for(const l of listings)assert.ok(included.has(l.filename),l.filename);
  const workshops=(await json('content/cpp-book-02/coding.json')).workedPrograms;
  assert.equal(workshops.length,78);
  for(const w of workshops){
    const d=decks.find(d=>Number(d.chapter)===Number(w.chapter));
    const e=Object.values(d.bookTwoDiscussionAids.examples).find(e=>e.filename===w.sourceFilename);
    assert.ok(e?.fullSlideId,w.sourceFilename);
    assert.equal(d.slides.find(s=>s.id===e.fullSlideId).code.text.trim(),w.source.trim());
  }
});

test('Book II diagrams are self-contained and both image views fit rather than crop',async()=>{
  for(const d of decks)for(const g of Object.values(d.bookTwoDiscussionAids.graphs)){
    assert.ok(g.title);assert.ok(g.caption);
    assert.match(g.image,/^data:image\/svg\+xml;base64,/);
    assert.match(Buffer.from(g.image.split(',')[1],'base64').toString('utf8'),/<svg/);
  }
  const css=await read('src/book2-discussion-aids.css');
  assert.match(css,/\.b2-aid-figure img\s*\{[^}]*object-fit: contain/);
  assert.match(css,/\.b2-aid-viewer-image\s*\{[^}]*object-fit: contain/);
});

test('Multi-file program links use the established verified workshop adaptation',async()=>{
  const workshops=(await json('content/cpp-book-02/coding.json')).workedPrograms;
  for(const d of decks)for(const e of Object.values(d.bookTwoDiscussionAids.examples).filter(e=>e.adaptationNote)){
    const w=workshops.find(w=>w.sourceFilename===e.filename&&w.adapted);
    assert.ok(w);assert.equal(w.originalSource.trim(),(await read(e.filename)).trim());
    assert.equal(d.slides.find(s=>s.id===e.fullSlideId).code.text,w.source);
  }
});

test('Book II regeneration is deterministic and preserves original slide fields',async()=>{
  const before=await Promise.all(entries.map(e=>read(e.path)));
  const original=d=>d.slides.filter(s=>!s.id.startsWith('b02-discussion-program-')).map(({bookTwoDiscussionAid,bookTwoDiagram,bookTwoProjectExample,...s})=>s);
  const result=spawnSync(process.execPath,['scripts/build-book2-discussion-aids.mjs'],{cwd:root,encoding:'utf8'});
  assert.equal(result.status,0,result.stderr);
  const after=await Promise.all(entries.map(e=>read(e.path)));
  assert.deepEqual(after,before);
  for(let i=0;i<before.length;i++)assert.deepEqual(original(JSON.parse(after[i])),original(JSON.parse(before[i])));
});
