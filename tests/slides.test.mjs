import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
const root=new URL('../',import.meta.url);
const read=p=>fs.readFile(new URL(p,root),'utf8');
const entries=JSON.parse(await read('lectures/manifest.json'));
const chapterOneEntry=entries.find(e=>e.courseId==='cpp-book-01'&&Number(e.chapter)===1);
assert.ok(chapterOneEntry,'Book I Chapter 1 lecture entry exists');
const deck=JSON.parse(await read(chapterOneEntry.path));
test('Chapter 1 lecture retains 26 distinct slide/narration pairs and original worked source',async()=>{
 assert.equal(deck.courseId,'cpp-book-01');assert.equal(deck.chapter,1);assert.equal(deck.slides.length,26);
 assert.equal(new Set(deck.slides.map(s=>s.id)).size,26);
 for(const s of deck.slides){assert.ok(s.title);assert.ok(s.narration.length>100);assert.ok(s.actions.length);assert.ok(s.sourceRef);assert.ok(s.question||s.checks?.length||s.stages?.length||s.sequence?.length);}
 assert.equal(deck.slides[4].code.text.trim(),(await read('cpp_series/book_01/listings/L0003.cpp')).trim());
 assert.equal(deck.slides[13].code.text.trim(),(await read('cpp_series/book_01/listings/L0010.cpp')).trim());
});
test('Only complete C++ examples may run online, and failures distinguish build from process exit',()=>{
 for(const s of deck.slides){if(s.code?.runAllowed){assert.equal(s.code.language,'cpp');assert.match(s.code.text,/\bmain\s*\(/);}if(s.code?.language==='bash')assert.equal(s.code.runAllowed,false);}
 for(const n of [18,19])assert.equal(deck.slides[n-1].code.expectedPhase,'build');
 const mode17=deck.slides[16].code.variants.find(v=>v.standard==='c++17');assert.ok(mode17);assert.match(mode17.text,/__cplusplus/);
 assert.ok(deck.slides[20].sequence.every(x=>x.observation));
});
test('Both builds integrate Slides without embedding all lecture data in the modular app',async()=>{
 const scripts=JSON.parse(await read('package.json')).scripts;
 assert.equal(scripts.build,'node scripts/build-slides.mjs');
 assert.equal(scripts['build:offline'],'node scripts/build-slides.mjs --offline');
 const builder=await read('scripts/build-slides.mjs');
 assert.match(builder,/SLIDES_JS/);assert.match(builder,/dist\/lectures/);assert.match(builder,/src\/slides.css/);
 assert.match(builder,/study-slides/);assert.match(builder,/--offline/);
 const renderer=await read('src/slides.js');assert.match(renderer,/await Library.json\(entry.path\)/);
 assert.ok(!renderer.includes(deck.slides[0].narration));
});
