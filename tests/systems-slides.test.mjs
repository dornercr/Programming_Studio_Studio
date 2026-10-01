import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import crypto from 'node:crypto';
const root=new URL('../',import.meta.url),text=p=>fs.readFile(new URL(p,root),'utf8'),json=async p=>JSON.parse(await text(p));
const entries=(await json('lectures/manifest.json')).filter(x=>x.courseId==='systems-programming');
const decks=await Promise.all(entries.map(e=>json(e.path)));
test('Every Systems chapter has a substantial authored interactive lecture with runnable teaching',async()=>{
 assert.deepEqual(entries.map(e=>e.chapter),Array.from({length:61},(_,i)=>i+1));
 for(const d of decks){assert.ok(d.slides.length>=26,d.title);assert.equal(d.author,'Dr. Charles Dorner');assert.equal(new Set(d.slides.map(s=>s.id)).size,d.slides.length);const c=await json(`content/systems-programming/ch${d.chapter}.json`),ids=new Set(c.topics.map(t=>t.id)),a=await json(`lectures/authoring/systems-programming/ch${d.chapter}.json`),original=[...a.opening,...a.mechanism,...a.closing];assert.ok(original.length>=10);assert.ok(original.filter(s=>s.code?.runAllowed).length>=2);for(const s of original){assert.ok(s.narration.split(/\s+/).length>=80,s.title);assert.ok(s.actions.length);assert.ok(s.question);for(const id of s.sourceRef.match(/\b(?:SP|SYS)\d+\.[A-Z0-9]+/g)||[])assert.ok(ids.has(id),id);}}
});
test('All 2690 original Systems lessons and 1775 source slides survive verbatim in accessible source records',async()=>{
 let topics=0,slides=0;for(const d of decks){const c=await json(`content/systems-programming/ch${d.chapter}.json`),records=d.slides.flatMap(s=>s.sourceLessons||[]);assert.equal(new Set(records.map(t=>t.id)).size,records.length);assert.deepEqual([...records].sort((a,b)=>a.id.localeCompare(b.id)),[...c.topics].sort((a,b)=>a.id.localeCompare(b.id)));topics+=records.length;slides+=records.filter(t=>t.slide).length;}
 assert.equal(topics,2690);assert.equal(slides,1775);
 for(const x of await json('lectures/systems-source-integrity.json'))assert.equal(crypto.createHash('sha256').update(await fs.readFile(new URL(x.file,root))).digest('hex'),x.sha256,x.file);
});
test('Systems programs, UML assets and exact source line references match their underlying source',async()=>{
 let programs=0,diagrams=0;for(const d of decks){const c=await json(`content/systems-programming/ch${d.chapter}.json`);for(const s of d.slides){
 if(s.code?.runAllowed){programs++;assert.match(s.code.text,/\bmain\s*\(/);const identity='code:'+String(s.code.sourcePath||s.code.filename);assert.equal(s.id,`sys-ch${String(d.chapter).padStart(2,'0')}-code-${crypto.createHash('sha256').update(identity).digest('hex').slice(0,16)}`);assert.equal(await text(s.code.downloadPath),s.code.text);if(s.code.sourcePath)assert.equal(await text(s.code.sourcePath),s.code.text);}
 if(s.excerpt?.lineStart){const lines=(await text(s.excerpt.filename)).split('\n');assert.equal(s.excerpt.text.trim(),lines.slice(s.excerpt.lineStart-1,s.excerpt.lineEnd).join('\n').trim(),s.id);}
 if(!s.diagram)continue;diagrams++;const bundle=c.diagrams[String(s.diagram.chapter)],v=s.diagram.view==='focus'?bundle.focus[s.diagram.index]:s.diagram.view==='extra'?bundle.extra_overviews[s.diagram.index]:bundle.overview;assert.ok(v,s.id);
 if(s.diagram.view==='focus'){const lines=(await text(v.file)).split('\n');assert.equal(v.code,lines.slice(v.lineStart-1,v.lineEnd).join('\n'),s.id);assert.equal(v.explain.length,v.lineEnd-v.lineStart+1);}else{await fs.access(new URL('content/'+v.image.$asset,root));assert.ok(v.explanation);}
 }}assert.ok(programs>=305);assert.equal(diagrams,130);
});
test('Known race correction remains prominent while the original account stays available',()=>{
 const d=decks.find(d=>d.chapter===45);for(const id of ['SP45.018','SP45.019']){const s=d.slides.find(s=>s.topicIds?.includes(id));assert.match(s.correction,/undefined behavior/);assert.match(s.narration,/does not bound/);assert.ok(s.sourceLessons.some(t=>t.id===id));}
});
test('Chapter transcripts include every SAY/DO block and chapters stay lazy-loaded',async()=>{
 for(const d of decks){const t=await text(`lectures/transcripts/systems-programming/ch${d.chapter}.txt`);for(const s of d.slides){assert.ok(t.includes(s.narration));assert.ok(t.includes(s.actions.join('\n')));}}
 const renderer=await text('src/slides.js');assert.match(renderer,/await Library.json\(entry.path\)/);assert.match(renderer,/slidesSourceArchive/);assert.ok(!renderer.includes(decks[0].slides[0].narration));
});
