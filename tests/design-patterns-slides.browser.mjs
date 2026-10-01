// Real browser checks at the GitHub Pages project path. No source leaves this machine.
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import path from 'node:path';
import os from 'node:os';
import {spawn,execFile} from 'node:child_process';
import {promisify} from 'node:util';
import {once} from 'node:events';
import {chromium} from 'playwright';
const root=path.resolve(import.meta.dirname,'..');process.chdir(root);
const exec=promisify(execFile),dir=await fs.mkdtemp(path.join(os.tmpdir(),'dp-slides-browser-'));
const qa=process.env.SLIDES_QA_DIR||'build/design-patterns-slides-browser';await fs.mkdir(qa,{recursive:true});
const base='/Programming_Studio_Studio/',origin='http://127.0.0.1:5188',url=origin+base;
const server=spawn(process.execPath,['scripts/serve.mjs'],{env:{...process.env,PORT:'5188',BASE_PATH:base},stdio:['ignore','pipe','inherit']});await once(server.stdout,'data');
const browser=await chromium.launch({headless:true,...(process.env.CHROMIUM_PATH?{executablePath:process.env.CHROMIUM_PATH}:{}),args:['--no-sandbox','--disable-gpu','--disable-webgl','--disable-dev-shm-usage']});
const ctx=await browser.newContext({viewport:{width:1550,height:1050},acceptDownloads:true}),page=await ctx.newPage(),errors=[],requests=[],responses=[],checks=[];
let compilerRequests=0;
page.on('pageerror',e=>errors.push(e.message));page.on('request',r=>requests.push(r.url()));page.on('response',r=>responses.push(r));
await ctx.route('**/*',async route=>{
 const u=new URL(route.request().url());if(u.origin===origin)return route.continue();
 if(!route.request().url().startsWith('https://godbolt.org/api/compiler/'))return route.abort();
 compilerRequests++;const payload=route.request().postDataJSON(),file=path.join(dir,'browser.cpp'),bin=path.join(dir,'browser');await fs.writeFile(file,payload.source);
 let response;
 try{await exec('g++',['-std=c++20','-pthread',file,'-o',bin],{timeout:30000});const r=await exec(bin,[],{timeout:5000});response={code:0,buildResult:{code:0,stdout:[],stderr:[]},execResult:{code:0,didExecute:true,stdout:r.stdout.trimEnd().split('\n').map(text=>({text})),stderr:[]}};}
 catch(e){response={code:1,stdout:[],stderr:[{text:e.stderr||e.message}]};}
 await route.fulfill({contentType:'application/json',body:JSON.stringify(response)});
});
const ready=()=>page.locator('#app[aria-busy="false"]').waitFor({timeout:60000});
const go=async(n,slide=1)=>{await page.goto(`${url}?course=design-patterns-cpp&chapter=${n}&view=slides&slide=${slide}`);await ready();await page.locator('#slides-jump').waitFor();};
const jump=async(i)=>{await page.locator('#slides-jump').selectOption(String(i));};
const deck=async(n)=>JSON.parse(await fs.readFile(`lectures/design-patterns-cpp/ch${n}.json`,'utf8'));
try{
 await page.goto(url+'?course=design-patterns-cpp&chapter=1&view=outline');await ready();assert.ok(!requests.some(x=>x.includes('/lectures/')));assert.equal(compilerRequests,0);
 checks.push('Initial Outline fetches no lecture shards and sends no code.');
 requests.length=0;responses.length=0;await go(1);const initial=[];for(const r of responses){try{initial.push({url:r.url().replace(origin,''),bytes:(await r.body()).length});}catch{}}
 assert.deepEqual(requests.filter(x=>/\/lectures\/.*\.json/.test(x)).map(x=>x.replace(origin+base,'')),['lectures/design-patterns-cpp/ch1.json']);
 assert.ok(!requests.some(x=>/\/data\/(cpp-book|systems)/.test(x)));assert.equal(compilerRequests,0);
 checks.push('Direct lecture load requests only its chapter lecture; no other books.');
 let checkedSlides=0,checkedDiagrams=0;
 for(let n=0;n<=23;n++){
  const d=await deck(n);if(n!==1)await go(n);else await go(1);
  assert.equal(await page.locator('#slides-chapter option').count(),24);assert.equal(await page.locator('#slides-jump option').count(),d.slides.length);
  for(let i=0;i<d.slides.length;i++){
   await jump(i);assert.equal(await page.locator('#slides-title').innerText(),d.slides[i].title);assert.ok(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1),`${n}/${i} desktop overflow`);
   if(d.slides[i].code?.runAllowed){assert.equal(await page.locator('#slides-editor').inputValue(),d.slides[i].code.text);assert.equal(await page.locator('[data-action="slides-run"]').count(),1);}
   if(d.slides[i].diagram||d.slides[i].visual){checkedDiagrams++;if(d.slides[i].diagram?.view==='focus'){assert.ok(await page.locator('.slides-focus-block pre').count());}else{await page.waitForFunction(()=>[...document.querySelectorAll('.slides-diagram')].every(x=>x.complete&&x.naturalWidth>0));}}
   checkedSlides++;
  }
  console.log(`Chapter ${n}: ${d.slides.length} lecture slides rendered`);
 }
 checks.push(`Every slide in all 24 chapters renders; ${checkedDiagrams} diagram/source views resolve.`);
 const d=await deck(1),codeIndex=d.slides.findIndex(s=>s.code?.runAllowed),diagramIndex=d.slides.findIndex(s=>s.diagram?.view==='overview'),focusIndex=d.slides.findIndex(s=>s.diagram?.view==='focus');
 await go(1,codeIndex+1);await page.locator('#slides-room').screenshot({path:path.join(qa,'code-desktop.png')});
 await page.locator('[data-action="slides-run"]').click();await page.waitForFunction(()=>document.querySelector('.slides-status')?.textContent.includes('Run finished'));assert.ok((await page.locator('#slides-results').innerText()).includes(d.slides[codeIndex].code.expectedStdout.trim()));
 const original=await page.locator('#slides-editor').inputValue();await page.locator('#slides-editor').fill(original+'\n// saved lecture draft');await page.reload();await ready();assert.match(await page.locator('#slides-editor').inputValue(),/saved lecture draft/);assert.equal(await page.locator('#slides-jump').inputValue(),String(codeIndex));
 let downloadPromise=page.waitForEvent('download');await page.locator('[data-action="slides-export"]').click();let download=await downloadPromise;const backup=await download.path(),saved=await fs.readFile(backup);const backupPath=path.join(dir,'backup.json');await fs.writeFile(backupPath,saved);
 await page.locator('[data-action="slides-reset"]').click();assert.equal(await page.locator('#slides-editor').inputValue(),original);await page.locator('#slides-import-file').setInputFiles(backupPath);await page.waitForFunction(()=>document.querySelector('#slides-editor')?.value.includes('saved lecture draft'));
 await page.locator('#slides-editor').fill('int main() { BROKEN }');await page.locator('[data-action="slides-run"]').click();await page.waitForFunction(()=>document.querySelector('#slides-results')?.textContent.includes('Build failed'));await page.locator('[data-action="slides-reset"]').click();
 checks.push('Real local compilation, build-error display, edited draft persistence and export/import pass.');
 downloadPromise=page.waitForEvent('download');await page.locator('[data-action="slides-download-code"]').click();download=await downloadPromise;assert.equal(await fs.readFile(await download.path(),'utf8'),original);
 downloadPromise=page.waitForEvent('download');await page.locator('[data-action="slides-transcript"]').click();download=await downloadPromise;assert.ok((await fs.readFile(await download.path(),'utf8')).includes(d.slides.at(-1).narration));
 await jump(diagramIndex);await page.locator('#slides-room').screenshot({path:path.join(qa,'uml-desktop.png')});await page.locator('[data-action="slides-diagram"]').click();assert.equal(await page.locator('.slides-diagram.expanded').count(),1);
 await jump(focusIndex);await page.locator('.slides-excerpt summary').click();await page.locator('#slides-room').screenshot({path:path.join(qa,'source-theory-desktop.png')});
 await page.locator('#slides-presenter summary').click();assert.ok((await page.locator('#slides-presenter').innerText()).includes(d.slides[focusIndex].narration));
 await page.locator('[data-action="slides-present"]').click();await page.waitForFunction(()=>document.body.classList.contains('slides-presenting'));await page.keyboard.press('Escape');await page.waitForFunction(()=>!document.body.classList.contains('slides-presenting'));
 await page.locator('#slides-chapter').selectOption('18');await ready();await page.waitForFunction(()=>document.querySelector('#slides-chapter')?.value==='18');assert.match(await page.locator('.slides-meta').innerText(),/Chapter 18/i);
 await page.locator('#slides-chapter').selectOption('1');await ready();await page.waitForFunction(()=>document.querySelector('#slides-chapter')?.value==='1');assert.equal(await page.locator('#slides-jump').inputValue(),String(focusIndex));
 checks.push('Chapter menu, separate slide position, diagrams, source explanation, presenter mode and downloads pass.');
 await page.evaluate(async()=>{if(document.fullscreenElement)await document.exitFullscreen();});await page.waitForFunction(()=>!document.fullscreenElement);const cdp=await ctx.newCDPSession(page);const {windowId}=await cdp.send('Browser.getWindowForTarget');await cdp.send('Browser.setWindowBounds',{windowId,bounds:{windowState:'normal'}});await cdp.detach();
 await page.setViewportSize({width:390,height:844});for(const n of [0,1,18,23]){const x=await deck(n);for(const i of [0,x.slides.findIndex(s=>s.code),x.slides.findIndex(s=>s.diagram),x.slides.length-1].filter(i=>i>=0)){await go(n,i+1);assert.ok(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1),`${n}/${i} mobile overflow`);}}await go(1,codeIndex+1);await page.locator('#slides-room').screenshot({path:path.join(qa,'code-mobile.png')});
 await page.setViewportSize({width:1550,height:1050});await page.goto(url+'?course=cpp-book-01&chapter=1&view=slides&slide=5');await ready();assert.equal(await page.locator('#slides-jump option').count(),26);assert.match(await page.locator('.slides-meta').innerText(),/C\+\+ Foundations/i);
 await page.locator('#mode-tabs [data-mode="outline"]').click();await ready();assert.equal(await page.locator('#slides-room').count(),0);assert.ok(!await page.locator('body').evaluate(el=>el.classList.contains('slides-active')));
 assert.deepEqual(errors,[]);checks.push('Mobile layout, original Book I lecture and returning to Outline pass.');
 const report={chapters:24,slidesRendered:checkedSlides,diagramsRendered:checkedDiagrams,initialLecturePayloadBytes:initial.reduce((a,x)=>a+x.bytes,0),initialRequests:initial,compilerRequests,externalCompilerServiceContacted:false,checks,errors};await fs.writeFile('tests/design-patterns-slides-browser-results.json',JSON.stringify(report,null,2)+'\n');console.log(JSON.stringify(report,null,2));
}catch(error){console.error('Browser failure state:',await page.locator('#slides-results').innerText().catch(()=>''),errors);throw error;}finally{await browser.close();server.kill();await fs.rm(dir,{recursive:true,force:true});}
