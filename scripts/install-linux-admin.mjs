// Additive, repeatable installation. All shared edits are prepared before writing.
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import {fileURLToPath} from 'node:url';
import vm from 'node:vm';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');process.chdir(root);
const CID='linux-system-administration';
const backup=path.join('backups','linux-admin-'+new Date().toISOString().replace(/[:.]/g,'-'));
const planned=new Map();
const read=p=>fs.readFileSync(p,'utf8');
function replace(text,old,value,label){if(!text.includes(old))throw Error('Unsupported source layout at '+label+'. No shared files have been changed.');return text.replace(old,()=>value);}
if(fs.existsSync('scripts/linux-admin-content.json.gz.b64')){
 const bundle=JSON.parse(zlib.gunzipSync(Buffer.from(read('scripts/linux-admin-content.json.gz.b64').trim(),'base64')));
 for(const [file,base64]of Object.entries(bundle.files)){
  if(!/^(content\/linux-system-administration\/|content\/assets\/linux-admin\/|lectures\/linux-system-administration\/|lectures\/transcripts\/linux-system-administration\/|linux-labs\/|docs\/linux-admin-source\.json$|scripts\/linux-(course|lecture)-registration\.json$)/.test(file)||file.split('/').includes('..'))throw Error('Invalid bundle path '+file);
  planned.set(file,Buffer.from(base64,'base64'));
 }
}
const get=p=>planned.has(p)?planned.get(p).toString():read(p);
const registration=JSON.parse(get('scripts/linux-course-registration.json'));
const manifest=JSON.parse(read('content/manifest.json'));
manifest.courses=manifest.courses.filter(c=>c.id!==CID).concat(registration);
planned.set('content/manifest.json',JSON.stringify(manifest)+'\n');
const lectures=JSON.parse(read('lectures/manifest.json'));
const linuxLectures=JSON.parse(get('scripts/linux-lecture-registration.json'));
const remainingLectures=new Map(linuxLectures.map(e=>[String(e.chapter),e]));
const mergedLectures=[];
for(const entry of lectures){
 if(entry.courseId!==CID)mergedLectures.push(entry);
 else if(remainingLectures.has(String(entry.chapter))){mergedLectures.push(remainingLectures.get(String(entry.chapter)));remainingLectures.delete(String(entry.chapter));}
}
mergedLectures.push(...remainingLectures.values());
planned.set('lectures/manifest.json',JSON.stringify(mergedLectures,null,2)+'\n');
let app=read('src/app.js');
const begin='// BEGIN LINUX ADMIN COURSE';const end='// END LINUX ADMIN COURSE';
if(app.includes(begin)){
 const start=app.indexOf(begin),finish=app.indexOf(end,start);
 if(finish<start)throw Error('Incomplete Linux helper block. No shared files have been changed.');
 app=app.slice(0,start)+begin+'\n'+read('src/linux-admin.js')+'\n'+end+app.slice(finish+end.length);
}
if(!app.includes('// BEGIN LINUX ADMIN COURSE')){
 const marker='/*__TEACHING_JS__*/';
 app=replace(app,marker,'// BEGIN LINUX ADMIN COURSE\n'+read('src/linux-admin.js')+'\n// END LINUX ADMIN COURSE\n'+marker,'teaching insertion');
 app=replace(app,'function availableModes(){','function availableModes(){if(linuxCourse())return linuxModes();','course tabs');
 app=replace(app,"function isBook(){return course().kind==='textbook';}","function isBook(){return course().kind==='textbook'||course().kind==='linux-admin';}",'chapter navigation');
 app=replace(app,"if(c.kind!=='textbook')return {};","if(c.kind!=='textbook'&&c.kind!=='linux-admin')return {};",'book imports');
 app=replace(app,"return {kind:'textbook',series:","return {kind:c.kind,...(c.kind==='linux-admin'?{linuxBook:c.linuxBook}:{}),series:",'Linux import metadata');
 app=replace(app,"${m.id==='slides'?'Interactive lecture':","${linuxCourse()?linuxModeCount(m.id,t,c):m.id==='slides'?'Interactive lecture':",'tab counts');
 app=replace(app,'labBeforeRender();','if(linuxCourse()&&visibleTopics().length&&[\'outline\',\'uml\',\'code\',\'book\',\'linuxlabs\'].includes(state.mode)){const t=topic();if(t){slidesBeforeRender();$(\'#workspace\').innerHTML=`<div class="wide-workspace">${linuxWorkspace(t)}</div>`;return;}}\n    labBeforeRender();','Linux reading and labs');
 app=replace(app,'renderBookControls();renderSidebar();renderWorkspace();',"if(linuxCourse()){$('#workspace-label').textContent='READ. OBSERVE. VERIFY. RECOVER.';$('#pack-notice').textContent='Linux System Administration · Complete reading · Local Linux labs';}\n    renderBookControls();renderSidebar();renderWorkspace();",'Linux workspace labels');
}
if(!app.includes('clean.linuxDrafts=linuxCleanDrafts(p.linuxDrafts);'))app=replace(app,'clean.codingSelected=validId(p.codingSelected)?p.codingSelected:null;','clean.linuxDrafts=linuxCleanDrafts(p.linuxDrafts);\n    clean.codingSelected=validId(p.codingSelected)?p.codingSelected:null;','local lab backup restoration');
if(!app.includes("'slides','linuxlabs'].includes(ui.mode)"))app=replace(app,"'slides'].includes(ui.mode)","'slides','linuxlabs'].includes(ui.mode)",'local lab backup mode');
if(!app.includes('linuxDrafts:p.linuxDrafts,coding:'))app=replace(app,'bookmarks:p.bookmarks,notes:p.notes,coding:','bookmarks:p.bookmarks,notes:p.notes,linuxDrafts:p.linuxDrafts,coding:','local lab draft retention');
app=app.replace("if(linuxCourse()&&['outline','uml','code','book','linuxlabs']", "if(linuxCourse()&&visibleTopics().length&&['outline','uml','code','book','linuxlabs']");
new vm.Script(app,{filename:'src/app.js'});planned.set('src/app.js',app);
let slides=read('src/slides.js');
if(!slides.includes('if(s.linuxAdmin)return linuxSlidesWorkbench(s);'))slides=replace(slides,'function slidesWorkbench(s){','function slidesWorkbench(s){\n    if(s.linuxAdmin)return linuxSlidesWorkbench(s);','Linux lecture pane');
new vm.Script('(function(){'+slides+'})()');planned.set('src/slides.js',slides);
let css=read('src/styles.css');
const cssBegin='/* BEGIN LINUX ADMIN CSS */',cssEnd='/* END LINUX ADMIN CSS */';
const cssSection=cssBegin+'\n'+read('src/linux-admin.css')+'\n'+cssEnd;
if(css.includes(cssBegin)){
 const start=css.indexOf(cssBegin),finish=css.indexOf(cssEnd,start);
 if(finish<start)throw Error('Incomplete Linux styles. No shared files have been changed.');
 css=css.slice(0,start)+cssSection+css.slice(finish+cssEnd.length);
}else css+='\n'+cssSection+'\n';
planned.set('src/styles.css',css);
// Make historical comparisons exclude exactly the explicit new course, while
// full migration, asset, search, and source-to-output tests include it unchanged.
for(const file of ['tests/modular.test.mjs','tests/book-one-expansion.test.mjs','tests/book-three.test.mjs']){
 let text=read(file);
 if(!text.includes("import {withoutLinuxAdministration}")){
  let changed=0;
  text=text.replace(/hash\(withoutBookOneExpansion\(withoutDesignPatternsExpansion\((original|data|all)\)\)\)/g,(_,arg)=>{changed++;return `hash(withoutLinuxAdministration(withoutBookOneExpansion(withoutDesignPatternsExpansion(${arg}))))`;});
  text=text.replace('counts(withoutBookOneExpansion(withoutDesignPatternsExpansion(restored)),prior)','counts(withoutLinuxAdministration(withoutBookOneExpansion(withoutDesignPatternsExpansion(restored))),prior)');
  if(!changed)throw Error('Historical comparison not recognized in '+file+'. No shared files have been changed.');
  text="import {withoutLinuxAdministration} from '../scripts/linux-admin-original.mjs';\n"+text;
 }
 planned.set(file,text);
}
// Retain all existing builders; the normal source assembler already picks up
// the registered course and copies its assets. Linux lecture paths use chN.json.
for(const [file,bytes]of planned){
 const data=Buffer.isBuffer(bytes)?bytes:Buffer.from(bytes);
 if(fs.existsSync(file)&&fs.readFileSync(file).equals(data))continue;
 if(fs.existsSync(file)){const dest=path.join(backup,file);fs.mkdirSync(path.dirname(dest),{recursive:true});fs.copyFileSync(file,dest);}
 fs.mkdirSync(path.dirname(file),{recursive:true});fs.writeFileSync(file,data);
}
console.log('Installed Linux System Administration: 33 main chapters, 39 lecture decks, 490 numbered sections, 735 original examples, 33 local labs.');
console.log('Existing courses, lecture registrations and build scripts preserved. Changed files are backed up under '+backup+'.');
console.log('Next: npm run build && npm test && npm start');
