import {chromium} from 'playwright';
import fs from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath,pathToFileURL} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
let options={headless:true};
if(process.env.CHROMIUM_MODULE){const {default:ch}=await import(pathToFileURL(process.env.CHROMIUM_MODULE));options={...options,args:ch.args,executablePath:process.env.CHROMIUM_PATH||await ch.executablePath()};}
const b=await chromium.launch(options);const p=await b.newPage({viewport:{width:1512,height:1100},reducedMotion:'reduce'});
await p.goto(process.env.STUDIO_URL||pathToFileURL(path.join(root,'dist-offline/index.html')).href);
await p.selectOption('#chapter-select','1');await p.locator('#mode-tabs [data-mode="uml"]').click();
await p.locator('.diagram-section').first().screenshot({path:path.join(root,'docs/preview-uml-overview.png')});
await p.locator('.focus-grid').first().screenshot({path:path.join(root,'docs/preview-uml-focus.png')});
await p.evaluate(()=>scrollTo(0,0));await p.screenshot({path:path.join(root,'docs/preview-desktop.png')});
await p.selectOption('#chapter-select','23');
const v=p.locator('.diagram-section');for(let i=0;i<3;i++)await v.nth(i).screenshot({path:path.join(root,`docs/preview-capstone-${i+1}.png`)});
await p.setViewportSize({width:390,height:844});await p.evaluate(()=>document.activeElement?.blur());await p.locator('.focus-grid').first().screenshot({path:path.join(root,'docs/preview-mobile-focus.png')});
// Capture the exact generated diagrams at a readable raster size for visual inspection.
await fs.mkdir(path.join(root,'docs/diagram-check'),{recursive:true});
for(const f of (await fs.readdir(path.join(root,'assets'))).filter(f=>f.endsWith('.svg'))){
 const svg=await fs.readFile(path.join(root,'assets',f),'utf8');
 await p.setViewportSize({width:1500,height:1500});await p.setContent('<style>body{margin:0;background:#fcfcff}svg{width:100%;height:auto;display:block}</style>'+svg);
 await p.locator('svg').screenshot({path:path.join(root,'docs/diagram-check',f.replace('.svg','.png'))});
}
await b.close();console.log('Visual previews saved.');
