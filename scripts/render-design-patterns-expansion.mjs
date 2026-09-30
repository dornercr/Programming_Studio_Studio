import fs from 'node:fs/promises';import crypto from 'node:crypto';import {instance} from '@viz-js/viz';
const pack=JSON.parse(await fs.readFile('content/design-patterns-expansion.json','utf8')),viz=await instance(),esc=s=>String(s).replaceAll('&','&amp;').replaceAll('<','&lt;').replaceAll('>','&gt;').replaceAll('"','&quot;');
const wrap=(s,n=43)=>{const words=s.split(/\s+/),lines=[];let line='';for(const w of words){if(line.length+w.length>n){lines.push(line);line='';}line+=(line?' ':'')+w;}if(line)lines.push(line);return lines;};
await fs.mkdir('diagrams/design_patterns_expansion',{recursive:true});
for(const d of pack.diagrams){
 const m=JSON.parse(await fs.readFile(`manuscript/ch${String(d.chapter).padStart(2,'0')}.json`,'utf8'));let dot='digraph G { graph [rankdir=TB, bgcolor="#fbfcff", pad="0.25", nodesep="0.45", ranksep="0.5"]; node [shape=plain]; edge [fontname="Arial", fontsize=13,color="#52649b"];\n';
 for(const [i,node]of d.overview.nodes.entries()){
  node.theory={what:node.label+' in the actual demonstration, using the original program.',where:'main runs after the type and function definitions; these are consecutive source excerpts.',why:m.example.design,invariant:m.example.invariants[i%m.example.invariants.length],risk:Object.values(m.example.failures[i % m.example.failures.length]).join(' '),file:d.source,lineStart:node.lineStart,lineEnd:node.lineEnd};
  const code=[`// ${d.source}:${node.lineStart}-${node.lineEnd}`,...node.code];
  dot+=`${node.id} [label=<<TABLE BORDER="1" COLOR="#91a4cf" CELLBORDER="0" CELLSPACING="0" CELLPADDING="12"><TR><TD BGCOLOR="#e6edff"><FONT FACE="Arial" POINT-SIZE="16"><B>${esc(node.label)}</B></FONT></TD></TR><TR><TD ALIGN="LEFT" BGCOLOR="#ffffff"><FONT FACE="Courier" POINT-SIZE="13">${code.map(esc).join('<BR ALIGN="LEFT"/>')}</FONT></TD></TR></TABLE>>];\n`;
  const rows=[['WHAT THIS BLOCK MEANS',node.theory.what],['WHERE IT FITS',node.theory.where],['WHY IT IS HERE',node.theory.why],['WHAT MUST STAY TRUE',node.theory.invariant],['WHAT CAN GO WRONG',node.theory.risk],['FIND THE CODE',`${d.source}:${node.lineStart}-${node.lineEnd}`]];
  dot+=`note${i} [label=<<TABLE BORDER="1" COLOR="#c9b789" CELLBORDER="0" CELLSPACING="0" CELLPADDING="7" BGCOLOR="#fffaf0" WIDTH="510">${rows.map(([label,body])=>`<TR><TD ALIGN="LEFT"><FONT FACE="Arial" POINT-SIZE="14"><B>${label}</B><BR ALIGN="LEFT"/>${wrap(body).map(esc).join('<BR ALIGN="LEFT"/>')}<BR ALIGN="LEFT"/></FONT></TD></TR>`).join('')}</TABLE>>];\n{rank=same;${node.id};note${i};}\n${node.id}->note${i} [style=dashed,arrowhead=none,color="#b5985b",label="teaching annotation"];\n`;
 }
 for(const e of d.overview.edges)dot+=`${e.from}->${e.to} [arrowhead=vee,penwidth=2,label=${JSON.stringify(e.label)}];\n`;
 dot+='}';const svg=viz.renderString(dot,{format:'svg'}).replaceAll('font-family="Courier"','font-family="ui-monospace,Consolas,monospace"');const digest=crypto.createHash('sha256').update(svg).digest('hex'),asset=`assets/${digest}.svg`;await fs.writeFile('content/'+asset,svg);await fs.writeFile(`diagrams/design_patterns_expansion/ch${String(d.chapter).padStart(2,'0')}.dot`,dot);d.overview.image={$asset:asset,prefix:'data:image/svg+xml;base64,'};
}
await fs.writeFile('content/design-patterns-expansion.json',JSON.stringify(pack,null,2)+'\n');console.log('Rendered 22 source-linked activity views with adjacent theory panels.');
