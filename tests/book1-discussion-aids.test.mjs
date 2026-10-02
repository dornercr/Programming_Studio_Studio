import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';
const root=new URL('../',import.meta.url);
const read=file=>fs.readFile(new URL(file,root),'utf8');
const json=async file=>JSON.parse(await read(file));
const hash=text=>crypto.createHash('sha256').update(text).digest('hex');
const entries=(await json('lectures/manifest.json')).filter(e=>e.courseId==='cpp-book-01');
const decks=await Promise.all(entries.map(e=>json(e.path)));

test('Book I aids cover all 27 decks and every original text-only discussion page',()=>{
  assert.deepEqual(entries.map(e=>Number(e.chapter)).sort((a,b)=>a-b),Array.from({length:27},(_,i)=>i));
  for(const d of decks){
    assert.ok(d.bookOneDiscussionAids,d.title);
    for(const s of d.slides.filter(s=>!s.id.startsWith('b01-discussion-program-'))){
      if(!(s.code||s.diagram||s.visual||s.stages?.length||s.sequence?.length||s.walkthrough?.length))assert.ok(s.bookOneDiscussionAid,s.title);
      if(s.diagram?.image&&!s.diagram.chapter)assert.ok(s.bookOneDiagram,s.title);
    }
  }
  assert.equal(decks.find(d=>d.chapter===1).slides.length,26);
});

test('Book I discussion excerpts, downloads, and editor links resolve to exact original C++',async()=>{
  for(const d of decks)for(const e of Object.values(d.bookOneDiscussionAids.examples)){
    const source=await read(e.filename);
    assert.equal(hash(source),e.sourceHash);
    assert.equal(source.split('\n').slice(e.lineStart-1,e.lineEnd).join('\n'),e.text);
    assert.equal(await read(e.downloadPath),source);
    if(e.fullSlideId){const s=d.slides.find(s=>s.id===e.fullSlideId);assert.ok(s?.code?.runAllowed);if(!e.adaptationNote)assert.equal(s.code.text.trim(),source.trim());else{assert.equal(hash(s.code.text),e.editorSourceHash);assert.equal(await read(s.code.downloadPath),s.code.text);}if(e.sampleInput)assert.equal(s.code.input,e.sampleInput);}
    else assert.equal(e.sourceText,source);
  }
});

test('Book I diagrams are self-contained and both image views fit rather than crop',async()=>{
  for(const d of decks)for(const g of Object.values(d.bookOneDiscussionAids.graphs)){
    assert.ok(g.title);assert.ok(g.caption);
    assert.match(g.image,/^data:image\/svg\+xml;base64,/);
    assert.match(Buffer.from(g.image.split(',')[1],'base64').toString('utf8'),/<svg/);
  }
  const css=await read('src/book1-discussion-aids.css');
  assert.match(css,/\.b1-aid-figure img\s*\{[^}]*object-fit: contain/);
  assert.match(css,/\.b1-aid-viewer-image\s*\{[^}]*object-fit: contain/);
});

test('Multi-file program links use the established verified workshop adaptation',async()=>{
  const workshops=(await json('content/cpp-book-01/coding.json')).workedPrograms;
  for(const d of decks)for(const e of Object.values(d.bookOneDiscussionAids.examples).filter(e=>e.adaptationNote)){
    const w=workshops.find(w=>w.sourceFilename===e.filename&&w.adapted);
    assert.ok(w);assert.equal(w.originalSource.trim(),(await read(e.filename)).trim());
    assert.equal(d.slides.find(s=>s.id===e.fullSlideId).code.text,w.source);
  }
});

test('Book I regeneration is deterministic and preserves original slide fields',async()=>{
  const before=await Promise.all(entries.map(e=>read(e.path)));
  const original=d=>d.slides.filter(s=>!s.id.startsWith('b01-discussion-program-')).map(({bookOneDiscussionAid,bookOneDiagram,bookOneProjectExample,...s})=>s);
  const result=spawnSync(process.execPath,['scripts/build-book1-discussion-aids.mjs'],{cwd:root,encoding:'utf8'});
  assert.equal(result.status,0,result.stderr);
  const after=await Promise.all(entries.map(e=>read(e.path)));
  assert.deepEqual(after,before);
  for(let i=0;i<before.length;i++)assert.deepEqual(original(JSON.parse(after[i])),original(JSON.parse(before[i])));
});
