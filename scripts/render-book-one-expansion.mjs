// Render editable Graphviz sources and normal SVG assets; no base64 in HTML.
import fs from 'node:fs/promises';import path from 'node:path';import crypto from 'node:crypto';import {instance} from '@viz-js/viz';
const root=path.resolve(import.meta.dirname,'..');process.chdir(root);
const pack=JSON.parse(await fs.readFile('content/book-one-expansion.json','utf8')),viz=await instance();
const esc=s=>String(s).replaceAll('&','&amp;').replaceAll('<','&lt;').replaceAll('>','&gt;').replaceAll('"','&quot;');
for(const study of pack.diagrams){
 const v=study.overview,q=pack.questions.find(q=>q.chapter===study.chapter),body=study.focus[0].code,lines=body.split('\n');
 // All displayed C++ is an actual source excerpt; annotation lines are // comments.
 v.nodes[0].code=[q.driver.slice(0,q.driver.indexOf(';')+1),'// See the full main driver in the source.'];
 v.nodes[1].code=lines.filter(x=>x.trim().startsWith('if (')||x.trim().startsWith('for (')).slice(0,2).map(x=>x.trim());
 if(!v.nodes[1].code.length)v.nodes[1].code=lines.slice(0,3);
 v.nodes[1].code.push('// Checks and loops enforce this contract.');
 if([17,18].includes(study.chapter)){
  v.kind='class';const name=study.chapter===17?'Stock':'Interval';
  v.nodes=[{id:'client',label:'Sample caller',code:[q.driver.slice(0,q.driver.indexOf(';')+1),'// Calls the public interface.']},{id:'type',label:name+' public interface',code:lines.filter(x=>x.includes('int count()')||x.includes('bool take(')||x.includes('Interval(')||x.includes('int low()')||x.includes('int high()')).map(x=>x.trim())},{id:'state',label:'Private value members',code:lines.filter(x=>x.trim().startsWith('int ')&&!x.includes('(')).map(x=>x.trim()).concat('// Values belong to each object.')}];
  v.edges=[{from:'client',to:'type',kind:'dependency',label:'uses the public operations'},{from:'type',to:'state',kind:'ownership',label:'object contains its value members'}];
  v.explanation='Dashed open arrow: caller depends on the public interface. Filled diamond at the class: composition, meaning its value members belong to each object. These are relationships, not execution order. No inheritance is shown.';
 }
 if(study.chapter===16){
  v.kind='state';v.nodes=[{id:'pending',label:'pending',code:['enum class ParcelState { pending, ready };','// The record begins awaiting validation.']},{id:'ready',label:'ready',code:['parcel.state = ParcelState::ready;','// Only the validated transition commits.']}];
  v.edges=[{from:'pending',to:'ready',kind:'transition',label:'releaseParcel: weight in [1,100]'},{from:'pending',to:'pending',kind:'transition',label:'bad weight: false, unchanged'},{from:'ready',to:'ready',kind:'transition',label:'already ready: false, unchanged'}];
  v.explanation='Arrowheads mark allowed state transitions. Self arrows show a rejected call preserving the state. Labels name guards and outcomes; they are not inheritance or resource ownership.';
 }
 if(study.chapter===15){
  v.kind='ownership';v.nodes=[{id:'caller',label:'Caller receives the owner',code:['return result;','// Sole ownership moves to the caller.']},{id:'owner',label:'unique_ptr<int[]> owner',code:['auto result = std::make_unique<int[]>(count);','// Its destructor releases the array.']},{id:'elements',label:'count live array elements',code:['for (std::size_t i = 0; i < count; ++i) result[i] = static_cast<int>(i) + 1;','// Indices must stay below count.']}];
  v.edges=[{from:'owner',to:'elements',kind:'ownership',label:'sole release responsibility; delete[]'},{from:'owner',to:'caller',kind:'transfer',label:'return transfers ownership'}];
  v.explanation='A filled diamond at the unique_ptr marks sole ownership of the dynamic array. The labeled transfer arrow moves that ownership to the caller; it does not duplicate ownership. The count remains a separate access-bound promise.';
 }
 if(study.chapter===23){
  v.kind='sequence';v.nodes=[{id:'owner',label:'Owner construction',code:['if (fail) throw std::runtime_error("construction");','// Failure path: owner never completes.']},{id:'members',label:'Completed members A and B',code:['LoggedMember a, b;','// Destruction runs B, then A.']},{id:'log',label:'Caller-owned log',code:['std::vector<std::string> log;','// Lives through the catch handler.']}];
  v.edges=[{from:'owner',to:'members',kind:'call',label:'1. construct A, then B'},{from:'members',to:'log',kind:'message',label:'2. record A+, then B+'},{from:'owner',to:'log',kind:'message',label:'3. record body'},{from:'owner',to:'members',kind:'unwind',label:'4. constructor throws; unwind'},{from:'members',to:'log',kind:'message',label:'5. destroy B, then A: B- A-'},{from:'owner',to:'log',kind:'message',label:'6. caller catch records caught'}];
  v.explanation='Time moves downward. Solid arrowheads show calls or recorded messages on the failing construction path. Dashed lifelines mark participants, not inheritance. The annotation about no owner destructor is a teaching note; the completed members still clean up in reverse order.';
 }
 let dot='digraph G { graph [rankdir=TB, bgcolor="#fcfbff", pad="0.3", nodesep="0.4", ranksep="0.7"]; node [shape=plain]; edge [fontname="Arial", fontsize=13, color="#7955ac", penwidth=1.8];\n';
 for(const node of v.nodes){
  const code=node.code||[];const width=Math.max(330,...code.map(s=>s.length*8+30));
  dot+=`${JSON.stringify(node.id)} [label=<<TABLE WIDTH="${width}" BORDER="1" COLOR="#b9a3d2" CELLBORDER="0" CELLSPACING="0" CELLPADDING="12"><TR><TD BGCOLOR="#eee4f7"><FONT FACE="Arial" POINT-SIZE="16"><B>${esc(node.label)}</B></FONT></TD></TR><TR><TD BGCOLOR="#ffffff" ALIGN="LEFT"><FONT FACE="Courier" POINT-SIZE="13">${code.map(esc).join('<BR ALIGN="LEFT"/>')}</FONT></TD></TR></TABLE>>];\n`;
 }
 for(const e of v.edges){const relationship=e.kind==='ownership'?'dir=both,arrowtail=diamond,arrowhead=none':e.kind==='dependency'?'style=dashed,arrowhead=vee':'arrowhead=vee';dot+=`${JSON.stringify(e.from)} -> ${JSON.stringify(e.to)} [${relationship},label=${JSON.stringify(e.label)}];\n`;}
 dot+='}';let svg=viz.renderString(dot,{format:'svg'}).replaceAll('font-family="Courier"','font-family="ui-monospace,Consolas,monospace"');
 if(v.kind==='sequence'){
  const positions={owner:215,members:630,log:1045};
  svg='<svg xmlns="http://www.w3.org/2000/svg" width="1260" height="790" viewBox="0 0 1260 790"><defs><marker id="arrow" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto"><path d="M0 0 L10 5 L0 10" fill="none" stroke="#7955ac" stroke-width="1.5"/></marker></defs><rect width="1260" height="790" fill="#fcfbff"/>';
  for(const n of v.nodes){const x=positions[n.id];svg+=`<rect x="${x-200}" y="15" width="400" height="125" rx="12" fill="#eee4f7" stroke="#b9a3d2"/><text x="${x}" y="43" text-anchor="middle" font-family="Arial,sans-serif" font-size="17" font-weight="bold">${esc(n.label)}</text>`;n.code.forEach((code,i)=>{svg+=`<text x="${x-185}" y="${78+i*23}" font-family="ui-monospace,Consolas,monospace" font-size="12">${esc(code)}</text>`;});svg+=`<path d="M${x} 141 V730" stroke="#b9a3d2" stroke-dasharray="6 6"/>`;}
  v.edges.forEach((e,i)=>{const y=203+i*83,x=positions[e.from],z=positions[e.to];svg+=`<line x1="${x}" y1="${y}" x2="${z}" y2="${y}" stroke="#7955ac" stroke-width="2" marker-end="url(#arrow)"/><text x="${(x+z)/2}" y="${y-12}" text-anchor="middle" font-family="Arial,sans-serif" font-size="15" fill="#453252">${esc(e.label)}</text>`;});svg+='<text x="20" y="775" font-family="Arial,sans-serif" font-size="14" fill="#765b86">Teaching annotation: there is no owner- entry on this failure path; the owner never completed construction.</text></svg>';
 }

 const digest=crypto.createHash('sha256').update(svg).digest('hex');const asset=`assets/${digest}.svg`;
 await fs.writeFile('content/'+asset,svg);await fs.mkdir('diagrams/book_one_expansion',{recursive:true});await fs.writeFile(`diagrams/book_one_expansion/ch${String(study.chapter).padStart(2,'0')}.dot`,dot);
 v.image={$asset:asset,prefix:'data:image/svg+xml;base64,'};
}
await fs.writeFile('content/book-one-expansion.json',JSON.stringify(pack,null,2)+'\n');console.log('Rendered 24 focused Book I diagram studies.');
