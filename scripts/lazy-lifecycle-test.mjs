import {chromium} from 'playwright';import assert from 'node:assert/strict';import {spawn} from 'node:child_process';import {once} from 'node:events';import fs from 'node:fs/promises';
const server=spawn(process.execPath,['scripts/serve.mjs'],{env:{...process.env,PORT:'5183',BASE_PATH:'/Programming_Studio_Studio/'},stdio:['ignore','pipe','inherit']});await once(server.stdout,'data');
const browser=await chromium.launch({headless:true,...(process.env.CHROMIUM_PATH?{executablePath:process.env.CHROMIUM_PATH,args:['--no-sandbox','--disable-gpu','--disable-webgl','--disable-dev-shm-usage']}: {})}),p=await browser.newPage({reducedMotion:'reduce'}),result=[];
const ready=()=>p.waitForSelector('#app[aria-busy="false"]');
try{
 await p.goto('http://127.0.0.1:5183/Programming_Studio_Studio/?course=cpp-book-01&chapter=1');await ready();
 let fail=true;await p.route('**/data/cpp-book-01/ch2.json',r=>fail?r.fulfill({status:503,body:'Temporary test failure'}):r.continue());
 await p.locator('#chapter-select').selectOption('2');await p.locator('#workspace [role="alert"]').waitFor();assert.ok((await p.locator('#workspace').innerText()).includes('503'));fail=false;await p.locator('[data-action="retry-load"]').click();await p.waitForFunction(()=>new URL(location.href).searchParams.get('chapter')==='2'&&document.querySelector('#app').getAttribute('aria-busy')==='false');assert.equal(await p.locator('#workspace [role="alert"]').count(),0);result.push('Failed chapter request keeps saved state, displays retry, and successfully refetches after recovery.');
 // A slow superseded book selection must not overwrite the last requested book.
 await p.route('**/data/cpp-book-07/catalog.json',async r=>{await new Promise(resolve=>setTimeout(resolve,250));await r.continue();});
 await p.locator('[data-course="cpp-book-07"].book-chapter-dropdown').selectOption('1');await p.locator('[data-course="cpp-book-08"].book-chapter-dropdown').selectOption('1');await p.waitForFunction(()=>document.querySelector('#book-dropdowns').dataset.currentCourse==='cpp-book-08'&&document.querySelector('#app').getAttribute('aria-busy')==='false');await p.waitForTimeout(350);assert.equal(await p.locator('#book-dropdowns').getAttribute('data-current-course'),'cpp-book-08');result.push('A delayed earlier course request cannot overwrite the newer course selection.');
 await fs.writeFile('tests/lazy-lifecycle-results.json',JSON.stringify(result,null,2));console.log(result.join('\n'));
}finally{await browser.close();server.kill();}
