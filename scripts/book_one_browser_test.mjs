import {sourceData,sourceCoding} from './read-source.mjs';
import {chromium} from 'playwright';import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath,pathToFileURL} from 'node:url';import assert from 'node:assert/strict';import {spawnSync} from 'node:child_process';import {chooseBook} from './book_test_helpers.mjs';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..'),catalog=sourceCoding();
let options={headless:true};if(process.env.CHROMIUM_MODULE){const{default:ch}=await import(pathToFileURL(process.env.CHROMIUM_MODULE));options={...options,args:ch.args,executablePath:process.env.CHROMIUM_PATH||await ch.executablePath()};}
const browser=await chromium.launch(options),p=await browser.newPage({viewport:{width:1512,height:1100},reducedMotion:'reduce'}),errors=[],results=[];p.on('pageerror',e=>errors.push(e.message));
const waitDone=()=>p.waitForFunction(()=>!document.querySelector('[data-action="lab-test"]').disabled,{},{timeout:80000});
try{
 await p.route('https://**',route=>route.abort());
 await p.goto(process.env.STUDIO_URL||pathToFileURL(path.join(root,'dist-offline/index.html')).href);await chooseBook(p,'cpp-book-01');await p.locator('[data-mode="coding"]').click();
 assert.equal(await p.locator('#lab-question optgroup[label="Practice questions"] option').count(),47);assert.equal(await p.locator('#lab-question option[value^="worked-"]').count(),86);assert.equal(await p.locator('#lab-question optgroup').count()>20,true);results.push('Book I has 47 graded questions and 86 source-linked worked labs, grouped by chapter.');
 const original=catalog.workedPrograms.find(w=>w.sourceId==='B01-L0003'),age=catalog.workedPrograms.find(w=>w.sourceId==='B01-L0026');
 const modified=original.source.replace('20.1','20.0');assert.notEqual(modified,original.source);
 // Produce the altered-output fixture by compiling only the known, published example locally.
 const mutationSource=path.join(root,'coding_lab/build/book-one-mutation.cpp'),mutationExe=mutationSource.slice(0,-4);await fs.writeFile(mutationSource,modified);
 const compile=spawnSync('g++',['-std=c++20','-pthread',mutationSource,'-o',mutationExe],{encoding:'utf8',timeout:30000});assert.equal(compile.status,0,compile.stderr);
 const altered=spawnSync(mutationExe,[],{encoding:'utf8',timeout:5000});assert.equal(altered.status,0,altered.stderr);
 const inputs=[];let simulatedRuns=0;
 await p.route('https://godbolt.org/api/**',async route=>{
   const body=route.request().postDataJSON(),code=body.source.replace(/^#line 1 "main.cpp"\n/,'').trimEnd(),stdin=body.options.executeParameters.stdin;
   let c;if(code===original.source.trimEnd())c=original.checks[0];else if(code===modified.trimEnd())c={...original.checks[0],stdout:altered.stdout};else if(code===age.source.trimEnd()){c=age.checks.find(x=>x.input===stdin);inputs.push(stdin);}else{await route.abort();return;}
   assert.ok(c,'Unexpected sample input');simulatedRuns++;
   const records=value=>value.replace(/\n+$/,'').split('\n').map(text=>({text}));
   await route.fulfill({json:{code:c.exitCode,didExecute:true,stdout:records(c.stdout),stderr:c.stderr?records(c.stderr):[],buildResult:{code:0,stderr:[]}}});
 });
 await p.selectOption('#lab-question',original.id);assert.equal(await p.locator('#lab-editor').inputValue(),original.source);assert.ok((await p.locator('#lab-results').textContent()).includes('Ready.'));
 await p.locator('[data-action="lab-test"]').click();await waitDone();let status=await p.locator('#lab-results').textContent();assert.ok(status.includes('matches its checked behavior'),status);assert.equal(await p.locator('.lab-case.passed').count(),1);assert.ok((await p.locator('.lab-worked-count').textContent()).startsWith('1'));results.push('Browser with locally verified compiler response: original Book I sorting program matches observed output.');
 await p.fill('#lab-editor',modified);assert.ok((await p.locator('.lab-run-status').textContent()).includes('Edited since'));await p.locator('[data-action="lab-test"]').click();await waitDone();status=await p.locator('#lab-results').textContent();assert.ok(status.includes('Some checks need attention'),status);assert.equal(await p.locator('.lab-case.failed').count(),1);assert.ok((await p.locator('.lab-case.failed').textContent()).includes('20.1'));assert.ok((await p.locator('.lab-case.failed').textContent()).includes('20'));results.push('Browser with locally compiled altered output: changing C++ fails the original check with expected and actual values.');
 await p.screenshot({path:path.join(root,'docs/preview-book-one-feedback.png')});
 await p.locator('[data-action="lab-reset"]').click();assert.equal(await p.locator('#lab-editor').inputValue(),original.source);await p.locator('[data-action="lab-undo"]').click();assert.equal(await p.locator('#lab-editor').inputValue(),modified);await p.locator('[data-action="lab-reset"]').click();
 await p.selectOption('#lab-question','worked-B01-L0026');
 await p.locator('[data-action="lab-test"]').click();await waitDone();assert.deepEqual(inputs,age.checks.map(x=>x.input));assert.equal(await p.locator('.lab-case.passed').count(),3);results.push('Three recorded age inputs check success, invalid input, and missing input, including stderr and exit codes (browser compiler response simulated from locally verified cases).');

 await p.selectOption('#lab-question','worked-B01-L0129');const joined=catalog.workedPrograms.find(w=>w.sourceId==='B01-L0129');assert.ok(joined.source.includes('Original book listing B01-L0127'));assert.ok(joined.source.includes('Original book listing B01-L0128'));assert.ok((await p.locator('.lab-adaptation').textContent()).includes('exact header'));const downloaded=p.waitForEvent('download');await p.locator('[data-action="lab-download"]').click();const file=await downloaded;const saved=path.join(root,'tests/book-one-downloaded.cpp');await file.saveAs(saved);assert.ok((await fs.readFile(saved,'utf8')).includes('int main('));await fs.unlink(saved);results.push('The chapter 20 inventory lab joins actual book source blocks, labels the adaptation, and downloads a complete program.');
 await p.reload();assert.equal(await p.locator('#lab-question').inputValue(),'worked-B01-L0129');assert.ok((await p.locator('#lab-editor').inputValue()).includes('Original book listing B01-L0127'));results.push('Selected worked example and editor survive reload.');
 await p.locator('[data-action="lab-original-listing"]').click();assert.equal(await p.locator('#series-code-select').inputValue(),'B01-L0129');await p.selectOption('#series-code-select','B01-L0127');await p.locator('[data-action="series-edit-in-lab"]').click();assert.equal(await p.locator('#lab-question').inputValue(),'worked-B01-L0129');results.push('Links move between the exact original header listing and the joined runnable workshop.');
 await p.setViewportSize({width:390,height:844});assert.ok(await p.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1));await p.locator('.lab-header').scrollIntoViewIfNeeded();await p.screenshot({path:path.join(root,'docs/preview-book-one-mobile.png')});results.push('390px mobile layout has no page overflow.');
 assert.deepEqual(errors,[]);await fs.writeFile(path.join(root,'tests/book-one-browser-results.json'),JSON.stringify({date:new Date().toISOString(),liveExternalRequests:0,simulatedFromLocalFixtures:simulatedRuns,simulatedAgeCases:3,results,errors},null,2)+'\n');console.log(results.join('\n'));
}finally{await browser.close();}
