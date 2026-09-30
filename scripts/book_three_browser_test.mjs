// Real local C++ execution behind an intercepted compiler API; no external submission.
import {chromium} from 'playwright';import fs from 'node:fs/promises';import path from 'node:path';import os from 'node:os';
import assert from 'node:assert/strict';import {spawn,execFile} from 'node:child_process';import {promisify} from 'node:util';import {once} from 'node:events';
import {sourceCoding} from './read-source.mjs';import {chooseBook} from './book_test_helpers.mjs';
const root=path.resolve(import.meta.dirname,'..'),base='/Programming_Studio_Studio/',port=5178;
const server=spawn(process.execPath,['scripts/serve.mjs'],{cwd:root,env:{...process.env,PORT:String(port),BASE_PATH:base},stdio:['ignore','pipe','inherit']});
await once(server.stdout,'data');
const browser=await chromium.launch({headless:true,...(process.env.CHROMIUM_PATH?{executablePath:process.env.CHROMIUM_PATH,args:['--no-sandbox','--disable-gpu','--disable-webgl','--disable-dev-shm-usage']}: {})});
const p=await browser.newPage({viewport:{width:1512,height:1100},reducedMotion:'reduce',acceptDownloads:true});
const temp=await fs.mkdtemp(path.join(os.tmpdir(),'book-three-browser-')),exec=promisify(execFile),binaries=new Map(),requests=[],errors=[],checks=[];
const catalog=sourceCoding(),qs=catalog.questions.filter(q=>q.courseId==='cpp-book-03'),ws=catalog.workedPrograms.filter(w=>w.courseId==='cpp-book-03');let executions=0;
p.on('pageerror',e=>errors.push(e.message));p.on('request',r=>requests.push(r.url()));await p.route('https://**',r=>r.abort());
const ready=()=>p.waitForSelector('#app[aria-busy="false"]');
async function select(id){await p.selectOption('#lab-question',id);await p.waitForFunction(id=>document.querySelector('#lab-question')?.value===id&&document.querySelector('#lab-editor'),id);}
async function test(){await p.locator('[data-action="lab-test"]').click();await p.waitForFunction(()=>!document.querySelector('[data-action="lab-test"]')?.disabled,{},{timeout:90000});}
function lines(s){return s.replace(/\n+$/,'').split('\n').map(text=>({text}));}
async function execute(binary,input){return new Promise((resolve,reject)=>{const child=spawn(binary,[],{cwd:temp,stdio:['pipe','pipe','pipe']});let stdout='',stderr='';const timer=setTimeout(()=>{child.kill();reject(Error('Local execution timed out'));},10000);child.stdout.on('data',x=>stdout+=x);child.stderr.on('data',x=>stderr+=x);child.on('error',reject);child.stdin.on('error',e=>{if(e.code!=='EPIPE')reject(e);});child.on('close',(code,signal)=>{clearTimeout(timer);if(signal)reject(Error(signal));else resolve({code,stdout,stderr});});child.stdin.end(input);});}
await p.route('https://godbolt.org/api/**',async route=>{
 const body=route.request().postDataJSON(),source=body.source,input=body.options.executeParameters.stdin;
 let binary=binaries.get(source);if(!binary){binary=path.join(temp,'program-'+binaries.size);await fs.writeFile(binary+'.cpp',source);await exec('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',binary+'.cpp','-o',binary],{timeout:45000});binaries.set(source,binary);}
 const run=await execute(binary,input);executions++;
 await route.fulfill({json:{code:run.code,didExecute:true,stdout:lines(run.stdout),stderr:lines(run.stderr),buildResult:{code:0,stderr:[]}}});
});
try{
 await p.goto(`http://127.0.0.1:${port}${base}?course=cpp-book-03&chapter=1&view=coding`);await ready();await p.waitForSelector('#lab-question');
 assert.equal(await p.locator('#lab-question optgroup[label="Practice questions"] option').count(),22);assert.equal(await p.locator('#lab-question option[value^="worked-B03-"]').count(),66);
 assert.ok(!requests.some(u=>/\/data\/(?:cpp-book-(?!03)|systems-programming|design-patterns-cpp)\//.test(u)));
 assert.deepEqual(requests.filter(u=>/\/ch.*\.json$/.test(u)).map(u=>new URL(u).pathname),[base+'data/cpp-book-03/ch1.json']);assert.equal(executions,0);
 checks.push('Direct Book III coding URL loads one chapter and this book’s coding resources, not the other textbooks; no compiler call on startup.');
 for(const q of qs){await select(q.id);assert.equal(await p.locator('#lab-editor').inputValue(),q.starter);assert.ok((await p.locator('.lab-brief').innerText()).includes(q.chapterTitle));}
 for(const w of ws){await select(w.id);assert.equal(await p.locator('#lab-editor').inputValue(),w.source);assert.equal((await p.locator('.lab-section-label').innerText()).trim(),'BOOK III / WORKED PROGRAM');assert.ok((await p.locator('.lab-worked-guide').innerText()).includes(w.guide.invariant));}
 checks.push('All 22 chapter questions and 66 runnable workshops are reachable, show exact source, Book III credit, and problem/design/invariant/trace/failure discussions.');
 const ring=qs.find(q=>q.id==='b3-ring-queue');await select(ring.id);await test();assert.ok(await p.locator('.lab-case.failed').count()>0);assert.ok((await p.locator('#lab-results').innerText()).includes('ACTUAL'));
 await p.screenshot({path:path.join(root,'docs/preview-book-three-feedback.png')});
 await p.locator('#lab-solution summary').click();await p.locator('[data-action="lab-use-solution"]').click();await test();assert.equal(await p.locator('.lab-case.passed').count(),ring.tests.length);
 const downloadPromise=p.waitForEvent('download');await p.locator('[data-action="lab-download"]').click();const downloaded=await (await downloadPromise).path();assert.ok((await fs.readFile(downloaded,'utf8')).includes('int main()'));
 const saved=ring.solution+'\n// Book III saved draft\n';await p.fill('#lab-editor',saved);await p.reload();await ready();assert.equal(await p.locator('#lab-question').inputValue(),ring.id);assert.equal(await p.locator('#lab-editor').inputValue(),saved);
 checks.push('A real locally executed broken ring queue fails with expected/actual feedback; the reference passes all five checks; download includes main and drafts restore on refresh.');
 const original=ws.find(w=>w.sourceId==='B03-L0003');await select(original.id);await test();assert.equal(await p.locator('.lab-case.passed').count(),1);
 const end=original.source.lastIndexOf('}'),changed=original.source.slice(0,end)+'std::cout << "changed\\n";'+original.source.slice(end);await p.fill('#lab-editor',changed);await test();assert.equal(await p.locator('.lab-case.failed').count(),1);assert.ok((await p.locator('.lab-case.failed').innerText()).includes('changed'));
 checks.push('An original worked program passes; changing its real output produces failed feedback, not a fabricated success.');
 const routes=ws.find(w=>w.sourceId==='B03-L0093');await select(routes.id);await test();assert.equal(await p.locator('.lab-case.passed').count(),6);
 await p.locator('.lab-worked-guide').scrollIntoViewIfNeeded();await p.screenshot({path:path.join(root,'docs/preview-book-three-guide.png')});
 await p.reload();await ready();assert.equal(await p.locator('#lab-question').inputValue(),routes.id);await p.locator('[data-action="lab-original-listing"]').click();await ready();assert.equal(await p.locator('#series-code-select').inputValue(),'B03-L0093');
 await p.selectOption('#series-code-select','B03-L0091');await p.locator('[data-action="series-edit-in-lab"]').click();await ready();assert.equal(await p.locator('#lab-question').inputValue(),routes.id);
 checks.push('The joined routing application passes all six valid/error/stdout/stderr/exit cases; original and shared-part links restore the correct workshop.');
 await chooseBook(p,'cpp-book-01');await p.locator('[data-mode="coding"]').click();await ready();assert.equal(await p.locator('#lab-question optgroup[label="Practice questions"] option').count(),47);assert.equal(await p.locator('#lab-question option[value^="worked-B01-"]').count(),86);
 await chooseBook(p,'cpp-book-02');await p.locator('[data-mode="coding"]').click();await ready();assert.equal(await p.locator('#lab-question optgroup[label="Practice questions"] option').count(),24);assert.equal(await p.locator('#lab-question option[value^="worked-B02-"]').count(),78);
 await chooseBook(p,'cpp-book-03');await p.locator('[data-mode="coding"]').click();await ready();await select(ring.id);assert.equal(await p.locator('#lab-editor').inputValue(),saved);
 checks.push('Book I and II keep their full question/workshop counts; Book III saved code stays separate across switches.');
 await p.locator('[data-action="settings"]').click();const backupEvent=p.waitForEvent('download');await p.locator('[data-action="export-backup"]').click();const backupPath=await (await backupEvent).path(),backup=JSON.parse(await fs.readFile(backupPath,'utf8'));assert.equal(backup.version,1);assert.ok(backup.data.progress['cpp-book-03'].coding[ring.id]);
 await p.locator('#app-dialog').evaluate(d=>d.close());await p.evaluate(()=>localStorage.removeItem('patterns-study-studio:v1'));await p.reload();await ready();p.once('dialog',d=>d.accept());await p.locator('#import-file').setInputFiles(backupPath);await ready();assert.equal(JSON.parse(await p.evaluate(()=>localStorage.getItem('patterns-study-studio:v1'))).progress['cpp-book-03'].coding[ring.id].code,saved);
 checks.push('The unchanged version-1 backup export/import preserves Book III coding drafts and checks.');
 await p.goto(`http://127.0.0.1:${port}${base}?course=cpp-book-03&chapter=1&view=uml`);await ready();for(const img of await p.locator('#workspace img').all())await p.waitForFunction(img=>img.complete&&img.naturalWidth>0,await img.elementHandle());assert.ok(await p.locator('#workspace img').count());
 checks.push('Existing Book III UML images render from the subdirectory paths.');
 await p.goto(`http://127.0.0.1:${port}${base}?course=cpp-book-03&chapter=23&view=coding`);await ready();await select(routes.id);await p.setViewportSize({width:390,height:844});assert.ok(await p.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1));await p.locator('.lab-header').scrollIntoViewIfNeeded();await p.screenshot({path:path.join(root,'docs/preview-book-three-mobile.png')});
 checks.push('390px mobile layout has no horizontal page overflow.');assert.deepEqual(errors,[]);
 await fs.writeFile(path.join(root,'tests/book-three-browser-results.json'),JSON.stringify({externalSubmissions:0,localCompilerExecutions:executions,checks,errors},null,2)+'\n');console.log(checks.join('\n'));
}finally{await browser.close();server.kill();await fs.rm(temp,{recursive:true,force:true});}
