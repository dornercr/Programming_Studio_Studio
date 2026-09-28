import fs from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
import {instance} from '@viz-js/viz';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const data=JSON.parse(await fs.readFile(path.join(root,'src/content.json'),'utf8'));const c=data.courses[0];
const viz=await instance();
const esc=x=>String(x).replaceAll('&','&amp;').replaceAll('<','&lt;').replaceAll('>','&gt;').replaceAll('"','&quot;');
const wrap=(x,max=45)=>String(x).split('\n').flatMap(l=>{const a=[];while(l.length>max){let i=l.lastIndexOf(' ',max);if(i<12)i=max;a.push(l.slice(0,i));l=l.slice(i).trimStart();}a.push(l);return a;});
const nodeLabel=n=>{
 const label=wrap(n.label,32),code=(n.code||[]).flatMap(x=>wrap(x,45));
 // The WASM layout engine has no OS font discovery. Reserve the real width
 // of fixed-width glyphs so browser fonts cannot spill out of the UML box.
 const width=Math.ceil(Math.max(190,...code.map(x=>x.length*7+28),...label.map(x=>x.length*8.6+28)));
 return `<<TABLE BORDER="1" WIDTH="${width}" COLOR="#bdb1da" BGCOLOR="#ffffff" CELLBORDER="0" CELLSPACING="0" CELLPADDING="10"><TR><TD WIDTH="${width}" BGCOLOR="#eee9fb"><B>${label.map(esc).join('<BR/>')}</B></TD></TR><TR><TD WIDTH="${width}" ALIGN="LEFT"><FONT FACE="Courier" POINT-SIZE="11">${code.map(esc).join('<BR ALIGN="LEFT"/>')}</FONT></TD></TR></TABLE>>`;
};
const edgeLabel=e=>{
 const lines=wrap(e.label,23),width=Math.ceil(Math.max(...lines.map(x=>x.length*6.8))+14);
 return `<<TABLE BORDER="0" CELLBORDER="0" WIDTH="${width}" CELLSPACING="0" CELLPADDING="5"><TR><TD WIDTH="${width}">${lines.map(esc).join('<BR/>')}</TD></TR></TABLE>>`;
};
function finishSVG(svg){
 svg=svg.replace(/font-family="Helvetica[^"]*"/g,'font-family="Arial,sans-serif"').replace(/font-family="Courier[^"]*"/g,'font-family="ui-monospace,Consolas,monospace"');
 return svg.replace(/<g id="node\d+" class="node">[\s\S]*?<\/g>/g,group=>{
  const p=group.match(/<polygon[^>]*points="([^"]+)"/);if(!p)return group;
  const xs=p[1].trim().split(/\s+/).map(x=>Number(x.split(',')[0]));const center=(Math.min(...xs)+Math.max(...xs))/2;
  return group.replace(/<text[^>]*font-weight="bold"[^>]*>/g,tag=>tag.replace('text-anchor="start"','text-anchor="middle"').replace(/ x="[^"]+"/,` x="${center}"`));
 });
}
function sequence(v){
 const w=Math.max(780,v.nodes.length*265),h=220+v.edges.length*96;
 const pos=new Map(v.nodes.map((n,i)=>[n.id,70+(i+.5)*(w-140)/v.nodes.length]));
 let b=`<svg xmlns="http://www.w3.org/2000/svg" width="${w}" height="${h}" viewBox="0 0 ${w} ${h}"><defs><marker id="arrow" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto"><path d="M0 0 L10 5 L0 10" fill="none" stroke="#6246ba" stroke-width="1.5"/></marker></defs><rect width="100%" height="100%" fill="#fcfcff"/>`;
 for(const n of v.nodes){const x=pos.get(n.id);b+=`<rect x="${x-116}" y="12" width="232" height="116" rx="10" fill="#eee9fb" stroke="#b7a9df"/>`;
 let y=35;for(const l of wrap(n.label,26)){b+=`<text x="${x}" y="${y}" text-anchor="middle" font-family="sans-serif" font-size="14" font-weight="bold" fill="#302b48">${esc(l)}</text>`;y+=18;}
 for(const l of (n.code||[]).flatMap(l=>wrap(l,28))){b+=`<text x="${x}" y="${y+8}" text-anchor="middle" font-family="monospace" font-size="10" fill="#302b48">${esc(l)}</text>`;y+=14;}
 b+=`<path d="M${x} 129 V${h-32}" stroke="#aaa1bc" stroke-dasharray="5 5"/>`;}
 v.edges.forEach((e,i)=>{const x=pos.get(e.from),z=pos.get(e.to),y=182+i*96;b+=`<line x1="${x}" y1="${y}" x2="${z}" y2="${y}" stroke="#6246ba" stroke-width="2" marker-end="url(#arrow)"/>`;const text=wrap(e.label.replace(/^\d+\s*/,''),45);text.forEach((t,j)=>b+=`<text x="${(x+z)/2}" y="${y-26+j*15}" text-anchor="middle" font-family="sans-serif" font-size="12" fill="#302b48">${j?'':`${i+1}. `}${esc(t)}</text>`);});
 return b+`<text x="20" y="${h-10}" font-family="sans-serif" font-size="11" fill="#747184">Time moves downward. Arrows are calls, not ownership. Return arrows omitted.</text></svg>`;
}
let count=0;
for(const [id,d] of Object.entries(c.diagrams)){
 const source=c.examples[id].examples.code;
 for(const [i,v] of [d.overview,...d.extra_overviews].entries()){
  // Replace overview shorthand that is not verbatim with the nearest exact source line when available.
  for(const n of v.nodes){n.code=n.code.map(x=>{
    if(x.trim().startsWith('//'))return x;
    const match=source.split('\n').find(l=>l.trim()===x.trim());
    if(match)return match.trim();
    return x;
  });}
  let dot=`digraph G { graph [rankdir=${v.kind==='state'?'LR':'TB'}, bgcolor="#fcfcff", pad="0.25", nodesep="0.55", ranksep="0.85", splines=polyline]; node [shape=plain, fontname="Helvetica", fontsize=14]; edge [fontname="Helvetica", fontsize=11, color="#7355df", fontcolor="#514675", penwidth=1.6];\n`;
  for(const n of v.nodes)dot+=`${JSON.stringify(n.id)} [label=${nodeLabel(n)}];\n`;
  for(const e of v.edges){let attrs='arrowhead=vee';if(e.kind==='inheritance')attrs='arrowhead=empty';if(e.kind==='dependency')attrs='style=dashed, arrowhead=vee';if(e.kind==='ownership')attrs='dir=both, arrowtail=diamond, arrowhead=none';if(e.kind==='shared')attrs='style=dashed, arrowhead=vee, color="#267b66"';dot+=`${JSON.stringify(e.from)} -> ${JSON.stringify(e.to)} [${attrs},label=${edgeLabel(e)}];\n`;}
  dot+='}';
  const svg=v.kind==='sequence'?sequence(v):finishSVG(viz.renderString(dot,{format:'svg'}));
  v.image='data:image/svg+xml;base64,'+Buffer.from(svg).toString('base64');
  await fs.writeFile(path.join(root,`assets/ch${id.padStart(2,'0')}_${i+1}.svg`),svg);
  await fs.writeFile(path.join(root,`diagrams/ch${id.padStart(2,'0')}_${i+1}.dot`),dot);
  count++;
 }
}
await fs.writeFile(path.join(root,'src/content.json'),JSON.stringify(data,null,2));console.log('Rendered',count,'diagrams');
