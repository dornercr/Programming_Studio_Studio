import fs from 'node:fs/promises';
import path from 'node:path';
import {spawnSync} from 'node:child_process';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=file=>fs.readFile(path.join(root,file),'utf8');
const original={'src/app.js':await read('src/app.js'),'src/styles.css':await read('src/styles.css')};
function section(text,begin,end,addition){
  const start=text.indexOf(begin),last=text.indexOf(end);
  if((start<0)!==(last<0)||(start>=0&&last<start))throw Error('Incomplete sidebar toggle section. No changes made.');
  if(start<0)return null;
  return text.slice(0,start)+addition.trimEnd()+text.slice(last+end.length);
}
let app=original['src/app.js'];
const js=await read('src/desktop-sidebar.js');
const existing=section(app,'  // BEGIN DESKTOP SIDEBAR TOGGLE','  // END DESKTOP SIDEBAR TOGGLE',js);
if(existing!==null)app=existing;
else{
  const marker='  function renderDrawer()';
  if(!app.includes(marker))throw Error('Cannot locate the navigation renderer. No changes made.');
  app=app.replace(marker,js.trimEnd()+'\n\n'+marker);
}
const header='class="icon-button mobile-only" data-action="open-menu"';
const toggle='class="icon-button sidebar-menu-toggle" data-action="open-menu" aria-controls="sidebar" aria-expanded="true"';
if(!app.includes(toggle)){
  if(!app.includes(header))throw Error('Cannot locate the navigation menu button. No changes made.');
  app=app.replace(header,toggle);
}
if(!app.includes('studioDesktopSidebarSync();')){
  const marker=/function renderDrawer\(\)\s*\{[^\n]*\}/;
  if(!marker.test(app))throw Error('Navigation renderer differs from the expected source. No changes made.');
  app=app.replace(marker,match=>match.slice(0,-1)+'studioDesktopSidebarSync();}');
}
const action="case 'open-menu':state.sidebarOpen=true;renderDrawer();break;";
const desktopAction="case 'open-menu':if(matchMedia('(min-width:801px)').matches)studioDesktopSidebarSetCollapsed(!studioDesktopSidebarCollapsed());else state.sidebarOpen=true;renderDrawer();break;";
if(!app.includes(desktopAction)){
  if(!app.includes(action))throw Error('Cannot locate the menu click handler. No changes made.');
  app=app.replace(action,desktopAction);
}
const search="if(key==='/'){event.preventDefault();";
const reveal="if(key==='/'){event.preventDefault();if(innerWidth>800&&studioDesktopSidebarCollapsed()){studioDesktopSidebarSetCollapsed(false);renderDrawer();}";
if(!app.includes(reveal)){
  if(!app.includes(search))throw Error('Cannot locate the search shortcut. No changes made.');
  app=app.replace(search,reveal);
}
const cssAddition=await read('src/desktop-sidebar.css');
const css=section(original['src/styles.css'],'/* BEGIN DESKTOP SIDEBAR TOGGLE */','/* END DESKTOP SIDEBAR TOGGLE */',cssAddition)
  ??original['src/styles.css'].trimEnd()+'\n\n'+cssAddition;
const syntax=spawnSync(process.execPath,['--input-type=module','--check'],{input:app,encoding:'utf8'});
if(syntax.status!==0)throw Error('Syntax validation failed. No changes made.\n'+syntax.stderr);
if(process.argv.includes('--check'))console.log('Desktop sidebar installer is compatible. No files changed.');
else{
  const stamp=new Date().toISOString().replace(/[:.]/g,'-');let changes=0;
  for(const [file,text]of Object.entries({'src/app.js':app,'src/styles.css':css})){
    if(text===original[file])continue;
    const backup=file+'.before-desktop-sidebar-'+stamp;
    await fs.copyFile(path.join(root,file),path.join(root,backup),fs.constants.COPYFILE_EXCL);
    await fs.writeFile(path.join(root,file),text);changes++;
    console.log(`Updated ${file}; backup: ${backup}`);
  }
  console.log(changes?'Installed. Run npm run build, then npm start.':'Already installed; no files changed.');
}
