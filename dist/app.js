/* Study Studio — framework-free JavaScript with compiled Tailwind CSS.
 * No analytics or accounts. Only explicit Coding Lab runs call Compiler Explorer.
 * All user-provided text is escaped before HTML rendering.
 */
(async () => {
  'use strict';
  const Library = document.getElementById('study-content') ? null : await import('./loader.js');
  const seed = Library ? Library.seed : JSON.parse(document.getElementById('study-content').textContent);
  const KEY = 'patterns-study-studio:v1';
  const APP = document.getElementById('app');
  const DIALOG = document.getElementById('app-dialog');
  const $ = (selector, parent = document) => parent.querySelector(selector);
  const escape = value => String(value ?? '').replace(/[&<>"']/g, char => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[char]));
  const isObject = x => !!x && typeof x === 'object' && !Array.isArray(x);
  const parseJSON = text => JSON.parse(text, (key,value) => ['__proto__','prototype','constructor'].includes(key) ? undefined : value);
  const safeURL = value => {try {const u = new URL(value);return ['https:','http:'].includes(u.protocol) ? u.href : '';} catch {return '';}};
  const fraction = (n,d) => d ? Math.round(n / d * 100) : 0;
  const blankProgress = () => ({reviewed:[], bookmarks:[], cards:{}, scenarios:{}, notes:{}, coding:{}, codingSelected:null});
  const blankStore = () => ({version:1, customTopics:[], courses:[], progress:{}, ui:{courseId:'design-patterns-cpp',mode:'outline',positions:{},large:false}});
  let storageOK = true, store;
  try {
    const saved = localStorage.getItem(KEY);
    store = saved ? validateStore(parseJSON(saved)) : blankStore();
  } catch { store = blankStore(); storageOK = false; }
  const state = {
    courseId:store.ui.courseId || 'design-patterns-cpp', mode:store.ui.mode || 'outline', topicId:null,
    cardId:null, scenarioId:null, flipped:false, query:'', domain:'all', bookmarksOnly:false,
    scope:'all', chapter:'all', task:'all', bookNav:'chapters', expanded:new Set(['A','1']), sidebarOpen:false, orders:{flashcards:[],scenarios:[]},
    drafts:{}, retries:new Set(), reviewHoldId:null, toastTimer:null
  };
  const ICONS = {
    book:'<path d="M4 4h6a3 3 0 0 1 3 3v14a4 4 0 0 0-4-2H4z"/><path d="M20 4h-4a3 3 0 0 0-3 3v14a4 4 0 0 1 4-2h3z"/>',
    layers:'<rect x="7" y="3" width="13" height="16" rx="3"/><path d="M4 7v12a3 3 0 0 0 3 3h9"/><path d="M11 8h5M11 12h4"/>',
    scenario:'<path d="M20 11a8 8 0 0 1-8 8H5l-3 3V11a9 9 0 0 1 18 0Z"/><path d="m8 10 3 3 5-5"/>',
    search:'<circle cx="10.8" cy="10.8" r="6.8"/><path d="m16 16 4.8 4.8"/>',
    chevron:'<path d="m9 5 7 7-7 7"/>', down:'<path d="m6 9 6 6 6-6"/>',
    left:'<path d="m14 6-6 6 6 6"/><path d="M8 12h12"/>',right:'<path d="m10 6 6 6-6 6"/><path d="M4 12h12"/>',
    check:'<path d="m5 12 4 4L19 6"/>',checkCircle:'<circle cx="12" cy="12" r="9"/><path d="m8 12 3 3 5-6"/>',
    star:'<path d="m12 3 2.8 5.7 6.2.9-4.5 4.4 1.1 6.2-5.6-3-5.6 3 1.1-6.2L3 9.6l6.2-.9z"/>',
    shuffle:'<path d="m17 3 4 4-4 4M17 13l4 4-4 4"/><path d="M3 17h3c5 0 5-10 10-10h5M3 7h3c2 0 3 2 4 4m4 4c1 1 2 2 4 2h3"/>',
    rotate:'<path d="M3 11a9 9 0 1 1 3 7M3 4v7h7"/>',
    bulb:'<path d="M8 15a7 7 0 1 1 8 0v3H8zM9 21h6M10 18v3M14 18v3"/>',
    settings:'<path d="M4 7h16M4 17h16"/><circle cx="8" cy="7" r="3" fill="white"/><circle cx="16" cy="17" r="3" fill="white"/>',
    source:'<path d="M14 3H5a2 2 0 0 0-2 2v15a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2V10zM14 3v7h7M7 14h10M7 18h7"/>',
    external:'<path d="M14 3h7v7M21 3l-10 10"/><path d="M10 3H5a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-5"/>',
    menu:'<path d="M4 6h16M4 12h16M4 18h16"/>',close:'<path d="m6 6 12 12M6 18 18 6"/>',
    plus:'<path d="M12 4v16M4 12h16"/>',download:'<path d="M12 3v12m-5-5 5 5 5-5M4 16v4h16v-4"/>',
    upload:'<path d="M12 15V3m-5 5 5-5 5 5M4 16v4h16v-4"/>',
    lock:'<rect x="5" y="10" width="14" height="11" rx="2"/><path d="M8 10V7a4 4 0 0 1 8 0v3M12 14v3"/>',
    type:'<path d="M3 19 9 4l6 15M5 14h8M16 10h6M19 10v9"/>',
    target:'<circle cx="12" cy="12" r="9"/><circle cx="12" cy="12" r="5"/><circle cx="12" cy="12" r="1"/>',
    clock:'<circle cx="12" cy="12" r="9"/><path d="M12 7v5l3 2"/>',
    edit:'<path d="m15 4 5 5M4 16 16 4a2 2 0 0 1 4 4L8 20l-5 1z"/>',
    info:'<circle cx="12" cy="12" r="9"/><path d="M12 11v6M12 7h.01"/>',
    grid:'<rect x="3" y="3" width="7" height="7" rx="1.5"/><rect x="14" y="3" width="7" height="7" rx="1.5"/><rect x="3" y="14" width="7" height="7" rx="1.5"/><rect x="14" y="14" width="7" height="7" rx="1.5"/>'
  };
  const icon = name => `<svg class="icon" viewBox="0 0 24 24" aria-hidden="true">${ICONS[name] || ICONS.book}</svg>`;
  const modes = [{id:'uml',label:'UML',icon:'grid',color:'bg-brand-soft text-brand'}, {id:'code',label:'C++',icon:'source',color:'bg-mint text-mint-ink'}, {id:'outline',label:'Outline',icon:'book',color:'bg-brand-soft text-brand'}, {id:'flashcards',label:'Flashcards',icon:'layers',color:'bg-peach text-peach-ink'}, {id:'scenarios',label:'Scenarios',icon:'scenario',color:'bg-mint text-mint-ink'}];

  modes.push(...modes.splice(0,2));
  modes.push({id:'lectures',label:'Lectures',icon:'source',color:'bg-mint text-mint-ink'});
  modes.push({id:'coding',label:'Coding Lab',icon:'edit',color:'bg-mint text-mint-ink'});
  modes.push({id:'book',label:'Book',icon:'book',color:'bg-brand-soft text-brand'});
  function availableModes(){return modes.filter(m=>!['uml','code','lectures','book'].includes(m.id)||(m.id==='book'&&course().series)||(m.id==='uml'&&Object.keys(course().diagrams||{}).length)||(m.id==='code'&&(Object.keys(course().examples||{}).length||course().series?.listings.length))||(m.id==='lectures'&&Object.keys(course().lectures||{}).length));}
  function courses() {
    return seed.courses.map(c => c.id === 'my-material' ? {...c,topics:store.customTopics} : c).concat(store.courses);
  }
  function course() {return courses().find(c=>c.id===state.courseId) || courses()[0];}
  function progress() {return store.progress[state.courseId] ||= blankProgress();}
  function topic() {return course().topics.find(t=>t.id===state.topicId) || null;}
  function titleForDomain(id) {return course().domains.find(d=>d.id===id)?.title || 'Study topics';}
  function remember() {
    store.ui.courseId=state.courseId;store.ui.mode=state.mode;
    store.ui.positions[state.courseId]={topicId:state.topicId,cardId:state.cardId,scenarioId:state.scenarioId};
    try {localStorage.setItem(KEY,JSON.stringify(store));storageOK=true;}
    catch {storageOK=false;}
    const status=$('#save-status');
    if(status) status.innerHTML=`<span class="status-dot" ${storageOK?'':'style="background:#d0993b"'}></span>${storageOK?'Saved on this device':'Session only · export to save'}`;
  }
  function restorePosition() {
    if(!courses().some(c=>c.id===state.courseId))state.courseId='design-patterns-cpp';
    const pos=store.ui.positions[state.courseId] || {};
    state.topicId=pos.topicId || course().topics[0]?.id || null;state.lessonView=state.topicId===chapterGuide()?.startTopicId?'map':'lesson';
    state.cardId=pos.cardId || null;state.scenarioId=pos.scenarioId || null;
    state.expanded.add(topic()?.domain || course().domains[0]?.id || 'A');state.expanded.add('CH'+(topic()?.chapter??1));if(isBook())state.scope='chapter';
  }
  function visibleTopics() {
    const q=state.query.trim().toLocaleLowerCase(),p=progress();
    return course().topics.filter(t=>
      (state.domain==='all'||t.domain===state.domain||t.domains?.includes(state.domain))&&
      (!isBook()||state.chapter==='all'||String(t.chapter)===state.chapter)&&
      (!isBook()||state.task==='all'||t.tasks?.includes(state.task))&&
      (!state.bookmarksOnly||p.bookmarks.includes(t.id))&&
      (!q||(Library&&course().catalog?Library.searchMatch(course(),t,q):`${JSON.stringify(t.blocks||[])} ${JSON.stringify(t.reading||{})} ${t.id} ${t.title} ${t.chapterTitle||''} ${(t.tasks||[]).join(' ')} ${t.summary} ${t.concepts.map(c=>`${c.term} ${c.definition}`).join(' ')}`.toLocaleLowerCase().includes(q))));
  }
  function deck(mode=state.mode) {
    let list=visibleTopics();const current=topic();const p=progress();
    if(state.scope==='topic')list=list.filter(t=>t.id===state.topicId);
    if(state.scope==='domain'&&current)list=list.filter(t=>t.domain===current.domain);
    if(state.scope==='chapter'&&current)list=list.filter(t=>t.chapter===current.chapter);
    if(state.scope==='bookmarked')list=list.filter(t=>p.bookmarks.includes(t.id));
    let items=mode==='flashcards'?list.flatMap(t=>t.cards.map(c=>({...c,topic:t}))):course().series?list.flatMap(t=>(t.practice||[]).map(e=>({...e,topic:t}))):list.filter(t=>t.scenario).map(t=>({...t.scenario,topic:t}));
    if(state.scope==='review')items=items.filter(item=>mode==='flashcards'?p.cards[item.id]?.rating==='again':(p.scenarios[item.id]?.correct===false || item.id===state.reviewHoldId));
    const order=state.orders[mode];
    if(order?.length){const index=new Map(order.map((id,i)=>[id,i]));items.sort((a,b)=>(index.get(a.id)??999999)-(index.get(b.id)??999999));}
    return items;
  }
  function activeItem() {
    const items=deck();const id=state.mode==='flashcards'?state.cardId:state.scenarioId;
    return items.find(x=>x.id===id)||items.find(x=>x.topic.id===state.topicId)||items[0]||null;
  }
  function synchronize() {
    if(!availableModes().some(m=>m.id===state.mode))state.mode='outline';
    const all=course().topics;if(!all.length){state.topicId=null;return;}
    if(!all.some(t=>t.id===state.topicId))state.topicId=all[0].id;
    const list=visibleTopics();
    if(list.length&&!list.some(t=>t.id===state.topicId))state.topicId=list[0].id;
    if(['flashcards','scenarios'].includes(state.mode)){
      const item=activeItem();
      if(item){state.topicId=item.topic.id;if(state.mode==='flashcards')state.cardId=item.id;else state.scenarioId=item.id;}
    }
    state.expanded.add(topic()?.domain);state.expanded.add('CH'+(topic()?.chapter??1));
  }
  function totals() {const t=course().topics;return {topics:t.length,cards:t.reduce((n,t)=>n+t.cards.length,0),scenarios:course().series?t.reduce((n,t)=>n+(t.practice||[]).length,0):t.filter(t=>t.scenario).length};}
  function stats() {
    const p=progress(), all=course().topics, ids=new Set(all.map(t=>t.id));
    const cards=new Set(all.flatMap(t=>t.cards.map(c=>c.id)));const scenarios=new Set(course().series?all.flatMap(t=>(t.practice||[]).map(e=>e.id)):all.filter(t=>t.scenario).map(t=>t.scenario.id));
    const reviewed=p.reviewed.filter(id=>ids.has(id)).length;
    const known=Object.entries(p.cards).filter(([id,x])=>cards.has(id)&&x.rating==='known').length;
    const attempts=Object.entries(p.scenarios).filter(([id])=>scenarios.has(id));
    return {reviewed,known,attempted:attempts.length,accuracy:fraction(attempts.filter(([,x])=>x.firstCorrect).length,attempts.length)};
  }
  function toast(message) {const el=$('#toast');el.textContent=message;el.hidden=false;clearTimeout(state.toastTimer);state.toastTimer=setTimeout(()=>el.hidden=true,3200);}

  function renderBookDropdowns(){
    const host=$('#book-dropdowns'),scroll=host.scrollTop;
    host.innerHTML=courses().filter(c=>c.id!=='my-material').map(c=>{
      const active=c.id===state.courseId,current=active?topic()?.chapter:null,id='book-menu-'+c.id;
      const chapterName=ch=>c.series?`${seriesChapterLabel(ch)} · ${ch.title}`:`${ch.number===0?'Foundations':'Chapter '+ch.number} · ${ch.title}`;
      return `<div class="book-menu ${active?'active':''}" data-course="${escape(c.id)}"><label for="${escape(id)}"><span class="book-menu-icon">${icon('book')}</span><span>${escape(c.title)}</span>${active?'<span class="book-menu-current" aria-label="Current book"></span>':''}</label><div class="course-control"><select class="book-chapter-dropdown" id="${escape(id)}" data-course="${escape(c.id)}" aria-label="${escape(c.title)} — choose chapter"><option value="" ${!active?'selected':''} disabled>Choose a chapter…</option><option value="resume" ${active&&current==null?'selected':''}>Resume this book</option>${(c.chapters||[]).map(ch=>`<option value="${ch.number}" ${active&&current===ch.number?'selected':''}>${escape(chapterName(ch))}</option>`).join('')}${c.glossary?.length?`<option value="-1" ${active&&current===-1?'selected':''}>Glossary</option>`:''}</select><span class="select-caret icon-sm">${icon('down')}</span></div></div>`;
    }).join('');host.scrollTop=scroll;
    host.dataset.currentCourse=state.courseId;$('#book-menu-count').textContent=String(courses().filter(c=>c.id!=='my-material').length);
  }
  let courseRequest=0;
  async function openBookDropdown(id,selection='resume'){
    const request=++courseRequest;
    if(Library)try{await Library.ensureCourse(id);if(request!==courseRequest)return;}catch(error){toast('Could not load this book. '+error.message);return;}
    if(!courses().some(c=>c.id===id))return;
    state.courseId=id;state.chapter='all';state.task='all';state.query='';state.domain='all';state.scope='all';state.bookmarksOnly=false;state.flipped=false;state.orders={flashcards:[],scenarios:[]};state.drafts={};state.retries.clear();restorePosition();
    if(selection==='-1'&&course().topics.some(t=>t.id==='REF.GLOSSARY')){state.mode='outline';state.chapter='-1';state.topicId='REF.GLOSSARY';state.lessonView='lesson';state.scope='topic';}
    else if(selection!=='resume'){
      const n=Number(selection),g=course().teaching?.[String(n)],first=g?.startTopicId||course().topics.find(t=>t.chapter===n)?.id;
      if(first){state.mode='outline';state.chapter=String(n);state.topicId=first;state.cardId=null;state.scenarioId=null;state.lessonView='map';state.scope='chapter';}
    }
    state.sidebarOpen=false;renderAll();window.scrollTo({top:0,behavior:'instant'});
  }

  function renderShell() {
    APP.innerHTML=`
      <div id="drawer-overlay" class="drawer-overlay" data-action="close-menu"></div>
      <aside id="sidebar" class="sidebar" aria-label="Curriculum and topic navigation">
        <div class="px-6 pt-7 pb-6">
          <div class="flex items-center gap-3"><span class="brand-mark">${icon('book')}</span><div><div class="text-lg font-bold tracking-tight">Study Studio<span class="text-brand">.</span></div><div class="text-[10px] text-muted mt-0.5">A little progress, every day.</div></div><button class="icon-button mobile-only ml-auto" data-action="close-menu" aria-label="Close navigation">${icon('close')}</button></div>
        </div>
        <div class="book-library-heading"><span class="eyebrow">Your books · <span id="book-menu-count"></span></span><button class="text-muted hover:text-brand icon-sm" data-action="add-topic" aria-label="Add your own study topic" title="Add your own study topic">${icon('plus')}</button></div>
        <section id="book-dropdowns" class="book-dropdowns" aria-label="Choose a chapter from a book"></section>
        <div class="px-5 pb-4 pt-3">
          <button class="personal-library-link" data-action="personal-material">${icon('edit')} My study material</button>
          <div class="search-box mt-3">${icon('search')}<input id="topic-search" type="search" placeholder="Search the current book" aria-label="Search topics" autocomplete="off"><kbd class="key">/</kbd></div>
        </div>
        <div class="px-5 pb-3 flex gap-2"><button id="all-topics-button" class="flex-1 text-left text-[11px] font-semibold px-3 py-2 rounded-lg bg-brand-soft text-brand" data-action="all-topics">All topics <span id="all-count" class="ml-1 opacity-60"></span></button><button id="saved-filter" class="icon-button" data-action="filter-bookmarks" aria-label="Show bookmarked topics" title="Bookmarked topics">${icon('star')}</button></div>
        <div id="book-nav-controls" class="px-5 pb-3"></div><div class="px-6 pb-2 flex justify-between"><span class="eyebrow">Course outline</span><span id="filtered-count" class="text-[10px] text-muted"></span></div>
        <nav class="nav-scroll" id="topic-nav" aria-label="Topics"></nav>
        <div class="sidebar-bottom" id="sidebar-progress"></div>
      </aside>
      <main class="app-main"><div class="main-inner">
        <header class="topline"><div class="flex items-center gap-2"><button class="icon-button mobile-only" data-action="open-menu" aria-label="Open curriculum and topics">${icon('menu')}</button><span class="desktop-breadcrumb">My workspace <span class="mx-2 text-line">/</span></span><span class="text-muted" id="breadcrumb-course">Study room</span></div><div class="flex items-center gap-3 top-actions"><span id="save-status" class="local-indicator"></span><button class="icon-button" data-action="sources" aria-label="Sources and content notes" title="Sources and content notes">${icon('source')}</button><button class="icon-button" data-action="settings" aria-label="Backup, import, and settings" title="Backup, import, and settings">${icon('settings')}</button></div></header>
        <div id="storage-warning"></div>
        <section class="mb-6"><div class="flex items-center gap-2 mb-3"><span class="pill bg-brand-soft text-brand">YOUR STUDY SPACE</span><span class="text-[10px] text-muted" id="course-domain-count"></span></div><h1 class="course-title text-[34px] font-bold tracking-tight leading-tight mb-2">Make it make sense.</h1><p class="text-[13px] text-muted leading-relaxed" id="course-description">Read it. Recall it. Put it into practice.</p></section>
        <div class="grid gap-3 mb-6 mode-tabs" id="mode-tabs" role="group" aria-label="Choose study material"></div>
        <div class="workspace-toolbar flex items-center justify-between gap-3 mb-4 flex-wrap"><div class="flex items-center gap-2"><span class="eyebrow" id="workspace-label">THE OUTLINE</span><span id="filter-status"></span></div><div class="flex items-center gap-2"><label for="domain-select" class="sr-only">Filter domain</label><select id="domain-select" class="select-small" aria-label="Filter by family"></select><button id="large-text" class="icon-button" data-action="large-text" aria-label="Toggle larger reading text" title="Larger reading text">${icon('type')}</button></div></div>
        <div id="book-toolbar" class="mb-4"></div><div id="workspace" tabindex="-1"></div>
        <footer class="source-notice flex items-center justify-between gap-4 mt-5 text-[10px] text-muted flex-wrap"><span id="pack-notice"></span><button class="source-link" data-action="sources">Content &amp; source notes ${icon('chevron')}</button></footer>
      </div></main>`;
    renderAll();
  }
  let renderVersion=0;
  async function renderAll() {
    const version=++renderVersion;
    APP.setAttribute('aria-busy','true');
    if($('#workspace'))$('#workspace').inert=true;
    try {
      if(Library){
        await Library.ensureCourse(state.courseId);
        if(version!==renderVersion)return;
        const c=course();
        if(state.query.trim())await Library.ensureSearch(c);
        if(version!==renderVersion)return;
        synchronize();
        if(await Library.ensureChapter(c,String(topic()?.chapter)))seriesCache.delete(c);
        if(state.mode==='coding')await Library.ensureLabs(c);
        if(version!==renderVersion)return;
      }
      renderAllReady();
      if(Library)writeRoute();
      document.dispatchEvent(new CustomEvent('studio:rendered'));
    }catch(error){
      if(version===renderVersion){console.error(error);$('#workspace').innerHTML=`<section class="panel p-6" role="alert"><h2>Unable to load this material</h2><p>${escape(error.message)}</p><p>Your saved work is still on this device.</p><button class="btn" data-action="retry-load">Try again</button></section>`;}
    }finally{if(version===renderVersion){APP.setAttribute('aria-busy','false');if($('#workspace'))$('#workspace').inert=false;}}
  }
  function renderAllReady() {
    synchronize();
    document.body.classList.toggle('large-reading',!!store.ui.large);
    const c=course(), t=totals();
    renderBookDropdowns();
    $('#breadcrumb-course').textContent=c.title;
    $('#course-description').textContent=c.description;
    $('#course-domain-count').textContent=isBook()?`${c.coverageLabel||'Foundations + 22 patterns + capstone'} · ${t.topics} study entries`:`${c.domains.length} ${c.domains.length===1?'domain':'domains'} · ${t.topics} topics`;
    $('#all-count').textContent=t.topics;
    $('#saved-filter').classList.toggle('active',state.bookmarksOnly);
    $('#saved-filter').setAttribute('aria-pressed',String(state.bookmarksOnly));
    $('#large-text').classList.toggle('active',!!store.ui.large);
    $('#large-text').setAttribute('aria-pressed',String(!!store.ui.large));
    $('#topic-search').value=state.query;
    $('#domain-select').innerHTML=`<option value="all">${c.familyLabel?'All subject areas':'All families'}</option>`+c.domains.map(d=>`<option value="${escape(d.id)}" ${state.domain===d.id?'selected':''}>${escape(d.id)} · ${escape(d.title)}</option>`).join('');
    $('#mode-tabs').style.setProperty('--mode-count',availableModes().length);
    $('#mode-tabs').innerHTML=availableModes().map(m=>`<button class="mode-tab ${state.mode===m.id?'active':''}" data-action="mode" data-mode="${m.id}" aria-pressed="${state.mode===m.id}"><span class="mode-icon ${m.color}">${icon(m.icon)}</span><span class="min-w-0"><span class="mode-label block text-[13px] font-semibold ${state.mode===m.id?'text-brand':'text-ink'}">${c.series&&m.id==='scenarios'?'Practice':c.series&&m.id==='code'?'Code & labs':m.label}</span><span class="mode-count block mt-1 text-[10px] text-muted">${m.id==='coding'?`${labQuestions().length} questions · run C++`:m.id==='outline'?(c.teaching?`${c.chapters.length} ${c.series?'reading maps':'chapter maps'}`:`${t.topics} lessons`):m.id==='flashcards'?`${t.cards} cards`:m.id==='scenarios'?`${t.scenarios} ${c.series?'prompts':'cases'}`:m.id==='lectures'?`${Object.keys(c.lectures||{}).length} chapters`:m.id==='uml'?`${Object.values(c.diagrams||{}).reduce((n,d)=>n+1+(d.extra_overviews||[]).length,0)} diagram views`:m.id==='book'?`${c.series.counts.chapters} chapters · PDF & EPUB`:c.series?`${c.series.listings.length} source listings`:`${Object.keys(c.examples||{}).length} working examples`}</span></span></button>`).join('');
    $('#workspace-label').textContent={outline:'READ, UNDERSTAND, AND DISCUSS',flashcards:'TRAIN YOUR RECALL',scenarios:'PUT IT INTO PRACTICE',uml:'FOLLOW THE RELATIONSHIPS',code:'READ THE WORKING C++',lectures:'LECTURE MATERIALS',book:'THE COMPLETE BOOK',coding:'BUILD IT. TEST IT. UNDERSTAND IT.'}[state.mode];
    $('#filter-status').innerHTML=state.query||state.bookmarksOnly?`<button class="filter-chip" data-action="clear-filters">${escape(state.query?`“${state.query.length>22?state.query.slice(0,22)+'…':state.query}”`:'Saved topics')} ${icon('close')}</button>`:'';
    $('#pack-notice').textContent=isBook()?`${c.title} · ${c.author||'Dr. Charles Dorner'} · Offline reading · online Coding Lab`:'Your study material · Stored locally in this browser.';
    $('#storage-warning').innerHTML=storageOK?'':'<div class="storage-warning">Browser storage is unavailable. Your work is usable in this session; export a backup to keep your progress.</div>';
    renderBookControls();renderSidebar();renderWorkspace();renderSidebarProgress();renderDrawer();remember();
  }
  function renderSidebar() {
    const list=visibleTopics(),c=course(),p=progress();
    if(isBook())return renderBookSidebar(list,c,p);
    $('#filtered-count').textContent=list.length===c.topics.length?'':`${list.length} shown`;
    $('#topic-nav').innerHTML=c.domains.map(d=>{
      const ts=list.filter(t=>t.domain===d.id);if(!ts.length)return '';
      const expanded=state.expanded.has(d.id)||!!state.query;const active=topic()?.domain===d.id;
      return `<div class="mb-1"><button class="domain-row ${active?'active':''}" data-action="domain-toggle" data-id="${escape(d.id)}" aria-expanded="${expanded}"><span class="domain-letter">${escape(d.id)}</span><span class="domain-title">${escape(d.title)}</span><span class="text-[9px] text-muted">${ts.length}</span><span class="icon-sm text-muted">${icon(expanded?'down':'chevron')}</span></button>${expanded?`<div class="ml-3 pl-2 border-l border-line">${ts.map(t=>`<button class="topic-row ${state.topicId===t.id?'active':''}" data-action="topic" data-id="${escape(t.id)}" ${state.topicId===t.id?'aria-current="true"':''}><span class="topic-code">${escape(t.id)}</span><span class="topic-text">${escape(t.title)}</span><span class="topic-status">${p.reviewed.includes(t.id)?icon('check'):p.bookmarks.includes(t.id)?icon('star'):''}</span></button>`).join('')}</div>`:''}</div>`;
    }).join('')||`<div class="text-center text-[12px] text-muted px-5 py-8 leading-relaxed">${c.topics.length?'No topics match these filters.':'Your personal topics will appear here.'}</div>`;
  }
  function renderSidebarProgress() {
    const s=stats(),t=totals();
    $('#sidebar-progress').innerHTML=`<div class="flex items-center justify-between mb-2"><span class="text-[11px] font-semibold text-ink">Your progress</span><span class="text-[11px] font-semibold text-brand">${fraction(s.reviewed,t.topics)}%</span></div><div class="progress-track" role="progressbar" aria-label="Topics reviewed" aria-valuenow="${s.reviewed}" aria-valuemin="0" aria-valuemax="${Math.max(t.topics,1)}"><div class="progress-fill" style="width:${fraction(s.reviewed,t.topics)}%"></div></div><div class="flex items-center justify-between mt-2"><span class="text-[10px] text-muted">${s.reviewed} of ${t.topics} topics reviewed</span><span class="text-muted icon-sm" title="Stored in this browser">${icon('lock')}</span></div>`;
  }
  function renderDrawer() {$('#sidebar').classList.toggle('open',state.sidebarOpen);$('#drawer-overlay').classList.toggle('show',state.sidebarOpen);}
  function bookmarkButton(t) {const saved=progress().bookmarks.includes(t.id);return `<button class="icon-button ${saved?'active':''}" data-action="bookmark" aria-label="${saved?'Remove bookmark':'Bookmark this topic'}" aria-pressed="${saved}" title="${saved?'Remove bookmark':'Bookmark topic'}">${icon('star')}</button>`;}
  function topicBadge(t) {if(isBook())return bookTopicBadge(t);return `<span class="pill bg-brand-soft text-brand">DOMAIN ${escape(t.domain)}</span><span class="text-[10px] text-muted">${escape(t.id)}</span>`;}
  function renderWorkspace() {
    labBeforeRender();
    if(state.mode==='coding'){$('#workspace').innerHTML=`<div class="coding-workspace">${renderCodingLab()}</div>`;labRefresh();return;}
    if(!course().topics.length){$('#workspace').innerHTML=`<section class="panel empty fade-in">${icon('book')}<h2>Make this space yours.</h2><p>Add your own topic, question-and-answer cards, and a practice scenario. Or import a study pack to start another curriculum.</p><div class="flex flex-wrap gap-3 justify-center"><button class="btn btn-primary" data-action="add-topic">${icon('plus')} Add a topic</button><button class="btn" data-action="import">${icon('upload')} Import study pack</button></div></section>`;return;}
    if(!visibleTopics().length){$('#workspace').innerHTML=`<section class="panel empty fade-in">${icon('search')}<h2>No matching topics.</h2><p>Try another search, select all domains, or clear the saved-topic filter.</p><button class="btn btn-primary" data-action="clear-filters">Clear filters</button></section>`;return;}
    const t=topic();
    const content=state.mode==='book'?renderSeriesBook():course().series&&state.mode==='code'?renderSeriesCode(t):course().series&&state.mode==='scenarios'?renderSeriesPractice():course().series&&state.mode==='uml'?renderSeriesUML(t):state.mode==='lectures'?renderLectures(t):state.mode==='uml'?renderUML(t):state.mode==='code'?renderCode(t):state.mode==='outline'?(t.kind==='glossary'?renderGlossaryOutline(t):renderOutline(t)):state.mode==='flashcards'?renderFlashcards():renderScenario();
    $('#workspace').innerHTML=`<div class="content-grid fade-in ${(course().series||['uml','code','lectures'].includes(state.mode)||(state.mode==='outline'&&chapterGuide()))?'wide-workspace':''}"><section class="min-w-0">${content}</section>${course().series||(state.mode==='outline'&&chapterGuide())?'':`<aside class="right-rail" aria-label="Topic support and progress">${renderRail(t)}</aside>`}</div>`;
  }
  function sourceLine(t) {if(isBook())return bookSourceLine(t);const u=safeURL(t.source);return `<div class="flex items-center justify-between gap-3 flex-wrap mt-4"><span class="text-[9px] text-muted">${course().id==='design-patterns-cpp'?'Original supplemental notes · Topic order from your supplied index':'User-authored study material'}</span>${u?`<a class="source-link" href="${escape(u)}" target="_blank" rel="noopener noreferrer">Linked topic article ${icon('external')}</a>`:''}</div>`;}
  function renderLegacyOutline(t) {
    const list=visibleTopics(),index=list.findIndex(x=>x.id===t.id),read=progress().reviewed.includes(t.id);
    return `<article class="panel"><div class="lesson-inner"><div class="flex items-center justify-between gap-3"><div class="flex items-center gap-2 flex-wrap">${topicBadge(t)}<span class="text-[10px] text-muted">· &nbsp; Topic ${index+1} of ${list.length}</span></div>${bookmarkButton(t)}</div><h2 class="lesson-heading">${escape(t.title)}</h2><p class="reading mb-5">${escape(t.summary)}</p><div class="flex items-center gap-2 mt-6 mb-1"><span class="eyebrow">The key ideas</span><div class="h-px flex-1 bg-line"></div></div><div>${t.concepts.map((c,i)=>`<section class="concept" id="concept-${i}"><span class="concept-number">${String(i+1).padStart(2,'0')}</span><div><h3>${escape(c.term)}</h3><p>${escape(c.definition)}</p></div></section>`).join('')}</div>${renderBlocks(t.blocks)}${t.takeaway?`<div class="takeaway"><span class="icon-sm pt-0.5">${icon('bulb')}</span><div><span class="text-[11px] font-semibold text-brand">Keep this distinction in mind</span><p>${escape(t.takeaway)}</p></div></div>`:''}${sourceLine(t)}</div><div class="lesson-bottom border-t border-line px-6 py-4 flex items-center justify-between gap-2"><button class="btn" data-action="previous" ${index<=0?'disabled':''}>${icon('left')} Previous</button><button class="btn ${read?'btn-success':'btn-primary'}" data-action="reviewed" aria-pressed="${read}">${icon('check')} ${read?'Reviewed':'Mark reviewed'}</button><button class="btn" data-action="next" ${index>=list.length-1?'disabled':''}>Next ${icon('right')}</button></div></article><div class="flex items-center justify-between gap-3 mt-3 px-1"><span class="text-[10px] text-muted">Move at your own pace. <kbd class="key">←</kbd> <kbd class="key">→</kbd> to navigate</span>${course().id==='design-patterns-cpp'||isBook()?`<button class="source-link" data-action="mode" data-mode="flashcards">Try this topic’s flashcards ${icon('right')}</button>`:`<button class="source-link" data-action="edit-topic">Edit this topic ${icon('edit')}</button>`}</div>`;
  }
  function scopeControl() {return `<select id="scope-select" class="select-small" aria-label="Choose practice scope">${[['all','All filtered topics'],['topic','This topic only'],...(isBook()?[['chapter','This chapter only']]:[]),['domain','This topic’s family'],['bookmarked','Bookmarked topics'],['review','Needs another look']].map(([id,text])=>`<option value="${id}" ${state.scope===id?'selected':''}>${text}</option>`).join('')}</select>`;}
  function deckEmpty() {return `<section class="panel"><div class="p-5 flex justify-end">${scopeControl()}</div><div class="empty">${icon('layers')}<h2>Nothing in this practice set yet.</h2><p>${state.scope==='review'?'Cards marked “Review again” and scenarios answered incorrectly appear here.':'Try all filtered topics, bookmark a topic, or add material to this topic.'}</p><button class="btn btn-primary" data-action="all-practice">Study all filtered topics</button>${course().id!=='design-patterns-cpp'&&!isBook()?'<button class="btn ml-2" data-action="edit-topic">Edit topic</button>':''}</div></section>`;}
  function renderFlashcards() {
    const list=deck(),item=activeItem();if(!item)return deckEmpty();const i=list.findIndex(c=>c.id===item.id),p=progress(),rating=p.cards[item.id]?.rating;
    return `<div class="panel p-5 sm:p-6"><div class="flex items-center justify-between gap-2 flex-wrap mb-5">${scopeControl()}<button class="btn btn-text" data-action="shuffle" aria-label="Shuffle flashcards">${icon('shuffle')} Shuffle</button></div><div class="flex items-center justify-between gap-3 mb-4"><div class="flex items-center gap-2 flex-wrap">${topicBadge(item.topic)}<span class="text-[10px] text-muted">${escape(item.topic.title)}</span></div>${bookmarkButton(item.topic)}</div><div class="relative z-0"><button class="deck-card ${state.flipped?'answer':''}" data-action="flip" aria-label="${state.flipped?'Answer shown. Activate to show question.':'Flashcard question. Activate to reveal answer.'}" aria-pressed="${state.flipped}"><span class="flex items-center justify-between w-full"><span class="eyebrow">${state.flipped?'THE ANSWER':'YOUR QUESTION'}</span><span class="text-[11px] text-muted">${i+1} <span class="opacity-50">/ ${list.length}</span></span></span><span class="${state.flipped?'card-answer':'card-question'}">${escape(state.flipped?item.answer:item.question)}</span>${!state.flipped&&item.code?`<span class="flashcard-code"><code>${escape(item.code)}</code></span>${item.sampleInput!==undefined?`<span class="flashcard-input">Sample stdin: ${escape(item.sampleInput||'(empty)')}</span>`:''}`:''}<span class="flip-hint">${icon('rotate')} ${state.flipped?'Click to see the question':'Think it through. Click to reveal.'}</span></button></div><div class="text-center text-[10px] text-muted mt-5 mb-4">${rating==='known'?'You marked this card as known.':rating==='again'?'You marked this card for another look.':'Give yourself a moment before revealing the answer.'} &nbsp; <kbd class="key">Space</kbd> to flip</div><div class="deck-actions flex items-center justify-center gap-3 mb-5">${state.flipped?`<button class="btn" data-action="rate-card" data-rating="again">${icon('rotate')} Review again</button><button class="btn btn-primary" data-action="rate-card" data-rating="known">${icon('check')} Got it</button>`:`<button class="btn btn-primary px-7" data-action="flip">${icon('layers')} Reveal answer</button>`}</div><div class="border-t border-line pt-4 flex justify-between items-center gap-3"><button class="btn" data-action="previous" ${i<=0?'disabled':''}>${icon('left')} Previous</button><span class="text-[10px] text-muted">${list.filter(c=>p.cards[c.id]?.rating==='known').length} / ${list.length} known in this set</span><button class="btn" data-action="next" ${i>=list.length-1?'disabled':''}>Next ${icon('right')}</button></div></div>${item.labId?`<p class="flashcard-lab-link"><button class="btn" data-action="lab-open-question" data-id="${escape(item.labId)}">Open this Coding Lab challenge ${icon('right')}</button></p>`:''}<p class="text-[10px] text-muted text-center mt-3">${course().series?'Source review, recall, tracing, and diagnosis cards.':'Original recall cards.'} Confidence ratings are self-assessments, not proof of mastery.</p>`;
  }
  function renderScenario() {
    const list=deck(),item=activeItem();if(!item)return deckEmpty();const i=list.findIndex(c=>c.id===item.id);
    const result=state.retries.has(item.id)?null:progress().scenarios[item.id];const selected=result?result.choice:state.drafts[item.id];
    return `<article class="panel"><div class="lesson-inner"><div class="flex justify-between items-center gap-2 flex-wrap mb-5">${scopeControl()}<button class="btn btn-text" data-action="shuffle" aria-label="Shuffle scenarios">${icon('shuffle')} Shuffle</button></div><div class="flex items-center justify-between gap-3"><div class="flex items-center gap-2 flex-wrap">${topicBadge(item.topic)}<span class="text-[10px] text-muted">· &nbsp; Case ${i+1} of ${list.length}</span></div>${bookmarkButton(item.topic)}</div><h2 class="lesson-heading text-[25px]">A design decision. A concrete consequence.</h2><p class="text-[11px] text-muted mb-4">${escape(item.topic.title)} · Original hypothetical scenario</p><p class="reading text-ink mb-5">${escape(item.prompt)}</p><div role="group" aria-label="Answer choices">${item.options.map((option,n)=>{
      const cls=result?(n===item.correctIndex?'correct':n===selected?'incorrect':''):(n===selected?'selected':'');
      return `<button class="choice ${cls}" data-action="choose-answer" data-index="${n}" aria-pressed="${n===selected}" ${result?'disabled':''}><span class="choice-letter">${String.fromCharCode(65+n)}</span><span class="flex-1">${escape(option)}</span>${result&&n===item.correctIndex?`<span class="icon-sm">${icon('check')}</span>`:result&&n===selected?`<span class="icon-sm">${icon('close')}</span>`:''}</button>`;
    }).join('')}</div>${result?`<div class="feedback ${result.correct?'':'incorrect'}" role="status"><div class="flex items-center gap-2"><span class="icon-sm ${result.correct?'text-mint-ink':'text-peach-ink'}">${icon(result.correct?'checkCircle':'bulb')}</span><strong class="text-[13px] ${result.correct?'text-mint-ink':'text-peach-ink'}">${result.correct?'That’s right.':'Not quite. Here’s the distinction.'}</strong></div><p>${escape(item.explanation)}</p>${item.rationales?`<div class="option-rationales"><h3>Why each option does or does not fit</h3>${item.rationales.map((r,n)=>`<p><strong>${String.fromCharCode(65+n)}${n===item.correctIndex?' · Correct':''}.</strong> ${escape(r)}</p>`).join('')}</div>`:''}<div class="flex justify-between gap-2 mt-3"><span class="text-[10px] text-muted">Correct answer: ${String.fromCharCode(65+item.correctIndex)} · Attempt ${result.attempts}</span><button class="source-link" data-action="retry-scenario">Try again ${icon('rotate')}</button></div></div>`:''}</div><div class="lesson-bottom border-t border-line px-6 py-4 flex items-center justify-between gap-2"><button class="btn" data-action="previous" ${i<=0?'disabled':''}>${icon('left')} Previous</button>${result?`<button class="btn btn-primary" data-action="next" ${i>=list.length-1?'disabled':''}>Next scenario ${icon('right')}</button>`:`<button class="btn btn-primary" data-action="check-answer" ${Number.isInteger(selected)?'':'disabled'}>Check answer ${icon('check')}</button>`}</div></article><p class="text-[10px] text-muted text-center mt-3">Practice reasoning, by following the actual code. <kbd class="key">1–4</kbd> choose · <kbd class="key">Enter</kbd> check</p>`;
  }
  function renderRail(t) {
    if(!t)return '';
    const p=progress(),s=stats(),count=totals(),read=p.reviewed.includes(t.id),known=t.cards.filter(c=>p.cards[c.id]?.rating==='known').length,answered=t.scenario&&p.scenarios[t.scenario.id];
    return `<section class="panel p-5 study-loop-panel"><div class="flex items-center gap-2 mb-4"><span class="text-brand icon-sm">${icon('target')}</span><h3 class="text-[12px] font-semibold">Your study loop</h3></div><p class="text-[10px] text-muted leading-relaxed mb-2">${escape(t.id)} · ${escape(t.title)}</p>${[[read,'Read & understand',read?'Topic marked reviewed':'Explore the key ideas','outline'],[known===t.cards.length&&t.cards.length>0,'Recall the concept',`${known} of ${t.cards.length} cards known`,'flashcards'],[!!answered,'Apply what you know',answered?(answered.correct?'Latest response correct':'Revisit this scenario'):(t.scenario?'Try the practice scenario':'No scenario added'),'scenarios']].map(([done,title,sub,m],i)=>`<button class="rail-row w-full text-left" data-action="mode" data-mode="${m}"><span class="rail-step ${done?'done':''}">${done?icon('check'):i+1}</span><span><span class="block font-medium ${state.mode===m?'text-brand':'text-ink'}">${title}</span><span class="block text-[9px] mt-0.5 text-muted">${sub}</span></span></button>`).join('')}<div class="border-t border-line mt-3 pt-3 flex justify-between gap-2"><span class="text-[10px] text-muted">Topics reviewed</span><span class="text-[10px] text-brand font-semibold">${s.reviewed} / ${count.topics}</span></div><div class="progress-track mt-2"><div class="progress-fill" style="width:${fraction(s.reviewed,count.topics)}%"></div></div></section>
      <section class="panel p-5 mt-4"><div class="flex items-center gap-2 mb-3"><span class="text-muted icon-sm">${icon('edit')}</span><label for="personal-note" class="text-[12px] font-semibold">Make it your own</label></div><textarea class="notes-area" id="personal-note" data-topic="${escape(t.id)}" placeholder="An example, a memory cue, or something to revisit…" maxlength="20000" aria-label="Personal notes for this topic">${escape(p.notes[t.id]||'')}</textarea><div class="text-[9px] text-muted mt-2 flex items-center gap-1 icon-sm">${icon('lock')} Private to this browser</div></section>
      <section class="rounded-xl bg-brand-soft p-5 mt-4 rail-tip"><span class="text-brand icon-sm">${icon('bulb')}</span><h3 class="text-[12px] font-semibold text-brand mt-2 mb-2">A useful little habit</h3><p class="text-[11px] leading-loose text-muted">${state.mode==='outline'?'After reading, explain the idea without looking. Then invent your own example.':state.mode==='flashcards'?'Say your answer before flipping. Mark uncertain cards for another look.':'Identify the relevant contract before choosing an answer. Read the explanation, even when you’re right.'}</p></section>
      ${state.mode==='scenarios'&&s.attempted?`<section class="panel p-5 mt-4"><span class="eyebrow">First-attempt accuracy</span><div class="text-[26px] font-semibold text-brand mt-2">${s.accuracy}%</div><p class="text-[10px] text-muted mt-1">Across ${s.attempted} attempted scenarios. Not an exam prediction.</p></section>`:''}`;
  }

  async function chooseTopic(id) {
    if(!course().topics.some(t=>t.id===id))return;
    state.topicId=id;state.cardId=null;state.scenarioId=null;state.flipped=false;state.expanded.add(topic().domain);state.expanded.add('CH'+(topic()?.chapter??1));state.sidebarOpen=false;
    await renderAll();window.scrollTo({top:0,behavior:'instant'});
  }
  function switchMode(mode) {
    if(!availableModes().some(m=>m.id===mode))return;
    const current=topic();state.mode=mode;state.flipped=false;
    const items=deck(mode);const key=mode==='flashcards'?'cardId':'scenarioId';
    if(['flashcards','scenarios'].includes(mode)&&!items.some(i=>i.id===state[key]&&i.topic.id===current?.id))state[key]=items.find(i=>i.topic.id===current?.id)?.id||null;
    renderAll();
  }
  function navigate(delta) {
    if(state.mode==='book')return;
    if(['uml','code','lectures'].includes(state.mode)){const chapters=course().chapters.filter(c=>c.number>0);const i=chapters.findIndex(c=>c.number===topic()?.chapter);const next=chapters[i+delta];if(next)goChapter(next.number);return;}
    const list=state.mode==='outline'?readingTopics():deck();
    const current=state.mode==='outline'?state.topicId:activeItem()?.id;
    const index=list.findIndex(x=>x.id===current);const next=index+delta;
    if(index<0||next<0||next>=list.length)return;
    const item=list[next];state.flipped=false;state.reviewHoldId=null;
    if(state.mode==='outline'){state.lessonView='lesson';state.topicId=item.id;state.cardId=null;state.scenarioId=null;}
    else {state.topicId=item.topic.id;if(state.mode==='flashcards')state.cardId=item.id;else state.scenarioId=item.id;}
    state.expanded.add(topic().domain);state.expanded.add('CH'+(topic()?.chapter??1));renderAll();
    if(state.mode==='outline'||state.mode==='scenarios')window.scrollTo({top:0,behavior:'instant'});
  }
  function clearFilters() {state.chapter='all';state.task='all';state.query='';state.domain='all';state.bookmarksOnly=false;state.cardId=null;state.scenarioId=null;renderAll();}
  function shuffle() {
    const items=deck();if(items.length<2){toast('Add more items to shuffle this set.');return;}
    const ids=items.map(i=>i.id);
    for(let i=ids.length-1;i>0;i--){const j=Math.floor(Math.random()*(i+1));[ids[i],ids[j]]=[ids[j],ids[i]];}
    state.orders[state.mode]=ids;const item=items.find(i=>i.id===ids[0]);
    if(state.mode==='flashcards')state.cardId=item.id;else state.scenarioId=item.id;
    state.topicId=item.topic.id;state.flipped=false;renderAll();toast('Practice set shuffled. Your progress is unchanged.');
  }
  function flipCard() {
    if(state.mode!=='flashcards')return;
    state.flipped=!state.flipped;renderWorkspace();
    const card=$('.deck-card');if(card)card.focus({preventScroll:true});
  }
  function rateCard(rating) {
    if(state.mode!=='flashcards'||!state.flipped||!['known','again'].includes(rating))return;
    const before=deck(),item=activeItem();if(!item)return;const index=before.findIndex(c=>c.id===item.id);
    progress().cards[item.id]={rating,updatedAt:new Date().toISOString()};state.flipped=false;
    const after=deck();const nextIndex=state.scope==='review'&&rating==='known'?Math.min(index,after.length-1):Math.min(index+1,after.length-1);
    if(after[nextIndex]){state.cardId=after[nextIndex].id;state.topicId=after[nextIndex].topic.id;}
    renderAll();toast(rating==='known'?'Marked as known.':'Saved for another look.');
  }
  function checkAnswer() {
    if(state.mode!=='scenarios'||course().series)return;const item=activeItem();if(!item)return;
    const old=progress().scenarios[item.id];if(old&&!state.retries.has(item.id))return;
    const choice=state.drafts[item.id];if(!Number.isInteger(choice)||choice<0||choice>=item.options.length)return;
    const correct=choice===item.correctIndex;
    progress().scenarios[item.id]={choice,correct,attempts:(old?.attempts||0)+1,firstCorrect:old?old.firstCorrect:correct,updatedAt:new Date().toISOString()};
    state.retries.delete(item.id);state.reviewHoldId=item.id;renderAll();
  }
  function modal(title,html) {DIALOG.classList.remove('wide-dialog');DIALOG.innerHTML=`<div class="dialog-head"><h2 id="dialog-title" class="text-lg font-semibold tracking-tight">${escape(title)}</h2><button class="icon-button" data-action="close-dialog" aria-label="Close dialog">${icon('close')}</button></div><div class="dialog-body">${html}</div>`;if(!DIALOG.open)DIALOG.showModal();}
  function showSources() {if(isBook())showBookSources();else modal('Your study material',`<p>${escape(course().provenance||'Personal study topics, flashcards, and scenarios saved in this browser.')}</p><p>Export a backup to keep your notes and progress. Export a study pack to share only the curriculum.</p>`);}
  function showSettings() {
    const s=stats(),t=totals();
    modal('Your workspace',`
      <div class="grid grid-cols-3 gap-3 mb-6"><div class="rounded-xl bg-brand-soft p-4"><div class="text-xl text-brand font-semibold">${s.reviewed}<span class="text-xs font-normal"> / ${t.topics}</span></div><div class="text-[10px] text-muted mt-1">Topics reviewed</div></div><div class="rounded-xl bg-peach p-4"><div class="text-xl text-peach-ink font-semibold">${s.known}</div><div class="text-[10px] text-muted mt-1">Cards known</div></div><div class="rounded-xl bg-mint p-4"><div class="text-xl text-mint-ink font-semibold">${s.attempted}</div><div class="text-[10px] text-muted mt-1">${course().series?'Prompts self-reviewed':'Scenarios attempted'}</div></div></div>
      <h3>Keep a copy of your work</h3><p>Backups include your progress, bookmarks, personal notes, coding drafts, and custom curricula. A study-pack export contains the current curriculum but not your private progress or notes.</p><div class="flex gap-3 flex-wrap mt-3"><button class="btn btn-primary" data-action="export-backup">${icon('download')} Export backup</button><button class="btn" data-action="export-pack">${icon('download')} Export study pack</button><button class="btn" data-action="import">${icon('upload')} Import / restore</button></div>
      <h3>Add another subject</h3><p>Select “My study material” and add topics, cards, and scenarios. Or import a study-pack JSON file; it appears in the curriculum dropdown. The included <code>docs/example-study-pack.json</code> shows the format.</p><button class="btn" data-action="add-topic">${icon('plus')} Add your own topic</button>
      <h3>Keyboard shortcuts</h3><div class="grid grid-cols-2 gap-3 text-[12px] text-muted"><span><kbd class="key">←</kbd> <kbd class="key">→</kbd> Previous / next</span><span><kbd class="key">Space</kbd> Flip a flashcard</span><span><kbd class="key">O</kbd> <kbd class="key">F</kbd> <kbd class="key">S</kbd> Change mode</span><span><kbd class="key">/</kbd> Search topics</span><span><kbd class="key">1–4</kbd> Choose scenario answer</span><span><kbd class="key">Enter</kbd> Check scenario answer</span></div>
      <h3>Reset this curriculum’s progress</h3><p>This clears review marks, card confidence ratings, and scenario attempts for the selected curriculum. Bookmarks, personal notes, coding drafts, and custom material are kept.</p><button class="btn btn-danger" data-action="reset-progress">${icon('rotate')} Reset learning progress</button>`);
  }
  function downloadJSON(filename,data) {
    const blob=new Blob([JSON.stringify(data,null,2)],{type:'application/json'});const url=URL.createObjectURL(blob);const a=document.createElement('a');a.href=url;a.download=filename;document.body.append(a);a.click();a.remove();setTimeout(()=>URL.revokeObjectURL(url),1500);
  }
  function exportBackup() {remember();downloadJSON(`study-studio-backup-${new Date().toISOString().slice(0,10)}.json`,{type:'study-studio-backup',version:1,savedAt:new Date().toISOString(),data:store});toast('Backup exported. Keep it somewhere safe.');}
  async function exportPack() {const current=course();const c=Library&&current.catalog?await Library.completeCourse(current):{...current};seriesCache.delete(current);if(isBook()){delete c.reader;delete c.pageImages;}downloadJSON(`${course().id}-study-pack.json`,{schemaVersion:1,courses:[c]});toast('Study pack exported without private progress or notes.');}

  function showEditor(edit=false) {
    if(course().id==='design-patterns-cpp'||isBook()){
      state.courseId='my-material';state.query='';state.domain='all';state.bookmarksOnly=false;state.scope='all';state.cardId=null;state.scenarioId=null;restorePosition();renderAll();edit=false;
    }
    const t=edit?topic():null;const sc=t?.scenario;
    modal(t?'Edit your topic':'Add your study material',`
      <p class="mb-5">Create a topic in <strong>${escape(course().title)}</strong>. Enter your own material. Cards and scenarios are optional and can be added later.</p>
      <form id="topic-form" data-edit-id="${escape(t?.id||'')}" class="form-grid">
        <div class="form-full"><label class="field-label" for="edit-title">Topic title *</label><input class="field" id="edit-title" name="title" required maxlength="220" placeholder="e.g., Ownership and borrowed references" value="${escape(t?.title||'')}"></div>
        <div class="form-full"><label class="field-label" for="edit-summary">Overview *</label><textarea class="field" id="edit-summary" name="summary" required rows="3" maxlength="15000" placeholder="The core idea, in your own words.">${escape(t?.summary||'')}</textarea></div>
        <div class="form-full"><label class="field-label" for="edit-concepts">Key ideas · one per line, using Term | Definition</label><textarea class="field" id="edit-concepts" name="concepts" rows="4" maxlength="40000" placeholder="Invariant | A rule that must remain true at a stated program boundary.">${escape(t?.concepts.map(x=>`${x.term} | ${x.definition}`).join('\n')||'')}</textarea><p class="!text-[10px] !mt-1">Each key idea also becomes a question-and-answer flashcard.</p></div>
        <div class="form-full"><label class="field-label" for="edit-takeaway">Memory cue or key distinction</label><input class="field" id="edit-takeaway" name="takeaway" maxlength="3000" value="${escape(t?.takeaway||'')}" placeholder="What is easy to confuse here?"></div>
        <div class="form-full"><label class="field-label" for="edit-source">Optional source URL</label><input class="field" id="edit-source" name="source" type="url" maxlength="2000" value="${escape(t?.source||'')}" placeholder="https://..."></div>
        <div class="form-full"><h3 class="!mt-0">An extra flashcard</h3></div>
        <div><label class="field-label" for="edit-question">Question</label><textarea class="field" id="edit-question" name="question" rows="3" maxlength="6000" placeholder="Your question">${escape(t?.cards.find(c=>c.id.endsWith('-extra'))?.question||'')}</textarea></div>
        <div><label class="field-label" for="edit-answer">Answer</label><textarea class="field" id="edit-answer" name="answer" rows="3" maxlength="12000" placeholder="Your answer">${escape(t?.cards.find(c=>c.id.endsWith('-extra'))?.answer||'')}</textarea></div>
        <div class="form-full"><h3 class="!mt-0">A practice scenario</h3><label class="field-label" for="edit-scenario">Scenario and question</label><textarea class="field" id="edit-scenario" name="scenario" rows="3" maxlength="12000" placeholder="Describe a situation, then ask a question.">${escape(sc?.prompt||'')}</textarea></div>
        ${[0,1,2,3].map(i=>`<div><label class="field-label" for="edit-option-${i}">Option ${String.fromCharCode(65+i)}</label><input class="field" id="edit-option-${i}" name="option${i}" maxlength="3000" value="${escape(sc?.options[i]||'')}"></div>`).join('')}
        <div><label class="field-label" for="edit-correct">Correct answer</label><select class="field" id="edit-correct" name="correct">${[0,1,2,3].map(i=>`<option value="${i}" ${sc?.correctIndex===i?'selected':''}>${String.fromCharCode(65+i)}</option>`).join('')}</select></div>
        <div><label class="field-label" for="edit-explanation">Why is it correct?</label><textarea class="field" id="edit-explanation" name="explanation" rows="3" maxlength="12000">${escape(sc?.explanation||'')}</textarea></div>
        <div class="form-full text-sm text-red-700" id="form-error" role="alert"></div>
        <div class="form-full flex justify-between gap-3"><button type="button" class="btn" data-action="close-dialog">Cancel</button><button class="btn btn-primary" type="submit">${icon('check')} Save topic</button></div>
      </form>`);
  }
  function saveTopic(form) {
    const f=new FormData(form),get=n=>String(f.get(n)||'').trim();const original=course().topics.find(t=>t.id===form.dataset.editId);
    try {
      const concepts=get('concepts').split('\n').filter(x=>x.trim()).map(line=>{const i=line.indexOf('|');if(i<1||!line.slice(i+1).trim())throw new Error('Use Term | Definition for each key-idea line.');return {term:line.slice(0,i).trim(),definition:line.slice(i+1).trim()};});
      if(concepts.length>60)throw new Error('Use no more than 60 key ideas per topic.');
      if(!get('title')||!get('summary'))throw new Error('Add a title and an overview.');
      if(get('source')&&!safeURL(get('source')))throw new Error('Use an http:// or https:// source URL.');
      if(Boolean(get('question'))!==Boolean(get('answer')))throw new Error('Provide both a question and an answer for the extra card.');
      const id=original?.id||`U.${Date.now().toString(36)}`;
      const cards=concepts.map((c,i)=>({id:`${id}-c${i+1}`,question:`What is meant by ${c.term.toLowerCase()}?`,answer:c.definition}));
      for(const c of original?.cards||[]){if(!c.id.startsWith(`${id}-c`)&&!c.id.endsWith('-extra'))cards.push(c);}
      if(get('question'))cards.push({id:`${id}-extra`,question:get('question'),answer:get('answer')});
      if(!cards.length)cards.push({id:`${id}-c1`,question:`What is the key idea of ${get('title')}?`,answer:get('summary')});
      let scenario=null;const options=[0,1,2,3].map(i=>get(`option${i}`));
      const anyScenario=get('scenario')||options.some(Boolean)||get('explanation');
      if(anyScenario){if(!get('scenario')||options.some(x=>!x)||!get('explanation'))throw new Error('A scenario needs a prompt, four options, and an explanation.');if(new Set(options).size!==4)throw new Error('Scenario options must be distinct.');scenario={id:`${id}-s1`,prompt:get('scenario'),options,correctIndex:Number(get('correct')),explanation:get('explanation')};}
      const t={id,domain:original?.domain||course().domains[0].id,title:get('title'),summary:get('summary'),concepts,takeaway:get('takeaway'),source:get('source'),origin:'user-authored',cards,scenario};
      const target=state.courseId==='my-material'?store.customTopics:store.courses.find(c=>c.id===state.courseId).topics;
      const index=target.findIndex(x=>x.id===id);if(index>=0)target[index]=t;else target.push(t);
      // Changed learning items should not inherit potentially stale correctness/confidence records.
      const p=progress();for(const c of original?.cards||[])delete p.cards[c.id];if(original?.scenario)delete p.scenarios[original.scenario.id];p.reviewed=p.reviewed.filter(x=>x!==id);
      state.topicId=id;state.cardId=null;state.scenarioId=null;state.mode='outline';state.scope='all';state.query='';state.domain='all';state.bookmarksOnly=false;
      DIALOG.close();renderAll();toast('Your topic is ready in all three study modes.');
    } catch(error){$('#form-error',form).textContent=error.message;}
  }

  function text(value,max=20000,fallback='') {if(typeof value!=='string')return fallback;if(value.length>max)throw new Error(`A text field exceeds the ${max}-character limit.`);return value;}
  function validId(id) {return typeof id==='string'&&id.length>0&&id.length<=100&&/^[A-Za-z0-9][A-Za-z0-9_.-]*$/.test(id)&&!['constructor','prototype','__proto__'].includes(id)&&!Object.prototype.hasOwnProperty.call(Object.prototype,id);}
  function validateTopic(t,domains) {
    if(!isObject(t)||!validId(t.id)||!domains.has(t.domain))throw new Error('Each topic needs a unique safe ID and an existing domain.');
    const title=text(t.title,220),summary=text(t.summary,15000);if(!title.trim()||!summary.trim())throw new Error('Each topic needs a title and summary.');
    const concepts=Array.isArray(t.concepts)?t.concepts:[];if(concepts.length>60)throw new Error('Too many key ideas in a topic.');
    const cleanConcepts=concepts.map(c=>{if(!isObject(c)||typeof c.term!=='string'||typeof c.definition!=='string')throw new Error('Key ideas require a term and definition.');return {term:text(c.term,400),definition:text(c.definition,15000)};});
    const cards=Array.isArray(t.cards)?t.cards:[];if(cards.length>1000)throw new Error('Too many cards in a topic.');
    const ids=new Set();const cleanCards=cards.map(c=>{if(!isObject(c)||!validId(c.id)||ids.has(c.id)||typeof c.question!=='string'||!c.question.trim()||typeof c.answer!=='string'||!c.answer.trim())throw new Error('Flashcards require distinct IDs, questions, and answers.');ids.add(c.id);return {id:c.id,question:text(c.question,12000),answer:text(c.answer,20000),pages:cleanPages(c.pages),origin:text(c.origin,100),glossaryId:validId(c.glossaryId)?c.glossaryId:undefined};});
    let scenario=null;
    if(t.scenario){const s=t.scenario;if(!isObject(s)||!validId(s.id)||!Array.isArray(s.options)||s.options.length!==4||s.options.some(o=>typeof o!=='string'||!o.trim())||new Set(s.options).size!==4||!Number.isInteger(s.correctIndex)||s.correctIndex<0||s.correctIndex>3||typeof s.prompt!=='string'||!s.prompt.trim()||typeof s.explanation!=='string'||!s.explanation.trim())throw new Error('A scenario needs an ID, a prompt, four distinct options, correctIndex 0–3, and an explanation.');scenario={id:s.id,prompt:text(s.prompt,15000),options:s.options.map(o=>text(o,3000)),correctIndex:s.correctIndex,explanation:text(s.explanation,20000),rationales:Array.isArray(s.rationales)&&s.rationales.length===4?s.rationales.map(r=>text(r,6000)):undefined};}
    return {id:t.id,title,summary,domain:t.domain,concepts:cleanConcepts,cards:cleanCards,scenario,takeaway:text(t.takeaway,6000),source:safeURL(t.source),origin:'user-imported',chapter:Number.isInteger(t.chapter)?t.chapter:undefined,chapterTitle:text(t.chapterTitle,400),section:Number.isInteger(t.section)?t.section:undefined,part:Number.isInteger(t.part)?t.part:undefined,pages:cleanPages(t.pages),tasks:Array.isArray(t.tasks)?t.tasks.filter(x=>/^[A-I]\.\d{1,2}$/.test(x)).slice(0,104):[],domains:Array.isArray(t.domains)?t.domains.filter(validId).slice(0,10):[],kind:t.kind==='glossary'?'glossary':undefined,sourceNote:text(t.sourceNote,6000),sourceRef:text(t.sourceRef,1000),blocks:cleanBlocks(t.blocks),seriesKind:text(t.seriesKind,40),practice:cleanSeriesPractice(t.practice),slide:Number.isInteger(t.slide)?t.slide:undefined,reading:isObject(t.reading)?t.reading:undefined,sectionId:validId(t.sectionId)?t.sectionId:undefined,outlineParent:validId(t.outlineParent)?t.outlineParent:undefined,sourceCompanions:Array.isArray(t.sourceCompanions)?t.sourceCompanions.filter(validId):[]};
  }
  function validateCourse(c) {
    if(!isObject(c)||!validId(c.id)||typeof c.title!=='string'||!c.title.trim()||!Array.isArray(c.domains)||!c.domains.length||c.domains.length>40||!Array.isArray(c.topics)||c.topics.length>10000)throw new Error('A study pack requires an ID, title, 1–40 domains, and up to 10,000 topics.');
    const ids=new Set();const domains=c.domains.map(d=>{if(!isObject(d)||!validId(d.id)||ids.has(d.id)||typeof d.title!=='string'||!d.title.trim())throw new Error('Domain IDs must be distinct and each domain needs a title.');ids.add(d.id);return {id:d.id,title:text(d.title,300)};});
    const topics=c.topics.map(t=>validateTopic(t,ids));const unique=new Set();const itemIds=new Set();
    for(const t of topics){if(unique.has(t.id))throw new Error('Topic IDs must be unique within a curriculum.');unique.add(t.id);for(const x of [...t.cards,...(t.scenario?[t.scenario]:[]),...(t.practice||[])]){if(itemIds.has(x.id))throw new Error('Practice-item IDs must be unique within a curriculum.');itemIds.add(x.id);}}
    return {id:c.id,title:text(c.title,200),subtitle:text(c.subtitle,300,'Imported study material'),description:text(c.description,1000,'Your imported study pack.'),provenance:text(c.provenance,6000,'User-imported material. Verify its accuracy and permissions.'),domains,topics,...cleanBookMetadata(c)};
  }
  function cleanProgress(p) {
    const clean=blankProgress();if(!isObject(p))return clean;
    clean.reviewed=Array.isArray(p.reviewed)?[...new Set(p.reviewed.filter(validId))]:[];
    clean.bookmarks=Array.isArray(p.bookmarks)?[...new Set(p.bookmarks.filter(validId))]:[];
    for(const [id,c] of Object.entries(isObject(p.cards)?p.cards:{}))if(validId(id)&&isObject(c)&&['again','known'].includes(c.rating))clean.cards[id]={rating:c.rating,updatedAt:text(c.updatedAt,60)};
    for(const [id,s] of Object.entries(isObject(p.scenarios)?p.scenarios:{}))if(validId(id)&&isObject(s)&&Number.isInteger(s.choice)&&s.choice>=0&&s.choice<4&&typeof s.correct==='boolean')clean.scenarios[id]={choice:s.choice,correct:s.correct,firstCorrect:typeof s.firstCorrect==='boolean'?s.firstCorrect:s.correct,attempts:Number.isInteger(s.attempts)&&s.attempts>0?Math.min(s.attempts,99999):1,updatedAt:text(s.updatedAt,60)};
    for(const [id,n] of Object.entries(isObject(p.notes)?p.notes:{}))if(validId(id)&&typeof n==='string')clean.notes[id]=text(n,20000);
    for(const [id,d] of Object.entries(isObject(p.coding)?p.coding:{}))if(validId(id)&&isObject(d)&&typeof d.code==='string'){
      clean.coding[id]={code:text(d.code,60000),input:text(d.input,20000),title:text(d.title,200),updatedAt:text(d.updatedAt,60),checkedCode:text(d.checkedCode,60000),passed:d.passed===true,checkedAt:text(d.checkedAt,60)};
      if(typeof d.initialCode==='string')clean.coding[id].initialCode=text(d.initialCode,60000);
    }
    clean.codingSelected=validId(p.codingSelected)?p.codingSelected:null;
    return clean;
  }
  function validateStore(value) {
    if(!isObject(value)||value.version!==1)throw new Error('This backup version is not supported.');
    const clean=blankStore();
    if(Array.isArray(value.customTopics)){if(value.customTopics.length>1000)throw new Error('Too many custom topics.');clean.customTopics=value.customTopics.map(t=>validateTopic(t,new Set(['1'])));}
    if(Array.isArray(value.courses)){if(value.courses.length>30)throw new Error('Too many curricula.');clean.courses=value.courses.map(validateCourse);}
    const ids=new Set(seed.courses.map(c=>c.id));for(const c of clean.courses){if(ids.has(c.id))throw new Error('Duplicate curriculum ID in backup.');ids.add(c.id);}
    for(const [id,p] of Object.entries(isObject(value.progress)?value.progress:{}))if(ids.has(id))clean.progress[id]=cleanProgress(p);
    const ui=isObject(value.ui)?value.ui:{};clean.ui.courseId=ids.has(ui.courseId)?ui.courseId:'design-patterns-cpp';clean.ui.mode=['outline','flashcards','scenarios','uml','code','lectures','book','coding'].includes(ui.mode)?ui.mode:'outline';clean.ui.large=!!ui.large;
    for(const [id,p] of Object.entries(isObject(ui.positions)?ui.positions:{}))if(ids.has(id)&&isObject(p))clean.ui.positions[id]={topicId:validId(p.topicId)?p.topicId:null,cardId:validId(p.cardId)?p.cardId:null,scenarioId:validId(p.scenarioId)?p.scenarioId:null};
    return clean;
  }
  async function importFile(file) {
    if(!file)return;
    try {
      if(file.size>25*1024*1024)throw new Error('Choose a JSON file smaller than 25 MB.');
      const data=parseJSON(await file.text());
      if(data.type==='study-studio-backup'){
        const clean=validateStore(data.data);
        if(!confirm('Restore this backup? It will replace the current saved progress, notes, bookmarks, and custom material in this browser.'))return;
        store=clean;state.courseId=store.ui.courseId;if(Library)await Library.ensureCourse(state.courseId);state.mode=store.ui.mode;state.chapter='all';state.task='all';state.query='';state.domain='all';state.scope='all';state.bookmarksOnly=false;state.orders={flashcards:[],scenarios:[]};state.drafts={};state.retries.clear();restorePosition();
        if(DIALOG.open)DIALOG.close();renderAll();toast('Backup restored.');return;
      }
      const raw=Array.isArray(data.courses)?data.courses:[data];if(!raw.length||raw.length>30)throw new Error('Import between 1 and 30 curricula.');if(store.courses.length+raw.length>30)throw new Error('This browser supports up to 30 imported curricula.');
      const incoming=raw.map(validateCourse),ids=new Set(courses().map(c=>c.id));
      for(const c of incoming){const original=c.id;let n=1;while(ids.has(c.id))c.id=`${original.slice(0,80)}-import-${n++}`;ids.add(c.id);}
      store.courses.push(...incoming);state.courseId=incoming[0].id;state.mode='outline';state.chapter='all';state.task='all';state.query='';state.domain='all';state.scope='all';state.bookmarksOnly=false;state.cardId=null;state.scenarioId=null;state.orders={flashcards:[],scenarios:[]};restorePosition();
      if(DIALOG.open)DIALOG.close();renderAll();toast(`${incoming.length} ${incoming.length===1?'curriculum':'curricula'} added to your library.`);
    } catch(error){toast(`Import failed: ${error.message}`);}finally{$('#import-file').value='';}
  }

  document.addEventListener('click',event=>{
    const el=event.target.closest('[data-action]');if(!el||el.disabled)return;
    const action=el.dataset.action;
    switch(action){
      case 'retry-load':renderAll();break;
      case 'personal-material':openBookDropdown('my-material','resume');break;
      case 'open-menu':state.sidebarOpen=true;renderDrawer();break;
      case 'close-menu':state.sidebarOpen=false;renderDrawer();break;
      case 'mode':switchMode(el.dataset.mode);break;
      case 'topic':chooseTopic(el.dataset.id);break;
      case 'domain-toggle':state.expanded.has(el.dataset.id)?state.expanded.delete(el.dataset.id):state.expanded.add(el.dataset.id);renderSidebar();break;
      case 'all-topics':state.scope='all';clearFilters();break;case 'clear-filters':clearFilters();break;
      case 'filter-bookmarks':state.lessonView='lesson';state.bookmarksOnly=!state.bookmarksOnly;state.cardId=null;state.scenarioId=null;renderAll();break;
      case 'bookmark':{const t=topic();if(!t)break;const p=progress();p.bookmarks=p.bookmarks.includes(t.id)?p.bookmarks.filter(x=>x!==t.id):[...p.bookmarks,t.id];renderAll();break;}
      case 'reviewed':{const t=topic(),p=progress();p.reviewed=p.reviewed.includes(t.id)?p.reviewed.filter(x=>x!==t.id):[...p.reviewed,t.id];renderAll();break;}
      case 'previous':navigate(-1);break;case 'next':navigate(1);break;
      case 'flip':flipCard();break;
      case 'shuffle':shuffle();break;
      case 'rate-card':rateCard(el.dataset.rating);break;
      case 'choose-answer':{const item=activeItem();if(!item||state.mode!=='scenarios')break;if(progress().scenarios[item.id]&&!state.retries.has(item.id))break;state.drafts[item.id]=Number(el.dataset.index);renderWorkspace();break;}
      case 'check-answer':checkAnswer();break;
      case 'retry-scenario':{const item=activeItem();if(!item)break;state.retries.add(item.id);delete state.drafts[item.id];renderWorkspace();break;}
      case 'all-practice':state.scope='all';state.cardId=null;state.scenarioId=null;renderAll();break;
      case 'large-text':store.ui.large=!store.ui.large;renderAll();break;
      case 'sources':showSources();break;case 'settings':showSettings();break;
      case 'close-dialog':DIALOG.close();break;
      case 'export-backup':exportBackup();break;case 'export-pack':exportPack();break;
      case 'import':$('#import-file').click();break;
      case 'add-topic':showEditor(false);break;case 'edit-topic':if(course().id!=='design-patterns-cpp'&&!isBook())showEditor(true);break;
      case 'reset-progress':if(confirm(`Reset learning progress for “${course().title}”? Your notes, bookmarks, coding drafts, and material will be kept.`)){const p=progress();store.progress[state.courseId]={...blankProgress(),bookmarks:p.bookmarks,notes:p.notes,coding:Object.fromEntries(Object.entries(p.coding||{}).map(([id,d])=>[id,{...d,passed:false,checkedCode:'',checkedAt:''}])),codingSelected:p.codingSelected};state.drafts={};state.retries.clear();renderAll();showSettings();toast('Learning progress reset. Notes and bookmarks were kept.');}break;
    }
  });
  document.addEventListener('input',event=>{
    if(event.target.id==='topic-search'){state.lessonView='lesson';const el=event.target,pos=el.selectionStart;state.query=el.value;state.cardId=null;state.scenarioId=null;state.flipped=false;renderAll();el.focus();try{el.setSelectionRange(pos,pos);}catch{}}
    if(event.target.id==='personal-note'){progress().notes[event.target.dataset.topic]=event.target.value;remember();}
  });
  document.addEventListener('change',event=>{
    const el=event.target;
    if(el.matches('.book-chapter-dropdown')&&el.value)openBookDropdown(el.dataset.course,el.value);
    if(el.id==='domain-select'){state.domain=el.value;state.cardId=null;state.scenarioId=null;state.flipped=false;renderAll();}
    if(el.id==='scope-select'){state.scope=el.value;state.cardId=null;state.scenarioId=null;state.flipped=false;renderAll();}
    if(el.id==='lecture-slide-select'&&el.value){state.mode='outline';state.lessonView='lesson';chooseTopic(el.value);}
    if(el.id==='import-file')importFile(el.files[0]);
  });
  document.addEventListener('submit',event=>{if(event.target.id==='topic-form'){event.preventDefault();saveTopic(event.target);}});
  document.addEventListener('keydown',event=>{
    if(DIALOG.open||event.ctrlKey||event.metaKey||event.altKey||event.isComposing)return;
    const tag=event.target.tagName;const interactive=['INPUT','TEXTAREA','SELECT'].includes(tag)||event.target.isContentEditable;
    if(event.key==='Escape'){if(state.sidebarOpen){state.sidebarOpen=false;renderDrawer();}return;}
    if(interactive)return;
    const key=event.key.toLowerCase();
    if(key==='/'){event.preventDefault();if(innerWidth<=800){state.sidebarOpen=true;renderDrawer();}$('#topic-search').focus();}
    else if(key==='arrowleft'){event.preventDefault();navigate(-1);}
    else if(key==='arrowright'){event.preventDefault();navigate(1);}
    else if(['o','f','s'].includes(key)){event.preventDefault();switchMode({o:'outline',f:'flashcards',s:'scenarios'}[key]);}
    else if(key===' '&&state.mode==='flashcards'&&!['BUTTON','A'].includes(tag)){event.preventDefault();flipCard();}
    else if(state.mode==='scenarios'&&!course().series&&['1','2','3','4'].includes(key)){const item=activeItem();if(item&&(!progress().scenarios[item.id]||state.retries.has(item.id))){event.preventDefault();state.drafts[item.id]=Number(key)-1;renderWorkspace();}}
    else if(key==='enter'&&state.mode==='scenarios'&&!['BUTTON','A'].includes(tag)){event.preventDefault();checkAnswer();}
  });
  // Clicking the native dialog's backdrop closes it; clicks within its bounds do not.
  DIALOG.addEventListener('click',event=>{if(event.target===DIALOG){const r=DIALOG.getBoundingClientRect();if(event.clientX<r.left||event.clientX>r.right||event.clientY<r.top||event.clientY>r.bottom)DIALOG.close();}});
  let glossaryQuery='', codeKind='examples';
  function isBook(){return course().kind==='textbook';}
  function cleanPages(){return undefined;}
  function cleanBlocks(v){if(Array.isArray(v)&&v.some(b=>b?.kind))return cleanSeriesBlocks(v);return (Array.isArray(v)?v:[]).slice(0,100).filter(isObject).map(b=>({type:['paragraph','code','list','reveal'].includes(b.type)?b.type:'paragraph',text:text(b.text,30000),title:text(b.title,1500),code:text(b.code,100000),items:Array.isArray(b.items)?b.items.slice(0,100).map(x=>text(x,15000)):[]}));}
  function cleanBookMetadata(c){
    if(c.kind!=='textbook')return {};
    const chapters=(c.chapters||[]).filter(x=>Number.isInteger(x.number)&&x.number>=0&&x.number<=100).map(x=>({number:x.number,title:text(x.title,400),part:Number(x.part)||0,partTitle:text(x.partTitle,400),role:['chapter','front','reference'].includes(x.role)?x.role:undefined}));
    const glossary=(c.glossary||[]).slice(0,1000).filter(x=>validId(x.id)).map(x=>({id:x.id,term:text(x.term,500),definition:text(x.definition,20000),chapter:Number(x.chapter)}));
    // Extra study payloads render as escaped text or inert image data, never executable HTML.
    return {kind:'textbook',series:isObject(c.series)?c.series:undefined,chapters,glossary,teaching:isObject(c.teaching)?c.teaching:undefined,readingOrder:Array.isArray(c.readingOrder)?c.readingOrder.filter(validId):undefined,author:text(c.author,400),coverageLabel:text(c.coverageLabel,600),familyLabel:text(c.familyLabel,100),lectures:isObject(c.lectures)?c.lectures:{},examples:isObject(c.examples)?c.examples:{},diagrams:isObject(c.diagrams)?c.diagrams:{}};
  }
  function renderBlocks(blocks){return (blocks||[]).map(b=>b.type==='code'?`<section class="code-excerpt"><h3>${escape(b.title)}</h3><pre><code>${escape(b.code)}</code></pre></section>`:b.type==='list'?`<section class="text-block"><h3>${escape(b.title)}</h3><ul>${(b.items||[]).map(x=>`<li>${escape(x)}</li>`).join('')}</ul></section>`:b.type==='reveal'?`<details class="answer-reveal"><summary>${escape(b.title)}</summary><p>${escape(b.text)}</p></details>`:`<p class="reader-paragraph">${escape(b.text)}</p>`).join('');}
  function bookTopicBadge(t){if(course().series)return `<span class="pill bg-brand-soft text-brand">BOOK ${escape(course().series.roman)} · ${escape(seriesLabel(t.chapter).toUpperCase())}</span>`;return `<span class="pill bg-brand-soft text-brand">${t.chapter===-1?'GLOSSARY':t.chapter===0?'FOUNDATIONS':'CHAPTER '+t.chapter}</span><span class="text-[10px] text-muted">${escape(t.id)}</span>`;}
  function bookSourceLine(t){const lecture=course().lectures?.[String(t.chapter)];return `<section class="book-source"><div class="eyebrow">${lecture?'LECTURE SOURCE':'TEXTBOOK CONNECTION'}</div><p>${escape(t.chapterTitle)}</p><p class="text-[10px] text-muted">${escape(t.sourceRef||'Chapter glossary and explanations')}</p><div class="flex gap-2 flex-wrap mt-3">${lecture?`<button class="btn" data-action="mode" data-mode="lectures">${icon('source')} Original lecture materials</button>`:t.chapter>0?`<button class="btn" data-action="mode" data-mode="uml">${icon('grid')} UML &amp; theory</button><button class="btn" data-action="mode" data-mode="code">${icon('source')} Working C++</button>`:''}</div></section>`;}

  function goChapter(n){const t=course().topics.find(t=>t.chapter===Number(n));if(!t)return;state.chapter=String(n);state.domain='all';state.query='';state.bookmarksOnly=false;state.scope='chapter';state.lessonView='map';chooseTopic(t.id);}
  function renderBookControls(){
    const nav=$('#book-nav-controls'),bar=$('#book-toolbar');if(!isBook()){nav.innerHTML='';bar.innerHTML='';return;}
    nav.innerHTML=`<div class="book-nav-switch"><button data-action="book-nav" data-view="chapters" class="${state.bookNav==='chapters'?'active':''}">Chapters</button><button data-action="book-nav" data-view="domains" class="${state.bookNav==='domains'?'active':''}">${escape(course().familyLabel||'Pattern families')}</button></div>`;
    bar.innerHTML=`<section class="book-toolbar-panel"><div class="flex items-center justify-between gap-2 flex-wrap"><span class="pill bg-mint text-mint-ink">${escape(course().author||'Dr. Charles Dorner')}</span><button class="source-link" data-action="coverage">Course outline ${icon('chevron')}</button></div><div class="book-control-row mt-3"><label for="chapter-select" class="sr-only">Choose textbook chapter</label><select id="chapter-select" class="select-small" aria-label="Choose textbook chapter"><option value="all">All chapters</option>${course().chapters.map(c=>`<option value="${c.number}" ${String(c.number)===state.chapter?'selected':''}>${course().series?escape(seriesChapterLabel(c)):(c.number===0?'Class 0':c.number)}. ${escape(c.title)}</option>`).join('')}${course().glossary?.length?`<option value="-1" ${state.chapter==='-1'?'selected':''}>Reference · Glossary</option>`:''}</select>${course().glossary?.length?`<button class="btn" data-action="book-glossary">${icon('book')} Glossary</button>`:''}</div></section>`;
  }
  function renderLegacyBookSidebar(list,c,p){
    $('#filtered-count').textContent=list.length===c.topics.length?'':`${list.length} shown`;
    const gs=state.bookNav==='chapters'?[...c.chapters.map(x=>({id:'CH'+x.number,label:x.number===0?'00':String(x.number).padStart(2,'0'),title:x.title,topics:list.filter(t=>t.chapter===x.number)})),{id:'CH-1',label:'REF',title:'Glossary',topics:list.filter(t=>t.chapter===-1)}]:c.domains.map(d=>({id:d.id,label:d.id.slice(0,3),title:d.title,topics:list.filter(t=>t.domain===d.id)}));
    $('#topic-nav').innerHTML=gs.filter(g=>g.topics.length).map(g=>{const open=state.expanded.has(g.id)||!!state.query;return `<div class="mb-1"><button class="domain-row ${g.topics.some(t=>t.id===state.topicId)?'active':''}" data-action="domain-toggle" data-id="${g.id}" aria-expanded="${open}"><span class="domain-letter">${g.label}</span><span class="domain-title">${escape(g.title)}</span><span class="domain-count">${g.topics.length}</span><span class="domain-chevron ${open?'open':''}">${icon('chevron')}</span></button>${open?`<div>${g.topics.map(t=>`<button class="topic-row ${t.id===state.topicId?'active':''}" data-action="topic" data-id="${t.id}"><span class="topic-id">${escape(t.id)}</span><span class="topic-title">${escape(t.title)}</span>${p.reviewed.includes(t.id)?`<span class="icon-sm">${icon('check')}</span>`:''}</button>`).join('')}</div>`:''}</div>`}).join('');
  }
  function renderGlossaryOutline(t){return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">The book’s vocabulary</h2><p class="reading">${course().glossary.length} definitions with chapter context. Use Flashcards to practice the same terms.</p><label for="glossary-search" class="field-label">Find a term</label><input class="field" id="glossary-search" value="${escape(glossaryQuery)}" placeholder="Try ownership, product, or snapshot"><div id="glossary-results">${glossaryResults()}</div></div></article>`;}
  function glossaryResults(){const q=glossaryQuery.toLowerCase();const a=course().glossary.filter(g=>(g.term+' '+g.definition).toLowerCase().includes(q));return `<p class="text-[11px] text-muted mt-4">${a.length} terms${a.length>40?' · Showing the first 40. Narrow your search.':''}</p>`+a.slice(0,40).map(g=>`<section class="glossary-entry"><h3>${escape(g.term)}</h3><p>${escape(g.definition)}</p><button class="source-link" data-action="go-chapter" data-chapter="${g.chapter}">Chapter ${g.chapter} ${icon('right')}</button></section>`).join('');}
  function renderLectures(t){const l=course().lectures?.[String(t.chapter)];if(!l)return '<section class="panel empty"><h2>Choose a lecture chapter</h2></section>';const slides=course().topics.filter(x=>x.chapter===t.chapter&&x.slide);return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">${escape(l.title)}</h2><p class="reading">${l.slideCount} source slides with their matching professor transcript. Downloads preserve the supplied presentation, including its original diagrams, images, equations, and speaker notes.</p><div class="flex gap-3 flex-wrap mt-5 mb-5"><button class="btn btn-primary" data-action="download-lecture" data-kind="presentation">${icon('download')} Download PowerPoint</button><button class="btn" data-action="download-lecture" data-kind="transcript">${icon('download')} Download transcript</button></div><label class="field-label" for="lecture-slide-select">Open a slide lesson</label><select class="field" id="lecture-slide-select"><option value="">Choose slide</option>${slides.map(x=>`<option value="${escape(x.id)}">${x.slide}. ${escape(x.title)}</option>`).join('')}</select><details class="answer-reveal"><summary>Read the complete chapter transcript</summary><div class="lecture-transcript">${escape(l.transcript)}</div></details><p class="code-ref">Supplied files: ${escape(l.presentationName)}; ${escape(l.transcriptName)}. These source lectures remain as supplied. Open C++ for the separately authored, compiled chapter demonstrations and labs.</p>${chapterNav(t.chapter)}</div></article>`;}
  function downloadLecture(kind){const l=course().lectures[String(topic().chapter)];if(kind==='presentation'&&l.presentationBase64?.$asset){Library.downloadAsset(l.presentationBase64,l.presentationName);return;}let blob,name;if(kind==='presentation'){const raw=atob(l.presentationBase64);blob=new Blob([Uint8Array.from(raw,c=>c.charCodeAt(0))],{type:'application/vnd.openxmlformats-officedocument.presentationml.presentation'});name=l.presentationName;}else{blob=new Blob([l.transcript],{type:'text/plain;charset=utf-8'});name=l.transcriptName;}const u=URL.createObjectURL(blob),a=document.createElement('a');a.href=u;a.download=name;document.body.append(a);a.click();a.remove();setTimeout(()=>URL.revokeObjectURL(u),1500);}
  function chapterNav(n){return `<div class="chapter-nav"><button class="btn" data-action="previous" ${n<=1?'disabled':''}>${icon('left')} Previous chapter</button><button class="btn" data-action="mode" data-mode="outline">Chapter lessons</button><button class="btn" data-action="next" ${n>=Math.max(...course().chapters.map(c=>c.number))?'disabled':''}>Next chapter ${icon('right')}</button></div>`;}
  function chapterNeeded(){if(course().id==='systems-programming')return `<section class="panel empty"><h2>Choose a Systems chapter</h2><p>Every Systems chapter has diagrams, focused explanations, a complete C++ demonstration, and an extension lab.</p><button class="btn btn-primary" data-action="go-chapter" data-chapter="1">Open Chapter 1 ${icon('right')}</button></section>`;if(!isBook())return `<section class="panel empty"><h2>No worked examples in this study pack</h2><p>Use Outline, Flashcards, and Scenarios for your personal material. Select Design Patterns in C++ for the textbook’s UML and full programs.</p><button class="btn btn-primary" data-action="mode" data-mode="outline">Return to outline</button></section>`;return `<section class="panel empty"><h2>Choose a worked example</h2><p>Every pattern chapter and the capstone have UML views, focused code, and full working source.</p><button class="btn btn-primary" data-action="go-chapter" data-chapter="1">Factory Method ${icon('right')}</button></section>`;}
  function renderUML(t){
    const d=course().diagrams?.[String(t.chapter)];if(!d)return chapterNeeded();
    const views=[d.overview,...(d.extra_overviews||[])];
    return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">${escape(t.chapterTitle)}: UML &amp; theory</h2><p class="reading">Follow each relationship to the code. Each added overview names its source; the original study uses ${escape(d.source)}. Code boxes are excerpts with teaching comments. Focused blocks below show exact source lines.</p>${views.map((v,i)=>`<section class="diagram-section ${v.expansionId?'diagram-study-addition':''}"><div class="flex gap-2 items-center justify-between flex-wrap"><h3>${escape(v.title)}</h3><button class="btn" data-action="expand-diagram" data-view="${i}">Open large ${icon('external')}</button></div><p class="diagram-kind">${escape(v.kind)} view${v.source?` · ${escape(v.source)}`:''}</p><div class="diagram-scroll"><img class="uml-image" src="${safeImage(v.image)}" alt="${escape(v.title)}. ${escape(v.explanation)}"></div><p class="diagram-legend">${escape(v.explanation)}</p>${v.expansionId?`<p><button class="btn" data-action="lab-open-question" data-id="${escape(v.expansionId)}">Open matching Coding Lab challenge ${icon('right')}</button></p>`:''}<details class="answer-reveal"><summary>Relationships as text</summary><ul>${v.edges.map(e=>`<li>${escape(v.nodes.find(n=>n.id===e.from)?.label||e.from)} → ${escape(v.nodes.find(n=>n.id===e.to)?.label||e.to)}: ${escape(e.kind)}; ${escape(e.label)}</li>`).join('')}</ul></details></section>`).join('')}<h3 class="focus-heading">Code blocks and adjacent theory</h3>${d.focus.map(f=>`<section class="focus-grid"><div><h4>${escape(f.title)}</h4><p class="code-ref">${escape(f.file)}:${f.lineStart}–${f.lineEnd}</p><pre><code>${escape(f.code.split('\n').map((l,i)=>`${f.lineStart+i}  ${l}`).join('\n'))}</code></pre><details class="answer-reveal"><summary>Line-by-line explanation</summary>${f.explain.map((x,i)=>`<p><strong>${f.lineStart+i}.</strong> ${escape(x)}</p>`).join('')}</details></div><aside class="theory-panel"><p class="annotation-label">Teaching annotation for this code block</p>${[['WHAT THIS MEANS',f.what],['WHERE IT FITS',f.where],['WHY IT IS HERE',f.why],['WHAT MUST STAY TRUE',f.invariant],['WHAT CAN GO WRONG',f.risk],['FIND THE CODE',`${f.file}:${f.lineStart}–${f.lineEnd}`]].map(([h,b])=>`<section><h5>${h}</h5><p>${escape(b)}</p></section>`).join('')}</aside></section>`).join('')}${chapterNav(t.chapter)}</div></article>`;
  }
  function safeImage(x){if(Library&&x?.$asset&&/^assets\/[a-f0-9]+\.(svg|png|bin)$/.test(x.$asset))return x.$asset;return typeof x==='string'&&x.startsWith('data:image/svg+xml;base64,')?x:'';}
  function renderCode(t){const ex=course().examples?.[String(t.chapter)];if(!ex)return chapterNeeded();const e=ex[codeKind]||ex.examples;return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">${escape(t.chapterTitle)}: working C++</h2><div class="flex gap-2 flex-wrap mb-4">${[['examples','Example'],['exercises','Lab starter'],['solutions','Lab solution']].map(([k,l])=>`<button class="btn ${codeKind===k?'btn-primary':''}" data-action="code-kind" data-kind="${k}" aria-pressed="${codeKind===k}">${l}</button>`).join('')}<button class="btn btn-primary" data-action="edit-in-lab">Edit &amp; run in Coding Lab</button><button class="btn" data-action="download-code">${icon('download')} Download .cpp</button></div><p class="code-ref">${escape(e.filename)} · C++17 · Standard library</p>${codeKind!=='examples'?`<p class="reading">${escape(ex.lab.task)}</p>${course().id==='systems-programming'?`<h4>Acceptance checks</h4><ul class="lab-checks">${ex.lab.checks.map(x=>`<li>${escape(x)}</li>`).join('')}</ul>`:''}`:''}<details class="answer-reveal" open><summary>Build and expected output</summary><pre><code>g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread ch${String(t.chapter).padStart(2,'0')}_${codeKind}.cpp -o example
./example</code></pre><h4>Expected output for this file</h4><pre><code>${escape(e.output)}</code></pre><p>${escape(codeKind==='examples'?ex.verification:codeKind==='exercises'?'The starter verifies its initial behavior. Complete the lab and add its acceptance checks.':ex.lab.answer)}</p></details><pre class="full-code"><code>${escape(e.code.split('\n').map((l,i)=>`${String(i+1).padStart(3)}  ${l}`).join('\n'))}</code></pre>${chapterNav(t.chapter)}</div></article>`;}
  function showBookSources(){if(course().series)return showSeriesSources();const c=course(),n=totals();if(c.lectures&&Object.keys(c.lectures).length){modal('Lecture series and study coverage',`<p>${escape(c.provenance)}</p><h3>Included material</h3><p>${c.chapters.length} chapters, ${Object.values(c.lectures).reduce((n,l)=>n+l.slideCount,0)} slide lessons, ${n.cards} recall cards, and ${n.scenarios} original application cases. The Lectures view downloads each original PowerPoint and full transcript.</p><p>Notes and progress are kept separately for each course in this browser. Export a backup before moving the HTML or clearing browser data.</p>`);return;}modal('Textbook and study coverage',`<p>${escape(c.provenance||'Your imported study material.')}</p><h3>Included material</h3><p>Foundations, all 22 pattern chapters, and the capstone. ${n.topics} lessons, ${n.cards} recall cards, ${n.scenarios} original scenarios, 25 diagram views, and 51 focused code blocks with adjacent theory. Every named textbook section has a lesson. Exercises include hints and explained answers. The C++ view includes each example, lab starter, and reference solution.</p><h3>Using this companion</h3><p>Choose a chapter, read its outline, then switch to Flashcards or Scenarios. Use UML to follow relationships and C++ to inspect the complete implementation. Review marks and card ratings are study records, not a guarantee of mastery.</p><h3>Local storage</h3><p>Notes, bookmarks, and results stay in this browser. Export a backup before changing browsers or moving a locally opened HTML file. The app has no account or analytics. Coding Lab sends only the current editor code and input to Compiler Explorer when you choose Run online or Check solution.</p>`);}
  function showLegacyCoverage(){modal('Book roadmap',`<p>Choose a chapter to open its lessons. ${course().lectures&&Object.keys(course().lectures).length?'Subject areas group the supplied lectures for study.':'Pattern families describe the design problem, not a required implementation language feature.'}</p>${course().chapters.map(ch=>`<section class="crosswalk-row"><h3>${ch.number===0?'Class 0':`Chapter ${ch.number}`} · ${escape(ch.title)}</h3><p>${course().topics.filter(t=>t.chapter===ch.number).length} lessons</p><button class="btn" data-action="go-chapter" data-chapter="${ch.number}">Study chapter ${icon('right')}</button></section>`).join('')}`);DIALOG.classList.add('wide-dialog');}
  document.addEventListener('click',e=>{const el=e.target.closest('[data-action]');if(!el||el.disabled)return;
    switch(el.dataset.action){
      case 'book-nav':state.bookNav=el.dataset.view;renderAll();break;
      case 'coverage':showCoverage();break;
      case 'go-chapter':if(DIALOG.open)DIALOG.close();goChapter(el.dataset.chapter);break;
      case 'book-glossary':state.chapter='-1';state.domain='all';state.query='';state.bookmarksOnly=false;state.scope='topic';state.mode='outline';chooseTopic('REF.GLOSSARY');break;
      case 'download-lecture':downloadLecture(el.dataset.kind);break;
      case 'code-kind':codeKind=el.dataset.kind;renderWorkspace();break;
      case 'download-code':{const e=course().examples[String(topic().chapter)][codeKind];const u=URL.createObjectURL(new Blob([e.code],{type:'text/plain'}));const a=document.createElement('a');a.href=u;a.download=`ch${String(topic().chapter).padStart(2,'0')}_${codeKind}.cpp`;a.click();setTimeout(()=>URL.revokeObjectURL(u),1000);break;}
      case 'expand-diagram':{const d=course().diagrams[String(topic().chapter)];const v=[d.overview,...d.extra_overviews][Number(el.dataset.view)];modal(v.title,`<div class="diagram-scroll expanded"><img src="${safeImage(v.image)}" alt="${escape(v.title)}"></div><p>${escape(v.explanation)}</p>`);DIALOG.classList.add('wide-dialog');break;}
    }
  });
  document.addEventListener('input',e=>{if(e.target.id==='glossary-search'){glossaryQuery=e.target.value;$('#glossary-results').innerHTML=glossaryResults();}});
  document.addEventListener('change',e=>{if(e.target.id==='chapter-select'){state.chapter=e.target.value;state.domain='all';state.query='';state.cardId=null;state.scenarioId=null;state.flipped=false;state.scope='chapter';state.lessonView='map';if(state.chapter!=='all'){const first=course().teaching?.[state.chapter]?.startTopicId;if(first)state.topicId=first;}renderAll();window.scrollTo({top:0,behavior:'instant'});}});

    // Hierarchical reading layer. Original topics remain the source/progress identity.
  function chapterGuide(n=topic()?.chapter){return course().teaching?.[String(n)];}
  function readingTopics(){
    const list=visibleTopics();if(!course().readingOrder)return list;
    const order=new Map(course().readingOrder.map((id,i)=>[id,i]));
    return list.filter(t=>!t.outlineParent||state.query||state.bookmarksOnly).sort((a,b)=>(order.get(a.id)??99999)-(order.get(b.id)??99999));
  }
  function prose(text){
    return String(text||'').split(/\n\n+/).flatMap(p=>{
      const sentences=p.split(/(?<=[.!?])\s+(?=[A-Z])/).map(s=>s+' ');
      const chunks=[];let buffer='';
      for(const s of sentences){if(buffer.length>270){chunks.push(buffer.trim());buffer='';}buffer+=s;}
      if(buffer.trim())chunks.push(buffer.trim());
      return chunks.map(x=>`<p>${escape(x)}</p>`);
    }).join('');
  }
  function teachingCode(w){
    if(w.excerpt)return w.excerpt;
    return w.code.replace(/^(?:#include[^\n]*\n)+\s*\n/,'');
  }
  function guideTrail(t,g){
    const section=g.sections.find(s=>s.id===t.sectionId);
    return `<nav class="lesson-trail" aria-label="Reading location"><button data-action="coverage">Course outline</button><span>/</span><button data-action="chapter-map">${t.chapter===0?'Class 0':'Chapter '+t.chapter}</button>${section?`<span>/</span><button data-action="guide-section" data-section="${escape(section.id)}">${escape(section.title)}</button>`:''}</nav>`;
  }
  function discussionPanel(d,label='Discuss the design'){
    if(!d)return '';
    return `<section class="discussion-panel"><span class="teaching-kicker">DISCUSSION</span><h3>${escape(label)}</h3><p class="discussion-question">${escape(d.question)}</p><p class="discussion-instruction">Make a claim. Point to a step or line that supports it. Then compare your reasoning.</p>${d.hint?`<details class="answer-reveal"><summary>Need a hint?</summary>${prose(d.hint)}</details>`:''}<details class="answer-reveal discussion-answer"><summary>Read the explained answer</summary><div class="teaching-prose">${prose(d.answer)}</div></details></section>`;
  }
  function traceTable(w){return `<div class="trace-table-wrap"><table class="trace-table"><caption>Follow the state, one step at a time</caption><thead><tr><th scope="col">Step</th><th scope="col">What changes</th><th scope="col">Why</th></tr></thead><tbody>${w.steps.map((s,i)=>`<tr><th scope="row"><span class="trace-number">${i+1}</span>${escape(s.action)}</th><td>${escape(s.state)}</td><td>${escape(s.why)}</td></tr>`).join('')}</tbody></table></div>`;}
  function workshopPanel(g,{compact=false}={}){
    const w=g.workshop;
    return `<section class="workshop-panel" id="chapter-workshop"><div class="teaching-section-head"><span class="teaching-kicker">WORKED EXAMPLE · ${escape(w.kind)}</span><h3>${escape(w.title)}</h3><div class="teaching-prose">${prose(w.problem)}</div></div>${w.design?`<section class="design-reason"><h4>Why this design?</h4><div class="teaching-prose">${prose(w.design)}</div></section>`:''}<div class="workshop-code-grid"><div class="min-w-0"><h4>Read the code</h4><p class="teaching-caption">${escape(w.excerptLabel||w.filename)} · Excerpt; complete program below.</p><pre class="teaching-code"><code>${escape(teachingCode(w))}</code></pre><h4>Expected output</h4><pre class="expected-output"><code>${escape(w.output)}</code></pre></div><aside class="worked-explanation"><span class="teaching-kicker">HOW TO READ THIS EXAMPLE</span><h4>Predict, then trace.</h4><p>Before looking at the output, follow each change in the table. Keep the assumptions with the result.</p>${w.invariants?`<h4>Rules that must stay true</h4><ul>${w.invariants.map(x=>`<li>${escape(x)}</li>`).join('')}</ul>`:''}<div class="pitfall-panel"><h4>A common wrong turn</h4>${prose(w.pitfall)}</div></aside></div>${traceTable(w)}<section class="alternative-panel"><h4>Compare another design</h4><div class="teaching-prose">${prose(w.alternative)}</div></section>${!compact?discussionPanel(w.discussion):''}${w.maintenance?`<section class="maintenance-panel"><h4>Maintaining a real program</h4><div class="teaching-prose">${prose(w.maintenance)}</div></section>`:''}<details class="answer-reveal full-program"><summary>Complete program, build command, and verification</summary><p class="teaching-caption">${escape(w.filename)} · C++17 · Standard library</p><div class="flex gap-2 flex-wrap"><button class="btn" data-action="download-workshop">${icon('download')} Download complete .cpp</button></div><pre><code>g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread ${escape(w.filename)} -o workshop
./workshop</code></pre><p>From the source ZIP root, use the command above. For the browser download, replace the source path with the downloaded filename. Keep assertions enabled.</p><pre><code>${escape(w.code)}</code></pre><p>${escape(w.verification||'The packaged verification script compiles this program and compares its exact output with expected.txt. Assertions check this demonstration’s stated result. Model output does not measure the behavior or speed of real hardware.')}</p></details></section>`;
  }
  function chapterMapCards(g){
    return `<ol class="chapter-map-list">${g.sections.map(s=>`<li class="map-section" id="map-${escape(s.id)}"><div class="map-section-heading"><span class="map-number">${String(s.number).padStart(2,'0')}</span><div><h3>${escape(s.title)}</h3><p>${escape(s.purpose)}</p></div></div><ol>${s.topics.map((id,i)=>{const t=course().topics.find(t=>t.id===id);return t?`<li><button class="map-topic" data-action="read-topic" data-id="${escape(t.id)}"><span class="map-subnumber">${s.number}.${i+1}</span><span><strong>${escape(t.reading?.title||t.title)}</strong>${s.topics.length<=6?`<span class="map-preview">${escape(t.reading?.preview||'')}</span>`:''}</span>${icon('chevron')}</button></li>`:''}).join('')}${!s.topics.length?`<li><button class="map-topic" data-action="workshop-jump"><span class="map-subnumber">${s.number}.1</span><span><strong>${escape(g.workshop.title)}</strong><span class="map-preview">New C++ workshop with an explained trace and result.</span></span>${icon('chevron')}</button></li>`:''}</ol></li>`).join('')}</ol>`;
  }
  function teachingNotes(t){return `<section class="teaching-notes"><label for="personal-note">Your explanation, in your own words</label><p>Record a prediction, a design choice, or the point you still want to test.</p><textarea class="notes-area" id="personal-note" data-topic="${escape(t.id)}" placeholder="What changed? Why did it work? When would I choose another design?" maxlength="20000">${escape(progress().notes[t.id]||'')}</textarea></section>`;}
  function readingFooter(t){
    const list=readingTopics(),index=list.findIndex(x=>x.id===t.id),read=progress().reviewed.includes(t.id);
    return `<div class="lesson-bottom teaching-footer"><button class="btn" data-action="previous" ${index<=0?'disabled':''}>${icon('left')} Previous lesson</button><button class="btn ${read?'btn-success':'btn-primary'}" data-action="reviewed" aria-pressed="${read}">${icon('check')} ${read?'Reviewed':'Mark reviewed'}</button><button class="btn" data-action="next" ${index>=list.length-1?'disabled':''}>Next lesson ${icon('right')}</button></div>`;
  }
  function renderChapterMap(t,g){
    return `<article class="panel teaching-reader chapter-guide">${guideTrail(t,g)}<div class="teaching-hero"><div><div class="flex gap-2 items-center justify-between">${topicBadge(t)}${bookmarkButton(t)}</div><span class="teaching-kicker">CHAPTER MAP</span><h2 class="lesson-heading">${escape(g.title)}</h2><p class="chapter-problem">${escape(g.workshop.problem)}</p><div class="teaching-actions"><button class="btn btn-primary" data-action="workshop-jump">Start with the example ${icon('right')}</button><button class="btn" data-action="read-topic" data-id="${escape(g.firstLessonId||g.startTopicId)}">Read the first lesson</button></div></div><aside class="chapter-goals"><span class="teaching-kicker">BY THE END, YOU CAN</span><ul>${g.objectives.map(x=>`<li>${escape(x)}</li>`).join('')}</ul><p>${g.sections.length} sections · ${g.sections.reduce((n,s)=>n+s.topics.length,0)} reading entries<br>One connected example, with discussion and practice.</p></aside></div><section class="map-intro"><h3>Your route through this chapter</h3><p>The numbered sections are the outline. Open a lesson for its explanation, or start with the chapter’s worked example below.</p></section>${chapterMapCards(g)}${workshopPanel(g)}<section class="next-practice"><h3>Now use the idea</h3><p>Explain the result without looking. Change one assumption. Then test your reasoning with the chapter’s questions.</p><div class="flex gap-2 flex-wrap"><button class="btn" data-action="mode" data-mode="flashcards">${icon('layers')} Recall the concepts</button><button class="btn" data-action="mode" data-mode="scenarios">${icon('scenario')} Apply the chapter</button>${course().diagrams?.[String(t.chapter)]?`<button class="btn" data-action="mode" data-mode="uml">${icon('grid')} Follow the UML</button>`:''}</div></section>${teachingNotes(t)}${readingFooter(t)}</article>`;
  }
  function sourceMaterial(t){
    const companion=(t.sourceCompanions||[]).map(id=>course().topics.find(x=>x.id===id)).filter(Boolean);
    return `<details class="answer-reveal source-material"><summary>Original source material and full context${companion.length?' · includes paired Deep Dive':''}</summary><p class="code-ref">${escape(t.sourceRef||'Textbook manuscript')}</p><div class="source-original"><h4>${escape(t.title)}</h4>${prose(t.summary)}${renderBlocks(t.blocks)}${companion.map(x=>`<h4>${escape(x.title)}</h4>${renderBlocks(x.blocks)}`).join('')}</div></details>`;
  }
  function lessonTheory(t){
    const paragraphs=t.reading?.paragraphs||[t.summary];const isLecture=!!t.slide;
    let result=`<div class="teaching-prose">${paragraphs.map(p=>prose(p)).join('')}</div>${t.reading?.plain?`<aside class="plain-connection"><h3>In plain language</h3>${prose(t.reading.plain)}</aside>`:''}`;
    if(t.concepts.length)result+=`<dl class="concept-definitions">${t.concepts.map(c=>`<div><dt>${escape(c.term)}</dt><dd>${prose(c.definition)}</dd></div>`).join('')}</dl>`;
    if(!isLecture)result+=renderBlocks(t.blocks.filter(b=>b.type!=='paragraph'));
    if(t.reading?.correction)result=`<aside class="source-correction"><h3>Read this distinction carefully</h3>${prose(t.reading.correction)}</aside>`+result;
    if(t.scenario){result+=`<section class="lesson-case"><h3>Compare the possible decisions</h3><ol type="A">${t.scenario.options.map(s=>`<li>${escape(s)}</li>`).join('')}</ol>${discussionPanel({question:'Which choice preserves the requirement, and what breaks in the others?',answer:t.scenario.explanation},'Explain your choice')}<button class="btn" data-action="mode" data-mode="scenarios">Answer in Scenarios ${icon('right')}</button></section>`;}
    return result;
  }
  function renderGuidedLesson(t,g){
    const d=t.reading?.discussion||g.workshop.discussion;
    return `<article class="panel teaching-reader">${guideTrail(t,g)}<div class="teaching-lesson-head"><div class="flex items-center justify-between gap-3"><div>${topicBadge(t)}</div>${bookmarkButton(t)}</div><h2 class="lesson-heading">${escape(t.reading?.title||t.title)}</h2><div class="lesson-jumps"><a href="#lesson-idea">Understand</a><a href="#chapter-workshop">See an example</a><a href="#lesson-discuss">Discuss</a><button data-action="chapter-map">Chapter map</button></div></div><section class="lesson-explanation" id="lesson-idea"><span class="teaching-kicker">UNDERSTAND THE IDEA</span>${lessonTheory(t)}</section><section class="context-link"><h3>Connect it to a complete example</h3><p>This lesson belongs to the <strong>${escape(g.title)}</strong> chapter. Use the shared example below to connect its rules to code and observable behavior.</p></section>${workshopPanel(g,{compact:true})}<div id="lesson-discuss">${discussionPanel(d,t.reading?.discussion?'Discuss this lesson':'Discuss the chapter example')}</div>${sourceMaterial(t)}${bookSourceLine(t)}${teachingNotes(t)}${readingFooter(t)}</article>`;
  }
  function renderOutline(t){
    if(course().series)return renderSeriesOutline(t);
    const g=chapterGuide(t.chapter);if(!g)return renderLegacyOutline(t);
    const showMap=state.lessonView==='map'||(state.lessonView!=='lesson'&&t.id===g.startTopicId);
    return showMap?renderChapterMap(t,g):renderGuidedLesson(t,g);
  }
  function renderBookSidebar(list,c,p){
    if(!c.teaching)return renderLegacyBookSidebar(list,c,p);
    const filtered=state.query||state.bookmarksOnly;
    const shown=filtered?list:list.filter(t=>!t.outlineParent);
    $('#filtered-count').textContent=filtered?`${shown.length} found`:'';
    const allowed=new Set(shown.map(t=>t.id));let previousPart='';
    $('#topic-nav').innerHTML=c.chapters.map(ch=>{
      const g=c.teaching[String(ch.number)],ts=shown.filter(t=>t.chapter===ch.number);if(!ts.length||!g)return '';
      const groupId='CH'+ch.number,open=state.expanded.has(groupId)||!!state.query;
      const section=topic()?.chapter===ch.number?topic()?.sectionId:null;
      let heading='';if(state.bookNav==='domains'&&previousPart!==ch.partTitle){previousPart=ch.partTitle;heading=`<p class="sidebar-part">${escape(ch.partTitle)}</p>`;}
      return heading+`<div class="nav-chapter"><button class="domain-row ${topic()?.chapter===ch.number?'active':''}" data-action="domain-toggle" data-id="${groupId}" aria-expanded="${open}"><span class="domain-letter">${String(ch.number).padStart(2,'0')}</span><span class="domain-title">${escape(ch.title)}</span><span class="domain-chevron ${open?'open':''}">${icon('chevron')}</span></button>${open?`<div class="nav-chapter-contents"><button class="nav-map-link" data-action="map-chapter" data-chapter="${ch.number}">${icon('grid')} Chapter map &amp; example</button>${g.sections.map(s=>{
        const ids=s.topics.filter(id=>allowed.has(id));if(!ids.length)return '';
        return `<details class="nav-section" ${section===s.id||state.query?'open':''}><summary><span>${s.number}.</span> ${escape(s.title)} <small>${ids.length}</small></summary><ol>${ids.map((id,i)=>{const t=ts.find(t=>t.id===id);return `<li><button class="topic-row ${t.id===state.topicId&&state.lessonView==='lesson'?'active':''}" data-action="read-topic" data-id="${escape(t.id)}" ${t.id===state.topicId?'aria-current="page"':''}><span class="topic-id">${s.number}.${i+1}</span><span class="topic-title">${escape(t.reading?.title||t.title)}</span>${p.reviewed.includes(t.id)?`<span class="icon-sm">${icon('check')}</span>`:''}</button></li>`}).join('')}</ol></details>`;
      }).join('')}${filtered?ts.filter(t=>t.outlineParent).map(t=>`<button class="topic-row" data-action="read-topic" data-id="${escape(t.id)}"><span class="topic-title">${escape(t.title)}</span></button>`).join(''):''}</div>`:''}</div>`;
    }).join('')+(allowed.has('REF.GLOSSARY')?'<button class="nav-map-link" data-action="book-glossary">Book glossary</button>':'');
  }
  function showCoverage(){
    if(course().series)return showSeriesCoverage();
    if(!course().teaching)return showLegacyCoverage();
    let part='';modal('Course outline',`<p>Choose a chapter. Its map shows the sections in order, the problem to solve, a working example, and the practice that follows.</p><div class="course-roadmap">${course().chapters.map(ch=>{const g=course().teaching[String(ch.number)];if(!g)return '';let heading='';if(ch.partTitle!==part){part=ch.partTitle;heading=`<h3 class="roadmap-part">${escape(part)}</h3>`;}return heading+`<details class="roadmap-chapter"><summary><span>${ch.number===0?'00':String(ch.number).padStart(2,'0')}</span><strong>${escape(ch.title)}</strong><small>${g.sections.length} sections</small></summary><p>${escape(g.workshop.problem)}</p><ol>${g.sections.map(s=>`<li>${escape(s.title)} <small>(${s.topics.length} reading entries)</small></li>`).join('')}</ol><button class="btn btn-primary" data-action="map-chapter" data-chapter="${ch.number}">Open chapter map ${icon('right')}</button></details>`}).join('')}</div>`);DIALOG.classList.add('wide-dialog');
  }
  document.addEventListener('click',event=>{
    const el=event.target.closest('[data-action]');if(!el||el.disabled)return;
    switch(el.dataset.action){
      case 'read-topic':state.mode='outline';state.lessonView='lesson';chooseTopic(el.dataset.id);break;
      case 'chapter-map':state.mode='outline';state.lessonView='map';renderAll();window.scrollTo({top:0,behavior:'instant'});break;
      case 'map-chapter':if(DIALOG.open)DIALOG.close();state.mode='outline';state.lessonView='map';goChapter(el.dataset.chapter);break;
      case 'workshop-jump':$('#chapter-workshop')?.scrollIntoView({behavior:'instant',block:'start'});break;
      case 'guide-section':state.mode='outline';state.lessonView='map';renderAll();document.getElementById('map-'+el.dataset.section)?.scrollIntoView({behavior:'instant',block:'start'});break;
      case 'download-workshop':{const w=chapterGuide()?.workshop;if(!w)break;const a=document.createElement('a'),u=URL.createObjectURL(new Blob([w.code],{type:'text/plain'}));a.href=u;a.download=`${course().id}_ch${String(topic().chapter).padStart(2,'0')}_workshop.cpp`;a.click();setTimeout(()=>URL.revokeObjectURL(u),1000);break;}
    }
  });

    // Complete EPUB reading and source-led practice for the eight-book C++ series.
  // Rich text is an inert AST: neither source HTML nor imported scripts are evaluated.
  const seriesCache=new WeakMap();
  const seriesSelections={};
  function seriesIndex(){
    const c=course();if(seriesCache.has(c))return seriesCache.get(c);
    const blocks=new Map(),owners=new Map(),listings=new Map();
    for(const t of c.topics)for(const b of t.blocks||[]){blocks.set(b.id,b);owners.set(b.id,t);}
    for(const e of c.series.listings)listings.set(e.id,e);
    const result={blocks,owners,listings};seriesCache.set(c,result);return result;
  }
  function seriesChapterLabel(ch){return ch?.role==='chapter'?`Chapter ${ch.number}`:ch?.role==='front'?'Before you begin':'Reference';}
  function seriesLabel(n){return seriesChapterLabel(course().chapters.find(c=>c.number===n));}
  function seriesReference(href,file){
    if(!href||/^[a-z][a-z0-9+.-]*:/i.test(href))return null;
    try{const u=new URL(href,'https://epub.invalid/'+file);return course().series.links[decodeURIComponent(u.pathname.slice(1))+decodeURIComponent(u.hash)]||course().series.links[decodeURIComponent(u.pathname.slice(1))];}catch{return null;}
  }
  function seriesRich(nodes,depth=0){
    if(depth>15||!Array.isArray(nodes))return '';
    return nodes.map(n=>{
      if(typeof n==='string')return escape(n);if(!isObject(n))return '';
      const inner=seriesRich(n.children,depth+1);
      if(n.tag==='br')return '<br>';
      if(n.tag==='a'){
        const ref=seriesReference(n.href,n.file),url=safeURL(n.href);
        if(ref)return `<button class="series-inline-link" data-action="series-reference" data-topic="${escape(ref.topicId)}" data-block="${escape(ref.blockId)}">${inner}</button>`;
        return url?`<a href="${escape(url)}" target="_blank" rel="noopener noreferrer">${inner}</a>`:inner;
      }
      const tag=['strong','b','em','i','code','sub','sup','p','ul','ol','li','dl','dt','dd'].includes(n.tag)?n.tag:'span';return `<${tag}>${inner}</${tag}>`;
    }).join('');
  }
  function seriesStatus(e){
    const status=e?.validation?.status;
    return {'verified':'Output verified','ran':'Compiled & ran','compiled':'Compiled · fixture needed','needs-context-or-repair':'Needs context or repair','requires-cuda':'CUDA setup required','output-differs':'Output needs review','timeout':'Run needs a fixture','fixture-or-runtime-failure':'Run needs review','check-error':'Check incomplete'}[status]||({excerpt:'Source excerpt',commands:'Shell commands',output:'Source output',configuration:'Configuration',program:'Program · not verified',listing:'Source listing'}[e?.kind]||'Source listing');
  }
  function seriesBlocks(blocks,{compact=false}={}){
    return (blocks||[]).map(b=>{
      const id=`series-${b.id}`;
      if(b.kind==='pre'){
        const e=seriesIndex().listings.get(b.listingId);
        return `<section class="series-code-block ${e?.kind==='output'?'series-output':''}" id="${escape(id)}"><div class="series-code-head"><span>${escape(e?.kind==='output'?'EXPECTED OUTPUT':(b.language||'text').toUpperCase())} · ${escape(seriesStatus(e))}</span>${e&&!compact?`<button class="source-link" data-action="series-listing" data-id="${escape(e.id)}">Open in Code ${icon('right')}</button>`:''}</div><pre><code>${escape(b.code)}</code></pre></section>`;
      }
      if(/^h[1-6]$/.test(b.kind)){const level=Math.min(4,Math.max(3,Number(b.kind[1])));return `<h${level} class="series-source-heading" id="${escape(id)}">${seriesRich(b.rich)||escape(b.text)}</h${level}>`;}
      if(['ul','ol'].includes(b.kind))return `<${b.kind} id="${escape(id)}">${(b.items||[]).map(x=>`<li>${seriesRich(x)}</li>`).join('')}</${b.kind}>`;
      if(b.kind==='table')return `<div class="series-table-wrap" id="${escape(id)}"><table>${(b.rows||[]).map(row=>`<tr>${row.map(cell=>`<${cell.header?'th':'td'}>${seriesRich(cell.rich)}</${cell.header?'th':'td'}>`).join('')}</tr>`).join('')}</table></div>`;
      if(b.kind==='dl')return `<dl id="${escape(id)}">${seriesRich(b.rich)}</dl>`;
      if(b.kind==='blockquote')return `<blockquote id="${escape(id)}">${seriesRich(b.rich)||escape(b.text)}</blockquote>`;
      return `<div class="series-paragraph" id="${escape(id)}">${seriesRich(b.rich)||escape(b.text)}</div>`;
    }).join('');
  }
  function seriesBlockIds(ids){return (ids||[]).map(id=>seriesIndex().blocks.get(id)).filter(Boolean);}
  function seriesTrail(t){return `<nav class="lesson-trail" aria-label="Reading location"><button data-action="mode" data-mode="book">Book ${escape(course().series.roman)}</button><span>/</span><button data-action="chapter-map">${escape(seriesLabel(t.chapter))}</button><span>/</span><span>${escape(t.chapterTitle)}</span></nav>`;}
  function seriesSections(g){return `<ol class="chapter-map-list">${g.sections.map(s=>`<li class="map-section" id="map-${escape(s.id)}"><div class="map-section-heading"><span class="map-number">${String(s.number).padStart(2,'0')}</span><div><h3>${escape(s.title)}</h3><p>${escape(s.purpose)}</p></div></div><ol>${s.topics.map(id=>{const t=course().topics.find(t=>t.id===id);return t?`<li><button class="map-topic" data-action="read-topic" data-id="${escape(id)}"><span class="series-entry-mark">${icon(t.seriesKind==='example'?'source':'book')}</span><span><strong>${escape(t.title)}</strong><span class="map-preview">${t.seriesKind==='example'?'Worked example · ':''}${escape(t.reading?.preview||'')}</span></span>${icon('chevron')}</button></li>`:'';}).join('')}</ol></li>`).join('')}</ol>`;}
  function renderSeriesOutline(t){
    const g=chapterGuide(t.chapter);if(!g)return renderLegacyOutline(t);
    const chapterTopics=course().topics.filter(x=>x.chapter===t.chapter),listings=course().series.listings.filter(e=>e.chapter===t.chapter);
    const example=chapterTopics.find(x=>x.seriesKind==='example'&&x.blocks.some(b=>b.kind==='pre'));
    const pract=chapterTopics.flatMap(x=>x.practice||[]);
    const map=state.lessonView==='map';
    const body=map?`<div class="teaching-hero"><div><span class="teaching-kicker">CHAPTER MAP</span><h2 class="lesson-heading">${escape(g.title)}</h2><p class="chapter-problem">${escape(chapterTopics[0]?.summary||'')}</p><div class="teaching-actions"><button class="btn btn-primary" data-action="read-topic" data-id="${escape(g.startTopicId)}">Read the chapter opening ${icon('right')}</button>${example?`<button class="btn" data-action="read-topic" data-id="${escape(example.id)}">Start with a worked example</button>`:''}</div></div><aside class="chapter-goals"><span class="teaching-kicker">YOUR STUDY ROUTE</span><ol><li>Read the problem and its requirements.</li><li>Predict what the example will do.</li><li>Trace the code and compare the result.</li><li>Explain the choice, then try a change.</li></ol><p>${g.sections.length} sections · ${listings.length} source listings · ${pract.length} practice prompts</p></aside></div>${seriesSections(g)}`:
    `<div class="teaching-lesson-head"><span class="teaching-kicker">${t.seriesKind==='example'?'WORKED EXAMPLE':'READ & UNDERSTAND'}</span><h2 class="lesson-heading">${escape(t.title)}</h2><div class="lesson-jumps"><button data-action="chapter-map">Chapter map</button>${listings.length?'<button data-action="mode" data-mode="code">Code & build details</button>':''}${pract.length?'<button data-action="mode" data-mode="scenarios">Practice & discussion</button>':''}</div></div><div class="series-prose source-reader">${seriesBlocks(t.blocks)}</div>${t.seriesKind==='section'?`<section class="series-subsections"><h3>Continue through this section</h3>${(g.sections.find(s=>s.id===t.sectionId)?.topics||[]).filter(id=>id!==t.id).map(id=>`<button class="map-topic" data-action="read-topic" data-id="${escape(id)}"><span>${escape(course().topics.find(x=>x.id===id)?.title)}</span>${icon('right')}</button>`).join('')}</section>`:''}<div class="code-ref series-source-ref">Source: ${escape(t.sourceRef)}</div>`;
    return `<article class="panel teaching-reader series-reader">${seriesTrail(t)}<div class="flex justify-between items-center">${topicBadge(t)}${bookmarkButton(t)}</div>${body}${teachingNotes(t)}${readingFooter(t)}</article>`;
  }
  function seriesRoadmap(){let part='';return `<div class="course-roadmap">${course().chapters.map(ch=>{const g=chapterGuide(ch.number);let h='';if(part!==ch.partTitle){part=ch.partTitle;h=`<h3 class="roadmap-part">${escape(part)}</h3>`;}return h+`<details class="roadmap-chapter"><summary><span>${ch.role==='chapter'?String(ch.number).padStart(2,'0'):ch.role==='front'?'00':'REF'}</span><strong>${escape(ch.title)}</strong><small>${g.sections.length} sections</small></summary><ol>${g.sections.map(s=>`<li>${escape(s.title)}</li>`).join('')}</ol><button class="btn btn-primary" data-action="map-chapter" data-chapter="${ch.number}">Open chapter map ${icon('right')}</button></details>`;}).join('')}</div>`;}
  function showSeriesCoverage(){modal('Book outline',seriesRoadmap());DIALOG.classList.add('wide-dialog');}
  function renderSeriesBook(){
    const c=course(),s=c.series,n=s.counts;
    return `<article class="panel teaching-reader series-reader"><div class="series-book-hero"><div><span class="teaching-kicker">C++ FROM BEGINNER TO EXPERT · BOOK ${escape(s.roman)}</span><h2 class="lesson-heading">${escape(s.sourceTitle)}</h2><p class="reading">${escape(c.author)}</p><p>Follow the book in order, or open a chapter for its problem, examples, and practice. Your notes, bookmarks, and review marks stay with this book.</p><div class="teaching-actions"><button class="btn btn-primary" data-action="map-chapter" data-chapter="1">Start Chapter 1 ${icon('right')}</button><button class="btn" data-action="map-chapter" data-chapter="0">How to use this book</button></div></div><aside class="series-book-stats"><strong>${n.chapters}</strong><span>chapters, plus front matter and reference</span><div>${n.listings} source listings</div><div>${n.practice} practice prompts</div><div>${n.cards} recall cards</div></aside></div><section class="series-downloads"><h3>Keep the original book</h3><p>These downloads preserve the supplied PDF and EPUB, including the original page layout. The study reader follows the EPUB content.</p><div class="flex gap-3 flex-wrap"><button class="btn" data-action="series-download-book" data-kind="pdf">${icon('download')} Original PDF</button><button class="btn" data-action="series-download-book" data-kind="epub">${icon('download')} Original EPUB</button></div></section><details class="answer-reveal"><summary>About the code and practice</summary><p>The Code view separates complete program candidates, excerpts, commands, configuration, and expected output. Verification labels report the checks actually performed. Missing companion headers, CUDA hardware, and service fixtures are identified; the supplied source is preserved.</p><p>Practice uses the book’s review questions, labs, and experiments. Reveal source explanations after writing your answer. Experiment guidance describes the baseline when the book does not supply a solution to the changed program. Confidence ratings are self-assessments.</p><p>One added diagram study follows a Chapter 1 example. It is a teaching aid linked to exact source lines; the original books remain available above.</p></details><h3 class="focus-heading">Contents</h3>${seriesRoadmap()}</article>`;
  }
  function showSeriesSources(){const c=course();modal('Book and study coverage',`<p>${escape(c.provenance)}</p><p>${c.series.counts.chapters} chapters; ${c.series.counts.sourceBlocks} source blocks; ${c.series.counts.listings} source listings. The Book view contains the original PDF and EPUB downloads.</p><p>Use Outline for the source explanation, Code for listings and check results, Practice for source prompts and explanations, and Flashcards for recall. Open UML for the selected Chapter 1 example.</p><p>Export a backup before moving the app or clearing browser data. No code is executed in the browser.</p>`);}
  function seriesListing(){const s=seriesSelections[course().id]||{},list=course().series.listings.filter(e=>e.chapter===topic()?.chapter);return list.find(e=>e.id===s.id)||list.find(e=>e.topicId===topic()?.id&&e.completeCandidate)||list.find(e=>e.completeCandidate)||list[0];}
  function renderSeriesCode(t){
    const list=course().series.listings.filter(e=>e.chapter===t.chapter),e=seriesListing();
    if(!e)return `<section class="panel empty"><h2>No source listing in this reading group</h2><p>Choose a chapter with worked code, or return to its reading.</p><button class="btn" data-action="map-chapter" data-chapter="1">Open Chapter 1</button></section>`;
    const b=seriesIndex().blocks.get(e.blockId),v=e.validation;
    return `<article class="panel teaching-reader series-reader">${seriesTrail(t)}<span class="teaching-kicker">CODE & BUILD DETAILS</span><h2 class="lesson-heading">${escape(e.title)}</h2><label class="field-label" for="series-code-select">Source listings in ${escape(seriesLabel(t.chapter))}</label><select id="series-code-select" class="field">${list.map(x=>`<option value="${escape(x.id)}" ${x.id===e.id?'selected':''}>${escape(x.id)} · ${escape(x.title)} · ${escape(x.kind)} (${escape(x.language)})</option>`).join('')}</select><div class="series-code-tools"><span class="pill ${['verified','ran'].includes(v?.status)?'bg-mint text-mint-ink':'bg-peach text-peach-ink'}">${escape(seriesStatus(e))}</span><button class="btn" data-action="series-download-code" data-id="${escape(e.id)}">${icon('download')} Download source</button>${['cpp','c++'].includes(e.language)?`<button class="btn btn-primary" data-action="series-edit-in-lab" data-id="${escape(e.id)}">Edit &amp; run</button>`:''}<button class="btn" data-action="series-copy-code" data-id="${escape(e.id)}">Copy code</button><button class="source-link" data-action="read-topic" data-id="${escape(e.topicId)}">Read the full explanation ${icon('right')}</button></div><p class="code-ref">${escape(e.filename)} · ${escape(e.language)} · ${e.completeCandidate?'Contains an entry point; see the check status.':'An excerpt, command, output, or configuration; not a standalone C++ program.'}</p><pre class="full-code"><code>${escape(b.code.split('\n').map((l,i)=>`${String(i+1).padStart(3)}  ${l}`).join('\n'))}</code></pre>${v?`<section class="series-check"><h3>${escape(seriesStatus(e))}</h3><p>${escape(v.detail||'Compiled and executed with the supplied exact expected-output fixture.')}</p>${v.buildCommand?`<h4>Build from the source ZIP root</h4><pre><code>mkdir -p cpp_series/build\n${escape(v.buildCommand)}</code></pre>${v.project?'<p>This is a multi-file example. The ZIP includes the separated project files used by this command; the single listing download preserves the book’s combined presentation.</p>':''}${['verified','ran','output-differs'].includes(v.status)?`<pre><code>./cpp_series/build/${escape(e.id)}</code></pre>`:''}`:''}${typeof v.stdout==='string'?`<h4>Observed standard output</h4><pre class="expected-output"><code>${escape(v.stdout||'(no standard output)')}</code></pre>`:''}${v.stderr?`<details class="answer-reveal"><summary>Observed standard error</summary><pre><code>${escape(v.stderr)}</code></pre></details>`:''}${v.diagnostics?`<details class="answer-reveal"><summary>Compiler diagnostics</summary><pre><code>${escape(v.diagnostics)}</code></pre></details>`:''}<p class="code-ref">Validation: ${escape(course().series.verification?.compiler||'GCC')} · C++20. Timing and platform behavior can differ on another machine.</p></section>`:'<p class="reading">Use this listing in the context described by the book. It was not tested as an independent executable.</p>'}${e.expectedBlocks?.length?`<section class="series-prose"><h3>Expected behavior from the book</h3>${seriesBlocks(seriesBlockIds(e.expectedBlocks),{compact:true})}</section>`:''}${e.explanationBlocks?.length?`<section class="series-prose"><h3>Why it works</h3>${seriesBlocks(seriesBlockIds(e.explanationBlocks),{compact:true})}</section>`:''}${e.runBlocks?.length?`<details class="answer-reveal"><summary>Original build or inspection commands</summary><p>These commands are preserved from the book. They may name companion files or services that are not in the supplied archive.</p><div class="series-prose">${seriesBlocks(seriesBlockIds(e.runBlocks),{compact:true})}</div></details>`:''}${teachingNotes(t)}</article>`;
  }
  function renderSeriesPractice(){
    const item=activeItem(),list=deck();if(!item)return deckEmpty();const i=list.findIndex(e=>e.id===item.id),rating=progress().scenarios[item.id];
    return `<article class="panel teaching-reader series-reader"><div class="flex justify-between gap-3 flex-wrap">${scopeControl()}<button class="btn" data-action="shuffle">${icon('shuffle')} Shuffle</button></div><div class="series-practice-head"><span class="teaching-kicker">SOURCE PRACTICE · ${i+1} OF ${list.length}</span><h2 class="lesson-heading">${escape(item.title)}</h2><p class="reading">${escape(item.prompt)}</p></div><div class="series-prose">${seriesBlocks(seriesBlockIds(item.promptBlocks),{compact:true})}</div><div class="flex gap-3 flex-wrap"><button class="btn" data-action="read-topic" data-id="${escape(item.sourceTopicId)}">Read the source context ${icon('right')}</button>${item.listingId?`<button class="btn" data-action="series-listing" data-id="${escape(item.listingId)}">Open the example code</button>`:''}</div><section class="teaching-notes"><label for="series-response">Your answer, prediction, or test evidence</label><textarea class="notes-area" id="series-response" data-practice="${escape(item.id)}" maxlength="20000" placeholder="State the rule, trace the example, and explain your result.">${escape(progress().notes[item.id]||'')}</textarea></section><details class="answer-reveal"><summary>Get a hint</summary><p>${escape(item.hint)}</p></details><details class="answer-reveal series-answer"><summary>${escape(item.answerLabel||'Compare with the source explanation')}</summary><div class="series-prose">${item.answer?`<p>${escape(item.answer)}</p>`:''}${seriesBlocks(seriesBlockIds(item.answerBlocks),{compact:true})}${!item.answer&&!item.answerBlocks?.length?'<p>The source gives no separate answer for this task. Check each stated requirement, test ordinary and boundary inputs, and explain why the rules still hold.</p>':''}</div></details><section class="series-self-check"><h3>Review your reasoning</h3><p>Compare your evidence with the guidance above. This records your judgment; it does not grade an open-ended answer.</p><div class="flex gap-3 flex-wrap"><button class="btn ${rating?.correct===true?'btn-success':'btn-primary'}" data-action="series-rate" data-rating="known">${icon('check')} I can explain it</button><button class="btn ${rating?.correct===false?'btn-primary':''}" data-action="series-rate" data-rating="again">${icon('rotate')} Practice again</button></div><p class="code-ref">${rating?`Last self-review: ${rating.correct?'can explain':'practice again'}.`:'Not self-reviewed yet.'}</p></section><div class="teaching-footer"><button class="btn" data-action="previous" ${i<=0?'disabled':''}>${icon('left')} Previous prompt</button><button class="btn" data-action="next" ${i>=list.length-1?'disabled':''}>Next prompt ${icon('right')}</button></div></article>`;
  }
  function seriesSaveBlob(name,blob){const a=document.createElement('a'),u=URL.createObjectURL(blob);a.href=u;a.download=name;document.body.append(a);a.click();a.remove();setTimeout(()=>URL.revokeObjectURL(u),1500);}
  function seriesOpenListing(id){const e=seriesIndex().listings.get(id);if(!e)return;seriesSelections[course().id]={id};state.mode='code';state.chapter=String(e.chapter);state.query='';state.domain='all';state.bookmarksOnly=false;chooseTopic(e.topicId);}
  function renderSeriesUML(t){if(course().diagrams[String(t.chapter)])return renderUML(t).replace('<article class="panel">','<article class="panel series-diagram">');return `<section class="panel empty"><h2>Trace a Chapter 1 example</h2><p>Select a chapter with a diagram study to inspect matching source excerpts and adjacent theory. Book I includes studies for every core chapter and its four practical appendices.</p><button class="btn btn-primary" data-action="series-uml-start">Open the diagram study ${icon('right')}</button></section>`;}
  document.addEventListener('click',async event=>{
    const el=event.target.closest('[data-action]');if(!el||el.disabled||!course().series)return;
    switch(el.dataset.action){
      case 'series-listing':seriesOpenListing(el.dataset.id);break;
      case 'series-reference':state.mode='outline';state.lessonView='lesson';state.chapter='all';state.query='';state.domain='all';state.bookmarksOnly=false;await chooseTopic(el.dataset.topic);document.getElementById('series-'+el.dataset.block)?.scrollIntoView({behavior:'instant',block:'center'});break;
      case 'series-download-book':{const kind=el.dataset.kind,a=course().series.assets[kind];if(!a)break;if(a.base64?.$asset){await Library.downloadAsset(a.base64,a.name);break;}const raw=atob(a.base64);seriesSaveBlob(a.name,new Blob([Uint8Array.from(raw,c=>c.charCodeAt(0))],{type:kind==='pdf'?'application/pdf':'application/epub+zip'}));break;}
      case 'series-download-code':{const e=seriesIndex().listings.get(el.dataset.id);if(e){const code=seriesIndex().blocks.get(e.blockId).code;seriesSaveBlob(e.filename.split('/').pop(),new Blob([code.endsWith('\n')?code:code+'\n'],{type:'text/plain;charset=utf-8'}));}break;}
      case 'series-copy-code':{const e=seriesIndex().listings.get(el.dataset.id);if(!e)break;const code=seriesIndex().blocks.get(e.blockId).code;try{await navigator.clipboard.writeText(code);toast('Source code copied.');}catch{modal('Copy source code',`<p>Select this text and copy it. The source download also contains the exact code.</p><textarea class="field series-copy" readonly>${escape(code)}</textarea>`);$('.series-copy').select();}break;}
      case 'series-rate':{const item=activeItem();if(!item)break;const old=progress().scenarios[item.id],correct=el.dataset.rating==='known';progress().scenarios[item.id]={choice:correct?0:1,correct,firstCorrect:old?old.firstCorrect:correct,attempts:(old?.attempts||0)+1,updatedAt:new Date().toISOString()};state.reviewHoldId=item.id;renderAll();toast(correct?'Self-review saved.':'Saved for another look.');break;}
      case 'series-uml-start':state.mode='uml';goChapter(1);break;
    }
  });
  document.addEventListener('change',e=>{if(e.target.id==='series-code-select')seriesOpenListing(e.target.value);});
  document.addEventListener('input',e=>{if(e.target.id==='series-response'){progress().notes[e.target.dataset.practice]=e.target.value;remember();}});
  function cleanSeriesRich(nodes,depth=0){
    if(depth>15||!Array.isArray(nodes))return [];
    return nodes.slice(0,2000).map(n=>typeof n==='string'?text(n,100000):isObject(n)?{tag:['strong','b','em','i','code','a','sub','sup','br','span','p','ul','ol','li','dl','dt','dd'].includes(n.tag)?n.tag:'span',children:cleanSeriesRich(n.children,depth+1),href:text(n.href,3000),file:text(n.file,1000)}:'');
  }
  function cleanSeriesBlocks(blocks){return blocks.slice(0,3000).filter(b=>isObject(b)&&validId(b.id)&&['h1','h2','h3','h4','h5','h6','p','pre','ul','ol','table','blockquote','dl'].includes(b.kind)).map(b=>({id:b.id,kind:b.kind,text:text(b.text,200000),rich:cleanSeriesRich(b.rich),code:text(b.code,200000),language:text(b.language,40),listingId:validId(b.listingId)?b.listingId:undefined,sourceFile:text(b.sourceFile,1000),anchors:(b.anchors||[]).map(x=>text(x,1000)),context:text(b.context,2000),headingContext:text(b.headingContext,3000),items:Array.isArray(b.items)?b.items.slice(0,2000).map(x=>cleanSeriesRich(x)):[],rows:Array.isArray(b.rows)?b.rows.slice(0,1000).map(row=>Array.isArray(row)?row.slice(0,100).filter(isObject).map(cell=>({header:!!cell.header,rich:cleanSeriesRich(cell.rich)})):[]):[]}));}
  function cleanSeriesPractice(items){return (Array.isArray(items)?items:[]).slice(0,1000).filter(e=>isObject(e)&&validId(e.id)).map(e=>({id:e.id,title:text(e.title,1000),prompt:text(e.prompt,20000),promptBlocks:(e.promptBlocks||[]).filter(validId),answer:text(e.answer,50000),answerBlocks:(e.answerBlocks||[]).filter(validId),answerLabel:text(e.answerLabel,1000),hint:text(e.hint,20000),sourceTopicId:validId(e.sourceTopicId)?e.sourceTopicId:undefined,listingId:validId(e.listingId)?e.listingId:undefined}));}

  // Pure compiler request / teaching-test helpers, shared by the app and its tests.
const LAB_COMPILER = 'g142';
const LAB_ENDPOINT = 'https://godbolt.org/api/compiler/g142/compile';
const cppString = value => JSON.stringify(String(value)).replace(/\\u([0-9a-f]{4})/gi,'\\u$1');
function labBuildProgram(code, question, mode='run', nonce='local') {
  const source = '#line 1 "main.cpp"\n' + code + '\n';
  if (!question) return source;
  if (mode === 'run') return source + '#line 1 "study_driver.cpp"\n' + question.driver + '\n';
  if (!/^[a-zA-Z0-9_-]+$/.test(nonce)) throw new Error('Invalid test run identifier');
  const checks = question.tests.map(t=>`  check(${cppString(t.id)}, ${t.expectedExpression ? `study_lab::repr(${t.expectedExpression})` : cppString(t.expected)}, []{ return ${t.expression}; });`).join('\n');
  return source + `#line 1 "study_tests.cpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <exception>
namespace study_lab {
  template<class T> std::string repr(const T& v);
  template<class T> std::string repr(const std::vector<T>& v);
  template<class A, class B> std::string repr(const std::pair<A,B>& v);
  template<class T> std::string repr(const T& v) { std::ostringstream out; out << std::boolalpha << v; return out.str(); }
  template<class A, class B> std::string repr(const std::pair<A,B>& v) { return "("+repr(v.first)+", "+repr(v.second)+")"; }
  template<class T> std::string repr(const std::vector<T>& v) { std::string out="["; for(std::size_t i=0;i<v.size();++i) { if(i)out+=", "; out+=repr(v[i]); } return out+"]"; }
  std::string hex(const std::string& s) { static const char* digits="0123456789abcdef"; std::string out; for(unsigned char c:s){out+=digits[c>>4];out+=digits[c&15];} return out; }
}
int main() {
  int failures=0;
  auto check=[&](const char* id, const std::string& expected, auto action) {
    std::string actual; bool ok=false;
    try { actual=study_lab::repr(action()); ok=actual==expected; }
    catch(const std::exception& e){ actual=std::string("exception: ")+e.what(); }
    catch(...){ actual="exception: unknown"; }
    if(!ok)++failures;
    std::cout << "\\n@STUDY:${nonce}|" << id << "|" << (ok?"PASS":"FAIL") << "|" << study_lab::hex(actual) << "|" << study_lab::hex(expected) << "\\n";
  };
${checks}
  return failures ? 1 : 0;
}
`;
}
function labRequest(source, stdin='') {
  return {source,lang:'c++',allowStoreCodeDebug:false,options:{userArguments:'-std=c++20 -Wall -Wextra -pedantic -pthread -fdiagnostics-color=never',compilerOptions:{executorRequest:true},filters:{execute:true},executeParameters:{args:[],stdin},tools:[],libraries:[]}};
}
function labText(lines) {
  return (Array.isArray(lines)?lines.map(x=>typeof x==='string'?x:String(x?.text??'')).join('\n'):String(lines??'')).replace(/\x1b\[[0-9;]*[A-Za-z]/g,'').slice(0,160000);
}
function unhex(s) {
  if(s.length%2||!/^[0-9a-f]*$/i.test(s))throw new Error('Malformed test record');
  return new TextDecoder().decode(Uint8Array.from(s.match(/../g)||[],x=>parseInt(x,16)));
}
function labParseResponse(payload, question=null, nonce='local') {
  if(!payload || typeof payload!=='object' || !('code' in payload))throw new Error('The compiler service returned an unexpected response. Your code is still saved.');
  const build=payload.buildResult||payload, run=payload.execResult||payload;
  const compiled=build.code===0;
  const diagnostics=labText(build.stderr)+'\n'+labText(build.stdout);
  let stdout=compiled&&run.didExecute?labText(run.stdout):'', stderr=compiled&&run.didExecute?labText(run.stderr):'';
  const records=new Map(), prefix=`@STUDY:${nonce}|`, remainder=[];
  for(const line of stdout.split('\n')){
    if(question&&line.startsWith(prefix)){
      const [id,status,actual,expected,...extra]=line.slice(prefix.length).split('|');
      if(!extra.length && ['PASS','FAIL'].includes(status) && question.tests.some(t=>t.id===id) && !records.has(id)){
        try { const a=unhex(actual),e=unhex(expected);records.set(id,{actual:a,expected:e,passed:status==='PASS'&&a===e});continue; } catch{}
      }
    }
    remainder.push(line);
  }
  const cases=question?question.tests.map(t=>({...t,...records.get(t.id),passed:records.get(t.id)?.passed===true,missing:!records.has(t.id)})):[];
  const completed=compiled && !!run.didExecute && !run.timedOut && !run.truncated;
  return {compiled,executed:!!run.didExecute,code:run.code,stdout:remainder.join('\n').replace(/\n+$/,''),stderr,diagnostics:diagnostics.trim(),timedOut:!!run.timedOut,truncated:!!run.truncated,cases,passed:!!question&&completed&&run.code===0&&cases.length>0&&cases.every(c=>c.passed)};
}
function labDiagnosticHelp(diagnostics) {
  if(/expected ['‘];['’]/.test(diagnostics))return 'A statement may be missing its ending semicolon (;). Check the reported line and the line just before it.';
  if(/not declared in this scope|undeclared identifier/.test(diagnostics))return 'The compiler cannot find this name. Check spelling, where the name is declared, and whether its header is included.';
  if(/redefinition of.*main|multiple definition.*main/.test(diagnostics))return 'A question supplies main() for you. Keep only the requested function or class in the editor, or use the Playground for a complete program.';
  if(/no matching function|cannot convert|invalid conversion/.test(diagnostics))return 'The value types do not match this operation. Compare the parameter types, return type, and the values passed at the reported line.';
  if(/undefined reference/.test(diagnostics))return 'A named function was declared but no matching definition was linked. Check the required signature and whether every called function has a body.';
  if(/expected.*[}]/.test(diagnostics))return 'A brace may be missing or unmatched. Check where each function, loop, and class begins and ends.';
  if(/fatal error:.*No such file/.test(diagnostics))return 'This header is unavailable in the online environment. These labs use the standard library; local libraries and CUDA need their own build setup.';
  return 'Start with the first error. Check its file and line, then compare your function signature with the question. Later errors may be caused by the first one.';
}

// Check a whole book program against its recorded input, output, and exit status.
// A program can have main(), so it cannot be joined with the function-test main().
function labCheckWorkedResult(result, check) {
  // The service returns lines, not an exact final-newline byte count.
  // Keep trailing spaces within a line: several textbook examples print them.
  const clean = value => String(value ?? '').replace(/\r\n/g,'\n').replace(/\n+$/,'');
  const expected = `Exit: ${check.exitCode}\nstdout:\n${clean(check.stdout)||'(empty)'}\nstderr:\n${clean(check.stderr)||'(empty)'}`;
  const actual = result.compiled && result.executed
    ? `Exit: ${result.code}\nstdout:\n${clean(result.stdout)||(result.stdout===''?'(empty)':result.stdout)}\nstderr:\n${clean(result.stderr)||'(empty)'}`
    : 'No completed result';
  const passed = result.compiled && result.executed && !result.timedOut && !result.truncated &&
    result.code===check.exitCode && clean(result.stdout)===clean(check.stdout) && clean(result.stderr)===clean(check.stderr);
  return {id:check.id,label:check.label,expression:`stdin: ${check.input||'(empty)'}`,
    expected,actual,hint:check.hint,passed,missing:!result.compiled||!result.executed};
}

    // User-triggered online compilation. Reading, editing, and progress remain local.
  const labContent=Library?Library.labData:JSON.parse(document.getElementById('coding-content').textContent);
  const labCatalog=labContent.questions,labWorked=labContent.workedPrograms||[];
  const labPlayground='#include <iostream>\n\nint main() {\n    int value = 0;\n    std::cin >> value;\n    std::cout << value * 2 << "\\n";\n}\n';
  const labSessions=new Map(), labUndo=new Map();
  let labActive=null;
  function labQuestions(){return labCatalog.filter(q=>q.courseId===state.courseId);}
  function labWorkedExamples(){return labWorked.filter(w=>w.courseId===state.courseId);}
  function labSelection(){const p=progress();const ids=labQuestions().map(q=>q.id).concat(labWorkedExamples().map(w=>w.id));const id=p.codingSelected;return id==='playground'||ids.includes(id)||p.coding?.[id]?.initialCode!==undefined?id:ids[0]||'playground';}
  function labQuestion(){return labQuestions().find(q=>q.id===labSelection())||null;}
  function labWorkedExample(){return labWorkedExamples().find(w=>w.id===labSelection())||null;}
  function labKey(){return state.courseId+'/'+labSelection();}
  function labDraft(){const p=progress();p.coding||={};const id=labSelection(),q=labQuestion(),w=labWorkedExample();return p.coding[id]||=({code:q?.starter??w?.source??labPlayground,input:q?.sampleInput??w?.sampleInput??'21\n',title:q?.title??w?.title??'Playground'});}
  function labSession(){const key=labKey();if(!labSessions.has(key))labSessions.set(key,{message:'Ready. Write your code, then run it or check the tests.',result:null,code:null});return labSessions.get(key);}
  function labStop(message='Stopped waiting. The remote service may finish the submitted run.'){
    if(!labActive)return;const active=labActive;labActive=null;active.controller.abort();const s=labSessions.get(active.key);if(s){s.busy=false;s.message=message;}labRefresh();
  }
  function labBeforeRender(){if(labActive&&(state.mode!=='coding'||labActive.key!==labKey()))labStop('Stopped waiting because you left this coding question.');}
  function labStatus(){const d=labDraft(),s=labSession();if(s.busy)return 'Compiling and running…';if(s.code!==null&&(s.code!==d.code||(s.kind==='run'&&s.input!==d.input)))return 'Edited since this result · run again';if(s.code===null&&s.message.startsWith('Ready.')&&d.checkedCode===d.code&&d.passed)return labWorkedExample()?'Original example checks passed for this draft':'All checks passed for this draft';return s.message;}
  function labWorkedGuide(w){
    if(!w?.guide)return '';
    const fields=[['problem','Problem and requirements'],['design','Design and alternative'],['invariant','What must stay true'],['trace','Concrete trace'],['failure','What can go wrong'],['maintenance','Maintaining real software']];
    return `<details class="lab-details lab-worked-guide" open><summary>Understand this chapter’s example</summary><p class="lab-small">${escape(w.guide.scope||'Chapter practice with contracts and concrete evidence.')}</p>${fields.map(([key,label])=>`<h4>${label}</h4><p>${escape(w.guide[key])}</p>`).join('')}</details>`;
  }
  function renderCodingLab(){
    const qs=labQuestions(),ws=labWorkedExamples(),q=labQuestion(),w=labWorkedExample(),d=labDraft(),id=labSelection();
    const imported=Object.entries(progress().coding||{}).filter(([,v])=>v.initialCode!==undefined);
    const solved=qs.filter(q=>{const v=progress().coding?.[q.id];return v?.passed&&v.checkedCode===v.code;}).length;
    const verified=ws.filter(x=>{const v=progress().coding?.[x.id];return v?.passed&&v.checkedCode===v.code;}).length;
    const grouped=[...new Set(ws.map(x=>x.chapter))].map(n=>`<optgroup label="${escape(ws.find(x=>x.chapter===n)?.chapterTitle||'Reference')} · ${ws.filter(x=>x.chapter===n).length} programs">${ws.filter(x=>x.chapter===n).map(x=>`<option value="${escape(x.id)}" ${id===x.id?'selected':''}>${escape(x.sourceId)} · ${escape(x.title)}</option>`).join('')}</optgroup>`).join('');
    return `<article class="coding-lab panel"><header class="lab-header"><div><span class="teaching-kicker">WRITE · RUN · UNDERSTAND</span><h2>Coding Lab<span class="lab-online-badge">Online compiler</span></h2><p>Turn an idea into working C++. Make a change, see what happens, and fix it.</p></div><div class="lab-progress"><strong>${solved}<span> / ${qs.length}</span></strong><span>questions passing</span>${ws.length?`<strong class="lab-worked-count">${verified}<span> / ${ws.length}</span></strong><span>worked programs checked</span>`:''}</div></header>
    <div class="lab-picker"><label for="lab-question">${w?'Worked example':'Challenge'} in this book</label><select id="lab-question" class="field">${qs.length?`<optgroup label="Practice questions">${qs.map((x,i)=>`<option value="${escape(x.id)}" ${id===x.id?'selected':''}>${i+1}. ${escape(x.title)} · ${escape(x.level)}</option>`).join('')}</optgroup>`:''}${grouped}<optgroup label="Your workspace"><option value="playground" ${id==='playground'?'selected':''}>Free C++ playground</option>${imported.map(([k,v])=>`<option value="${escape(k)}" ${id===k?'selected':''}>${escape(v.title||k)}</option>`).join('')}</optgroup></select></div>
    <div class="lab-layout"><section class="lab-brief" aria-label="Coding question"><div class="lab-section-label">${w?`BOOK ${escape(course().series?.roman||state.courseId)} / WORKED PROGRAM`:'01 / THE CHALLENGE'}</div><h3>${escape(q?.title??w?.title??d.title??'Free C++ playground')}</h3><p>${escape(q?.prompt??(w?`The book’s ${w.sourceId} program is ready to run. Read its source, predict the output, then check it against the recorded behavior. Experiment: ${w.experiment}`:'Write a complete C++20 program, including main(). Run it with your own input and inspect its output. This workspace has no automatic grading tests.'))}</p>${q?`<code class="lab-signature">${escape(q.signature)}</code>${labWorkedGuide(q)}<p class="lab-small">Edit the supplied function or class. We add <code>main()</code> when running or testing.</p><button class="source-link" data-action="go-chapter" data-chapter="${q.chapter}">Review: ${escape(q.chapterTitle)} ${icon('right')}</button><details class="lab-details" open><summary>Sample input &amp; output</summary><label>INPUT</label><pre>${escape(q.sampleInput||'(empty)')}</pre><label>EXPECTED OUTPUT</label><pre>${escape(q.sampleOutput)}</pre></details><details class="lab-details"><summary>${q.tests.length} public checks</summary><ul>${q.tests.map(t=>`<li>${escape(t.label)}</li>`).join('')}</ul><p class="lab-small">These checks test the stated examples and boundaries. Passing them does not prove every possible input, performance, or concurrency property.</p></details><details class="lab-details" id="lab-hints"><summary>Need a hint?</summary>${q.hints.map((h,i)=>`<details><summary>Hint ${i+1}</summary><p>${escape(h)}</p></details>`).join('')}</details><details class="lab-details" id="lab-solution"><summary>Reference solution &amp; explanation</summary><p>${escape(q.explanation)}</p><pre><code>${escape(q.solution)}</code></pre><button class="btn" data-action="lab-use-solution">Use reference in editor</button></details>`:w?`<p class="lab-small">${escape(w.sourceFilename)} · C++20 · ${escape(w.chapterTitle)}.</p>${w.environmentNote?`<p class="lab-small lab-environment-note">${escape(w.environmentNote)}</p>`:''}<button class="source-link" data-action="lab-original-listing" data-id="${escape(w.sourceId)}">Read this listing in the book ${icon('right')}</button>${w.adapted?`<div class="lab-adaptation"><strong>Single-file teaching adaptation</strong><p>${escape(w.adaptationNote)}</p></div>`:''}${labWorkedGuide(w)}<details class="lab-details" open><summary>Original input and output</summary><label>INPUT</label><pre>${escape(w.sampleInput||'(empty)')}</pre><label>RECORDED STDOUT</label><pre>${escape(w.sampleOutput||'(empty; assertions or the exit code are checked)')}</pre></details><details class="lab-details"><summary>${w.checks.length} original behavior ${w.checks.length===1?'check':'checks'}</summary><ul>${w.checks.map(x=>`<li>${escape(x.label)}${x.exitCode?` · expected exit ${x.exitCode}`:''}</li>`).join('')}</ul><p class="lab-small">Check example compares the code with the book’s original behavior. If you intentionally change its output, use Run online to inspect your new behavior. Some values depend on the compiler or platform.</p></details><details class="lab-details" id="lab-hints"><summary>Experiment guidance</summary><p>${escape(w.experiment)}</p><p>First predict what changes in the output. Then run and explain it. Reset restores the original or labeled adaptation.</p></details>`:`<p class="lab-small">Standard library · one source file · GCC 14.2 on Linux. Source excerpts may need headers and a main function. CUDA, external services, and multi-file projects need their own build environment.</p>`}</section>
    <section class="lab-workbench" aria-label="C++ editor and terminal"><div class="lab-editor-head"><div><span class="lab-file-dot"></span>main.cpp <span>C++20</span></div><span id="lab-draft-label">Saved draft</span></div><div class="lab-editor-shell"><pre id="lab-lines" aria-hidden="true">${d.code.split('\n').map((_,i)=>i+1).join('\n')}</pre><textarea id="lab-editor" aria-label="C++ source code" spellcheck="false" autocomplete="off" autocapitalize="off" wrap="off" maxlength="60000">${escape(d.code)}</textarea></div>
    <div class="lab-actions"><button class="btn btn-primary" data-action="lab-run">${icon('right')} Run online</button><button class="btn lab-check-button" data-action="lab-test" ${q||w?'':'disabled'}>${icon('checkCircle')} ${w?'Check example':'Check solution'}</button><button class="btn" data-action="lab-stop" ${labActive?.key===labKey()?'':'disabled'}>Stop waiting</button><div class="lab-secondary-actions"><button class="source-link" data-action="lab-reset">Reset</button><button class="source-link" data-action="lab-undo" ${labUndo.has(labKey())?'':'disabled'}>Undo replace</button><button class="source-link" data-action="lab-download">Download .cpp</button></div></div>
    <p class="lab-network-note">Online execution sends this editor’s code and input to <a href="https://godbolt.org" target="_blank" rel="noopener noreferrer">Compiler Explorer</a>. Internet required. <kbd>Ctrl</kbd>/<kbd>⌘</kbd> + <kbd>Enter</kbd> runs code.</p>
    <details class="lab-input-panel" open><summary>Program input <span>stdin · supplied before the run</span></summary><textarea id="lab-stdin" aria-label="Program input" spellcheck="false" maxlength="20000" rows="2">${escape(d.input)}</textarea><p>Run uses this input. ${w?'Check example uses the recorded input for each original behavior check.':'Check solution uses the question’s test cases.'}</p></details>
    <section class="lab-terminal" aria-label="Terminal output"><div class="lab-terminal-head"><span><span class="lab-terminal-dot"></span> TERMINAL</span><button data-action="lab-clear" class="source-link">Clear</button></div><div id="lab-results" aria-live="polite">${labResults()}</div><form id="lab-command-form" class="lab-command-form"><label for="lab-command">›</label><input id="lab-command" aria-label="Lab command" placeholder="run, test, hint, clear, help" autocomplete="off" spellcheck="false"><button type="submit">Enter ↵</button></form><p class="lab-terminal-foot">Lab commands and program output. Input is sent before execution; this is not a live operating-system shell.</p></section>
    <details class="lab-details"><summary>See the complete program used by Run</summary><pre id="lab-built-program"><code>${escape(labBuildProgram(d.code,q,'run'))}</code></pre></details></section></div></article>`;
  }
  function labResults(){
    const s=labSession(),r=s.result;
    const heading=`<p class="lab-run-status" role="status">${escape(labStatus())}</p>`;
    if(!r)return heading;
    const diag=r.diagnostics?`<section class="lab-diagnostics"><h4>${r.compiled?'Compiler warnings':'Build failed'}</h4><pre>${escape(r.diagnostics)}</pre>${!r.compiled?`<p class="lab-explanation">${escape(labDiagnosticHelp(r.diagnostics))}</p>`:''}${[...new Set([...r.diagnostics.matchAll(/main\.cpp:(\d+)(?::\d+)?/g)].map(m=>Number(m[1])))].slice(0,8).map(line=>`<button class="lab-line-link" data-action="lab-line" data-line="${line}">Go to editor line ${line}</button>`).join('')}</section>`:'';
    const runtime=r.compiled?`<div class="lab-exit">${r.executed?`Process exit: ${escape(r.code)}${r.timedOut?' · time limit reached':''}${r.truncated?' · output limit reached':''}`:'The service compiled the code but did not execute it.'}</div>${r.stdout?`<h4>stdout</h4><pre class="lab-stdout">${escape(r.stdout)}</pre>`:''}${r.stderr?`<h4>stderr</h4><pre class="lab-stderr">${escape(r.stderr)}</pre>`:''}${r.executed&&!r.stdout&&!r.stderr&&!r.cases.length?'<p class="lab-small">The program produced no output.</p>':''}`:'';
    const checks=r.cases.length?`<div class="lab-check-summary ${r.passed?'passed':'failed'}">${r.cases.filter(c=>c.passed).length} / ${r.cases.length} checks passed${r.passed?' · nice work':''}</div><div class="lab-case-list">${r.cases.map(c=>`<details class="lab-case ${c.passed?'passed':'failed'}" ${c.passed?'':'open'}><summary><span>${c.passed?'✓':'×'}</span> ${escape(c.label)} <em>${c.missing?'Not reached':c.passed?'Passed':'Failed'}</em></summary><div class="lab-case-values"><div><label>EXPECTED</label><pre>${escape(c.expectedLabel||c.expected)}</pre></div><div><label>ACTUAL</label><pre>${escape(c.missing?'No completed result':c.actual)}</pre></div></div>${c.passed?'':`<p>${escape(c.missing?'Fix the build or runtime error first. The program ended before this check reported a result.':c.hint)}</p>`}<details><summary>Test expression</summary><pre>${escape(c.expression)}</pre></details></details>`).join('')}</div>`:'';
    return heading+diag+runtime+checks;
  }
  function labRefresh(){if(state.mode!=='coding'||!$('#lab-results'))return;$('#lab-results').innerHTML=labResults();const qs=labQuestions(),ws=labWorkedExamples(),solved=qs.filter(q=>{const d=progress().coding?.[q.id];return d?.passed&&d.checkedCode===d.code;}).length,verified=ws.filter(w=>{const d=progress().coding?.[w.id];return d?.passed&&d.checkedCode===d.code;}).length;const count=$('.lab-progress strong');if(count)count.innerHTML=`${solved}<span> / ${qs.length}</span>`;const worked=$('.lab-worked-count');if(worked)worked.innerHTML=`${verified}<span> / ${ws.length}</span>`;const busy=!!labActive;for(const a of ['lab-run','lab-test']){const b=$(`[data-action="${a}"]`);if(b)b.disabled=busy||(a==='lab-test'&&!labQuestion()&&!labWorkedExample());}const stop=$('[data-action="lab-stop"]');if(stop)stop.disabled=!busy;}
  function labEdited(){const d=labDraft();d.code=$('#lab-editor').value;d.input=$('#lab-stdin').value;d.updatedAt=new Date().toISOString();$('#lab-lines').textContent=d.code.split('\n').map((_,i)=>i+1).join('\n');$('#lab-built-program').textContent=labBuildProgram(d.code,labQuestion(),'run');remember();$('#lab-draft-label').textContent=storageOK?'Saved draft':'Session only';labRefresh();}
  async function labRun(mode){
    if(labActive)return;const q=labQuestion(),w=labWorkedExample();if(mode==='test'&&!q&&!w){labSession().message='The playground has no grading tests. Use run to compile and execute it.';labRefresh();return;}
    const d=labDraft(),key=labKey(),s=labSession(),code=d.code,input=d.input,courseId=state.courseId,id=labSelection(),storeAtStart=store;
    const nonce='n'+crypto.getRandomValues(new Uint32Array(2)).join(''),controller=new AbortController();
    const request={key,controller};labActive=request;s.busy=true;s.result=null;s.code=code;s.input=input;s.kind=mode;s.message='Compiling and running…';labRefresh();
    const timeout=setTimeout(()=>{if(labActive===request)labStop('The compiler took too long to respond. Your draft is saved; try again later.');},60000);
    try{
      const execute=async stdin=>{
        const response=await fetch(LAB_ENDPOINT,{method:'POST',headers:{'Content-Type':'application/json','Accept':'application/json'},body:JSON.stringify(labRequest(labBuildProgram(code,q,mode,nonce),stdin)),signal:controller.signal});
        if(!response.ok)throw new Error(response.status===429?'The online compiler is busy or rate-limited. Wait a little, then run again.':`The compiler service returned HTTP ${response.status}. Try again later.`);
        return labParseResponse(await response.json(),mode==='test'&&q?q:null,nonce);
      };
      let result;
      if(mode==='test'&&w){
        const cases=[];let latest=null;
        for(const check of w.checks){
          if(labActive!==request)return;
          latest=await execute(check.input);
          cases.push(labCheckWorkedResult(latest,check));
          if(!latest.compiled||!latest.executed||latest.timedOut||latest.truncated)break;
          s.message=`Checked ${cases.length} of ${w.checks.length} original cases…`;s.result={...latest,cases:[...cases],passed:false};labRefresh();
        }
        result={...latest,cases:w.checks.map(check=>cases.find(c=>c.id===check.id)||{...labCheckWorkedResult({compiled:false,executed:false},check),missing:true}),passed:cases.length===w.checks.length&&cases.every(c=>c.passed)};
      }else result=await execute(mode==='test'?'':input);
      if(labActive!==request)return;s.result=result;s.code=code;
      s.message=!result.compiled?'Fix the first compiler error, then run again.':result.timedOut?'Execution reached its time limit. Check loop bounds and blocking input.':result.truncated?'Output was cut off. Check for excessive printing or an endless loop.':!result.executed?'Compilation completed; execution was not available.':mode==='test'?(result.passed?(w?'The original example matches its checked behavior. Now make a change, predict the result, and Run online.':'All checks passed. Explain why your code works, then try another question.'):'Some checks need attention. Compare the expected and actual values below.'):result.code===0?'Run finished successfully.':`The program ended with exit code ${result.code}. Read stderr and check the failing path.`;
      if(mode==='test'&&store===storeAtStart){const saved=store.progress[courseId]?.coding?.[id];if(saved){saved.checkedCode=code;saved.passed=result.passed;saved.checkedAt=new Date().toISOString();remember();}}
    }catch(error){if(labActive===request){s.message=error.name==='AbortError'?'Stopped waiting.':`Could not complete the run. ${error instanceof TypeError?'Check your internet connection or try again if the compiler service is unavailable.':error.message} Your draft is still here.`;s.result=null;}}
    finally{clearTimeout(timeout);if(labActive===request){s.busy=false;labActive=null;}else if(labActive?.key!==key)s.busy=false;labRefresh();}
  }
  function labReplace(code){labStop();const d=labDraft();labUndo.set(labKey(),{code:d.code,input:d.input});d.code=code;d.updatedAt=new Date().toISOString();remember();renderWorkspace();$('#lab-editor')?.focus();}
  function labOpenSource(id,title,code){labStop();const p=progress();p.coding||={};const key='source-'+id;p.coding[key]||={code:code.slice(0,60000),initialCode:code.slice(0,60000),input:'',title:title.slice(0,200)};p.codingSelected=key;state.mode='coding';renderAll();$('#workspace').scrollIntoView({block:'start'});}
  function labDownload(){const q=labQuestion(),d=labDraft();const blob=new Blob([labBuildProgram(d.code,q,'run')],{type:'text/plain;charset=utf-8'}),url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=labSelection()+'.cpp';a.click();setTimeout(()=>URL.revokeObjectURL(url),1000);toast('Complete program downloaded. Build with g++ -std=c++20 -pthread file.cpp');}
  function labCommand(command){
    switch(command.trim().toLowerCase()){
      case 'run':labRun('run');break;case 'test':case 'check':labRun('test');break;
      case 'clear':{const s=labSession();s.result=null;s.message='Terminal cleared.';s.code=null;labRefresh();break;}
      case 'stop':labStop();break;
      case 'hint':case 'solution':{const el=$('#lab-'+(command.trim().toLowerCase()==='hint'?'hints':'solution'));if(el){el.open=true;el.scrollIntoView({block:'nearest'});}else{labSession().message='The playground has no hints or reference solution.';labRefresh();}break;}
      case 'download':labDownload();break;
      case 'reset':labReplace(labQuestion()?.starter??labWorkedExample()?.source??labDraft().initialCode??labPlayground);break;
      default:labSession().message='Commands: run · test · hint · solution · reset · download · clear · stop · help. Edit C++ in the editor above; this command box is not a shell.';labRefresh();
    }
  }
  document.addEventListener('click',event=>{
    const el=event.target.closest('[data-action]');if(!el||el.disabled)return;
    switch(el.dataset.action){
      case 'lab-run':labRun('run');break;case 'lab-test':labRun('test');break;case 'lab-stop':labStop();break;case 'lab-clear':labCommand('clear');break;case 'lab-reset':labCommand('reset');break;case 'lab-download':labDownload();break;
      case 'lab-open-question':{progress().codingSelected=el.dataset.id;remember();state.mode='coding';renderAll();break;}
      case 'lab-use-solution':if(labQuestion())labReplace(labQuestion().solution);break;
      case 'lab-undo':{const prev=labUndo.get(labKey());if(prev){labStop();Object.assign(labDraft(),prev);labUndo.delete(labKey());remember();renderWorkspace();}break;}
      case 'lab-line':{const editor=$('#lab-editor'),line=Number(el.dataset.line),parts=editor.value.split('\n');const start=parts.slice(0,line-1).reduce((n,s)=>n+s.length+1,0);editor.focus();editor.setSelectionRange(start,start+(parts[line-1]?.length||0));editor.scrollTop=Math.max(0,(line-4)*21);$('#lab-lines').scrollTop=editor.scrollTop;break;}
      case 'lab-original-listing':seriesOpenListing(el.dataset.id);break;
      case 'edit-in-lab':{const e=course().examples?.[String(topic().chapter)]?.[codeKind];if(e)labOpenSource('ch'+topic().chapter+'-'+codeKind,e.filename,e.code);break;}
      case 'series-edit-in-lab':{const e=seriesIndex().listings.get(el.dataset.id),b=e&&seriesIndex().blocks.get(e.blockId),w=labWorked.find(x=>x.courseId===state.courseId&&(x.sourceId===e?.id||x.sourcePartIds?.includes(e?.id)));if(w){labStop();progress().codingSelected=w.id;state.mode='coding';renderAll();$('#workspace').scrollIntoView({block:'start'});}else if(b)labOpenSource(e.id,e.title,b.code);break;}
    }
  });
  document.addEventListener('change',event=>{if(event.target.id==='lab-question'){labStop();progress().codingSelected=event.target.value;remember();renderWorkspace();}});
  document.addEventListener('input',event=>{if(['lab-editor','lab-stdin'].includes(event.target.id))labEdited();});
  document.addEventListener('scroll',event=>{if(event.target?.id==='lab-editor')$('#lab-lines').scrollTop=event.target.scrollTop;},true);
  document.addEventListener('submit',event=>{if(event.target.id==='lab-command-form'){event.preventDefault();const input=$('#lab-command'),command=input.value;input.value='';labCommand(command);}});
  document.addEventListener('keydown',event=>{if(event.target.id!=='lab-editor')return;if(event.key==='Tab'&&!event.shiftKey){event.preventDefault();const el=event.target;el.setRangeText('    ',el.selectionStart,el.selectionEnd,'end');labEdited();}if(event.key==='Enter'&&(event.ctrlKey||event.metaKey)){event.preventDefault();labRun('run');}});

  function writeRoute(){
    const url=new URL(location.href);url.searchParams.set('course',state.courseId);
    if(topic())url.searchParams.set('chapter',String(topic().chapter));else url.searchParams.delete('chapter');
    if(state.topicId)url.searchParams.set('topic',state.topicId);else url.searchParams.delete('topic');
    url.searchParams.set('view',state.mode);url.searchParams.set('lesson',state.lessonView||'lesson');
    if(state.cardId&&state.mode==='flashcards')url.searchParams.set('card',state.cardId);else url.searchParams.delete('card');
    if(state.scenarioId&&state.mode==='scenarios')url.searchParams.set('practice',state.scenarioId);else url.searchParams.delete('practice');
    const route=url.href;if(route!==location.href)history.replaceState(null,'',route);
  }
  async function readRoute(){
    const params=new URL(location.href).searchParams;
    const id=params.get('course');if(id&&courses().some(c=>c.id===id))state.courseId=id;
    if(Library)await Library.ensureCourse(state.courseId);
    restorePosition();
    const n=params.get('chapter'),c=course();
    if(n!==null){const t=c.topics.find(t=>String(t.chapter)===n);if(t){state.chapter=n;state.topicId=c.teaching?.[n]?.startTopicId||t.id;state.lessonView='map';}}
    const t=params.get('topic');if(t&&c.topics.some(x=>x.id===t)){state.topicId=t;state.chapter=String(topic().chapter);}
    if(params.get('view'))state.mode=params.get('view');
    if(params.get('lesson'))state.lessonView=params.get('lesson');
    state.cardId=params.get('card')||state.cardId;state.scenarioId=params.get('practice')||state.scenarioId;
  }
  if(Library)await readRoute();else restorePosition();
  renderShell();
  window.addEventListener('popstate',async()=>{if(Library){await readRoute();renderAll();}});
  document.addEventListener('click',e=>{if(e.target.closest('[data-action="retry-load"]'))renderAll();});
})().catch(error=>{const host=document.getElementById('app');host.textContent='Unable to load Study Studio. '+error.message+' ';const button=document.createElement('button');button.textContent='Retry';button.onclick=()=>location.reload();host.append(button);});
