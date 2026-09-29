import {chromium} from 'playwright';import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath,pathToFileURL} from 'node:url';import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');let options={headless:true};if(process.env.CHROMIUM_MODULE){const{default:ch}=await import(pathToFileURL(process.env.CHROMIUM_MODULE));options={...options,args:ch.args,executablePath:process.env.CHROMIUM_PATH||await ch.executablePath()};}
const b=await chromium.launch(options),p=await b.newPage({viewport:{width:900,height:1600},reducedMotion:'reduce'}),reports=[];await fs.mkdir(path.join(root,'docs/diagram-check'),{recursive:true});
for(const name of (await fs.readdir(path.join(root,'assets'))).filter(x=>x.startsWith('systems_')&&x.endsWith('.svg'))){
 const svg=await fs.readFile(path.join(root,'assets',name),'utf8');await p.setContent('<style>body{margin:0;background:#fcfcff}svg{display:block;width:100%;height:auto}</style>'+svg);
 const errors=await p.evaluate(()=>{
  const errors=[];for(const group of document.querySelectorAll('g.node')){
   const boundary=group.querySelector('polygon');if(!boundary)continue;const bound=boundary.getBBox();for(const t of group.querySelectorAll('text')){const r=t.getBBox();if(r.x<bound.x-1||r.x+r.width>bound.x+bound.width+1||r.y<bound.y-1||r.y+r.height>bound.y+bound.height+1)errors.push({node:group.id,text:t.textContent,bound:{x:bound.x,y:bound.y,w:bound.width,h:bound.height},textBox:{x:r.x,y:r.y,w:r.width,h:r.height}});}
  }return errors;
 });reports.push({file:name,errors});await p.locator('svg').screenshot({path:path.join(root,'docs/diagram-check',name.replace('.svg','.png'))});
}
await fs.writeFile(path.join(root,'docs/systems-diagram-layout-check.json'),JSON.stringify(reports,null,2));const failures=reports.filter(x=>x.errors.length);console.log(JSON.stringify({diagrams:reports.length,failures},null,2));await b.close();assert.equal(failures.length,0);
