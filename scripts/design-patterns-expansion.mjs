import fs from 'node:fs';import path from 'node:path';import {isDeepStrictEqual} from 'node:util';import {fileURLToPath} from 'node:url';
const root=path.resolve(import.meta.dirname,'..');
export const designPatternsExpansion=JSON.parse(fs.readFileSync(path.join(root,'content/design-patterns-expansion.json'),'utf8'));
const pack=designPatternsExpansion;
function restoreEdits(target,scope,id){for(const e of pack.edits.filter(e=>e.scope===scope&&e.id===id)){if(!isDeepStrictEqual(target[e.field],e.after))throw Error('Unexpected editorial drift: '+id+'.'+e.field);target[e.field]=structuredClone(e.before);}}
export function withoutDesignPatternsExpansion(data){
 const prior=structuredClone(data),c=prior.courses.find(c=>c.id===pack.courseId);const cards=new Set(pack.cards.map(x=>x.id)),views=new Set(pack.diagrams.map(x=>x.expansionId));
 for(const t of c.topics){t.cards=t.cards.filter(x=>!cards.has(x.id));restoreEdits(t,'topic',t.id);}
 for(const d of Object.values(c.diagrams))d.extra_overviews=d.extra_overviews.filter(v=>!views.has(v.expansionId));
 return prior;
}
export function withoutDesignPatternsCoding(coding){
 const prior=structuredClone(coding),ids=new Set(pack.questions.map(x=>x.id)),worked=new Set(pack.workedPrograms.map(x=>x.id));
 prior.questions=prior.questions.filter(x=>!ids.has(x.id));prior.workedPrograms=prior.workedPrograms.filter(x=>!worked.has(x.id));
 for(const q of prior.questions)restoreEdits(q,'question',q.id);return prior;
}
function applyEdits(target,scope,id){for(const e of pack.edits.filter(e=>e.scope===scope&&e.id===id)){if(!isDeepStrictEqual(target[e.field],e.before)&&!isDeepStrictEqual(target[e.field],e.after))throw Error('Source changed; review editorial patch: '+id+'.'+e.field);target[e.field]=structuredClone(e.after);}}
function apply(){
 const read=p=>JSON.parse(fs.readFileSync(path.join(root,p),'utf8')),write=(p,x)=>fs.writeFileSync(path.join(root,p),JSON.stringify(x));const cards=new Set(pack.cards.map(x=>x.id)),views=new Set(pack.diagrams.map(x=>x.expansionId));
 for(let n=1;n<=23;n++){
  const file=`content/${pack.courseId}/ch${n}.json`,chunk=read(file);
  for(const t of chunk.topics){t.cards=t.cards.filter(x=>!cards.has(x.id));applyEdits(t,'topic',t.id);for(const {chapter,topicId,...card} of pack.cards.filter(x=>x.topicId===t.id))t.cards.push(card);}
  const d=chunk.diagrams[String(n)];d.extra_overviews=d.extra_overviews.filter(v=>!views.has(v.expansionId));
  for(const v of pack.diagrams.filter(x=>x.chapter===n)){if(!v.overview.image?.$asset)throw Error('Render diagrams first');d.extra_overviews.push(v.overview);}
  write(file,chunk);
 }
 const file=`content/${pack.courseId}/coding.json`,coding=read(file),ids=new Set(pack.questions.map(x=>x.id)),worked=new Set(pack.workedPrograms.map(x=>x.id));
 coding.questions=coding.questions.filter(x=>!ids.has(x.id));coding.workedPrograms=coding.workedPrograms.filter(x=>!worked.has(x.id));for(const q of coding.questions)applyEdits(q,'question',q.id);
 coding.questions.push(...pack.questions);coding.workedPrograms.push(...pack.workedPrograms);write(file,coding);
 const manifest=read('content/manifest.json');for(const [key,added] of [['questions',pack.questions],['workedPrograms',pack.workedPrograms]]){const newIds=new Set(added.map(x=>x.id));manifest.codingOrder[key]=manifest.codingOrder[key].filter(id=>!newIds.has(id)).concat(added.map(x=>x.id));}write('content/manifest.json',manifest);
 console.log('Applied Design Patterns corrections and study expansion.');
}
if(process.argv[1]&&path.resolve(process.argv[1])===fileURLToPath(import.meta.url)&&process.argv.includes('--apply'))apply();
