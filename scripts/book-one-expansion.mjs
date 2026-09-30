// Additive authoring step and strict reconstruction of the unchanged historical source.
import fs from 'node:fs';import path from 'node:path';import {fileURLToPath} from 'node:url';
const root=path.resolve(import.meta.dirname,'..');
export const bookOneExpansion=JSON.parse(fs.readFileSync(path.join(root,'content/book-one-expansion.json'),'utf8'));
export function withoutBookOneExpansion(data){
 const prior=structuredClone(data),book=prior.courses.find(c=>c.id==='cpp-book-01');
 const ids=new Set(bookOneExpansion.cards.map(c=>c.id)),studies=new Set(bookOneExpansion.diagrams.map(d=>d.expansionId));let removed=0;
 for(const t of book.topics){const before=t.cards.length;t.cards=t.cards.filter(c=>!ids.has(c.id));removed+=before-t.cards.length;}
 book.series.counts.cards-=removed;
 for(const [key,d] of Object.entries(book.diagrams)){
  if(studies.has(d.expansionId))delete book.diagrams[key];
  else{d.extra_overviews=d.extra_overviews.filter(v=>!studies.has(v.expansionId));d.focus=d.focus.filter(f=>!studies.has(f.expansionId));}
 }
 return prior;
}
export function withoutBookOneQuestions(coding){
 const ids=new Set(bookOneExpansion.questions.map(q=>q.id));return {...coding,questions:coding.questions.filter(q=>!ids.has(q.id))};
}
function apply(){
 const pack=bookOneExpansion,read=p=>JSON.parse(fs.readFileSync(path.join(root,p),'utf8')),write=(p,x)=>fs.writeFileSync(path.join(root,p),JSON.stringify(x));
 const cardIds=new Set(pack.cards.map(c=>c.id)),studyIds=new Set(pack.diagrams.map(d=>d.expansionId));
 for(let n=1;n<=24;n++){
  const file=`content/cpp-book-01/ch${n}.json`,chunk=read(file),topics=new Map(chunk.topics.map(t=>[t.id,t]));
  for(const t of chunk.topics)t.cards=t.cards.filter(c=>!cardIds.has(c.id));
  for(const {topicId,chapter,...card} of pack.cards.filter(c=>c.chapter===n)){
   const t=topics.get(topicId);if(!t)throw Error('Missing target topic: '+topicId);t.cards.push(card);
  }
  chunk.diagrams??={};const old=chunk.diagrams[String(n)];
  if(old&&studyIds.has(old.expansionId))delete chunk.diagrams[String(n)];
  else if(old){old.extra_overviews=old.extra_overviews.filter(v=>!studyIds.has(v.expansionId));old.focus=old.focus.filter(f=>!studyIds.has(f.expansionId));}
  for(const {chapter,...study} of pack.diagrams.filter(d=>d.chapter===n)){
   if(!study.overview.image?.$asset)throw Error('Render the expansion diagrams before applying the pack.');
   if(chunk.diagrams[String(n)]){chunk.diagrams[String(n)].extra_overviews.push(study.overview);chunk.diagrams[String(n)].focus.push(...study.focus);}
   else chunk.diagrams[String(n)]=study;
  }
  write(file,chunk);
 }
 const source=read('content/cpp-book-01/source.json');source.series.counts.cards=read('content/cpp-book-01/ch-1.json').topics.reduce((a,t)=>a+t.cards.length,0);
 for(let n=0;n<=26;n++)source.series.counts.cards+=read(`content/cpp-book-01/ch${n}.json`).topics.reduce((a,t)=>a+t.cards.length,0);
 write('content/cpp-book-01/source.json',source);
 const coding=read('content/cpp-book-01/coding.json'),ids=new Set(pack.questions.map(q=>q.id));coding.questions=coding.questions.filter(q=>!ids.has(q.id)).concat(pack.questions);write('content/cpp-book-01/coding.json',coding);
 const manifest=read('content/manifest.json');manifest.codingOrder.questions=manifest.codingOrder.questions.filter(id=>!ids.has(id)).concat(pack.questions.map(q=>q.id));write('content/manifest.json',manifest);
 console.log('Applied Book I expansion: 400 cards, 24 diagram views, 24 coding questions.');
}
if(process.argv[1]&&path.resolve(process.argv[1])===fileURLToPath(import.meta.url)&&process.argv.includes('--apply'))apply();
