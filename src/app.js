/* Study Studio — framework-free JavaScript with compiled Tailwind CSS.
 * No runtime dependencies, analytics, remote calls, or secrets.
 * All user-provided text is escaped before HTML rendering.
 */
(() => {
  'use strict';
  const seed = JSON.parse(document.getElementById('study-content').textContent);
  const KEY = 'patterns-study-studio:v1';
  const APP = document.getElementById('app');
  const DIALOG = document.getElementById('app-dialog');
  const $ = (selector, parent = document) => parent.querySelector(selector);
  const escape = value => String(value ?? '').replace(/[&<>"']/g, char => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[char]));
  const isObject = x => !!x && typeof x === 'object' && !Array.isArray(x);
  const parseJSON = text => JSON.parse(text, (key,value) => ['__proto__','prototype','constructor'].includes(key) ? undefined : value);
  const safeURL = value => {try {const u = new URL(value);return ['https:','http:'].includes(u.protocol) ? u.href : '';} catch {return '';}};
  const fraction = (n,d) => d ? Math.round(n / d * 100) : 0;
  const blankProgress = () => ({reviewed:[], bookmarks:[], cards:{}, scenarios:{}, notes:{}});
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
  function availableModes(){return modes.filter(m=>!['uml','code','lectures'].includes(m.id)||(m.id==='uml'&&Object.keys(course().diagrams||{}).length)||(m.id==='code'&&Object.keys(course().examples||{}).length)||(m.id==='lectures'&&Object.keys(course().lectures||{}).length));}
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
      (!q||`${JSON.stringify(t.blocks||[])} ${JSON.stringify(t.reading||{})} ${t.id} ${t.title} ${t.chapterTitle||''} ${(t.tasks||[]).join(' ')} ${t.summary} ${t.concepts.map(c=>`${c.term} ${c.definition}`).join(' ')}`.toLocaleLowerCase().includes(q)));
  }
  function deck(mode=state.mode) {
    let list=visibleTopics();const current=topic();const p=progress();
    if(state.scope==='topic')list=list.filter(t=>t.id===state.topicId);
    if(state.scope==='domain'&&current)list=list.filter(t=>t.domain===current.domain);
    if(state.scope==='chapter'&&current)list=list.filter(t=>t.chapter===current.chapter);
    if(state.scope==='bookmarked')list=list.filter(t=>p.bookmarks.includes(t.id));
    let items=mode==='flashcards'?list.flatMap(t=>t.cards.map(c=>({...c,topic:t}))):list.filter(t=>t.scenario).map(t=>({...t.scenario,topic:t}));
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
  function totals() {const t=course().topics;return {topics:t.length,cards:t.reduce((n,t)=>n+t.cards.length,0),scenarios:t.filter(t=>t.scenario).length};}
  function stats() {
    const p=progress(), all=course().topics, ids=new Set(all.map(t=>t.id));
    const cards=new Set(all.flatMap(t=>t.cards.map(c=>c.id)));const scenarios=new Set(all.filter(t=>t.scenario).map(t=>t.scenario.id));
    const reviewed=p.reviewed.filter(id=>ids.has(id)).length;
    const known=Object.entries(p.cards).filter(([id,x])=>cards.has(id)&&x.rating==='known').length;
    const attempts=Object.entries(p.scenarios).filter(([id])=>scenarios.has(id));
    return {reviewed,known,attempted:attempts.length,accuracy:fraction(attempts.filter(([,x])=>x.firstCorrect).length,attempts.length)};
  }
  function toast(message) {const el=$('#toast');el.textContent=message;el.hidden=false;clearTimeout(state.toastTimer);state.toastTimer=setTimeout(()=>el.hidden=true,3200);}

  function renderShell() {
    APP.innerHTML=`
      <div id="drawer-overlay" class="drawer-overlay" data-action="close-menu"></div>
      <aside id="sidebar" class="sidebar" aria-label="Curriculum and topic navigation">
        <div class="px-6 pt-7 pb-6">
          <div class="flex items-center gap-3"><span class="brand-mark">${icon('book')}</span><div><div class="text-lg font-bold tracking-tight">Study Studio<span class="text-brand">.</span></div><div class="text-[10px] text-muted mt-0.5">A little progress, every day.</div></div><button class="icon-button mobile-only ml-auto" data-action="close-menu" aria-label="Close navigation">${icon('close')}</button></div>
        </div>
        <div class="px-5 pb-5">
          <div class="flex items-center justify-between mb-2"><label for="course-select" class="eyebrow">Study library</label><button class="text-muted hover:text-brand icon-sm" data-action="add-topic" aria-label="Add your own study topic" title="Add your own study topic">${icon('plus')}</button></div>
          <div class="course-control"><select id="course-select" aria-label="Choose curriculum"></select><span class="select-caret icon-sm">${icon('down')}</span></div>
          <div class="search-box mt-4">${icon('search')}<input id="topic-search" type="search" placeholder="Find a topic or concept" aria-label="Search topics" autocomplete="off"><kbd class="key">/</kbd></div>
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
  function renderAll() {
    synchronize();
    document.body.classList.toggle('large-reading',!!store.ui.large);
    const c=course(), t=totals();
    $('#course-select').innerHTML=courses().map(c=>`<option value="${escape(c.id)}" ${c.id===state.courseId?'selected':''}>${escape(c.title)}</option>`).join('');
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
    $('#mode-tabs').innerHTML=availableModes().map(m=>`<button class="mode-tab ${state.mode===m.id?'active':''}" data-action="mode" data-mode="${m.id}" aria-pressed="${state.mode===m.id}"><span class="mode-icon ${m.color}">${icon(m.icon)}</span><span class="min-w-0"><span class="mode-label block text-[13px] font-semibold ${state.mode===m.id?'text-brand':'text-ink'}">${m.label}</span><span class="mode-count block mt-1 text-[10px] text-muted">${m.id==='outline'?(c.teaching?`${c.chapters.length} chapter maps`:`${t.topics} lessons`):m.id==='flashcards'?`${t.cards} cards`:m.id==='scenarios'?`${t.scenarios} cases`:m.id==='lectures'?`${Object.keys(c.lectures||{}).length} chapters`:m.id==='uml'?`${Object.values(c.diagrams||{}).reduce((n,d)=>n+1+(d.extra_overviews||[]).length,0)} diagram views`:`${Object.keys(c.examples||{}).length} working examples`}</span></span></button>`).join('');
    $('#workspace-label').textContent={outline:'READ, UNDERSTAND, AND DISCUSS',flashcards:'TRAIN YOUR RECALL',scenarios:'PUT IT INTO PRACTICE',uml:'FOLLOW THE RELATIONSHIPS',code:'READ THE WORKING C++',lectures:'LECTURE MATERIALS'}[state.mode];
    $('#filter-status').innerHTML=state.query||state.bookmarksOnly?`<button class="filter-chip" data-action="clear-filters">${escape(state.query?`“${state.query.length>22?state.query.slice(0,22)+'…':state.query}”`:'Saved topics')} ${icon('close')}</button>`:'';
    $('#pack-notice').textContent=isBook()?`${c.title} · ${c.author||'Dr. Charles Dorner'} · Offline study companion`:'Your study material · Stored locally in this browser.';
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
    if(!course().topics.length){$('#workspace').innerHTML=`<section class="panel empty fade-in">${icon('book')}<h2>Make this space yours.</h2><p>Add your own topic, question-and-answer cards, and a practice scenario. Or import a study pack to start another curriculum.</p><div class="flex flex-wrap gap-3 justify-center"><button class="btn btn-primary" data-action="add-topic">${icon('plus')} Add a topic</button><button class="btn" data-action="import">${icon('upload')} Import study pack</button></div></section>`;return;}
    if(!visibleTopics().length){$('#workspace').innerHTML=`<section class="panel empty fade-in">${icon('search')}<h2>No matching topics.</h2><p>Try another search, select all domains, or clear the saved-topic filter.</p><button class="btn btn-primary" data-action="clear-filters">Clear filters</button></section>`;return;}
    const t=topic();
    const content=state.mode==='lectures'?renderLectures(t):state.mode==='uml'?renderUML(t):state.mode==='code'?renderCode(t):state.mode==='outline'?(t.kind==='glossary'?renderGlossaryOutline(t):renderOutline(t)):state.mode==='flashcards'?renderFlashcards():renderScenario();
    $('#workspace').innerHTML=`<div class="content-grid fade-in ${(['uml','code','lectures'].includes(state.mode)||(state.mode==='outline'&&chapterGuide()))?'wide-workspace':''}"><section class="min-w-0">${content}</section>${state.mode==='outline'&&chapterGuide()?'':`<aside class="right-rail" aria-label="Topic support and progress">${renderRail(t)}</aside>`}</div>`;
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
    return `<div class="panel p-5 sm:p-6"><div class="flex items-center justify-between gap-2 flex-wrap mb-5">${scopeControl()}<button class="btn btn-text" data-action="shuffle" aria-label="Shuffle flashcards">${icon('shuffle')} Shuffle</button></div><div class="flex items-center justify-between gap-3 mb-4"><div class="flex items-center gap-2 flex-wrap">${topicBadge(item.topic)}<span class="text-[10px] text-muted">${escape(item.topic.title)}</span></div>${bookmarkButton(item.topic)}</div><div class="relative z-0"><button class="deck-card ${state.flipped?'answer':''}" data-action="flip" aria-label="${state.flipped?'Answer shown. Activate to show question.':'Flashcard question. Activate to reveal answer.'}" aria-pressed="${state.flipped}"><span class="flex items-center justify-between w-full"><span class="eyebrow">${state.flipped?'THE ANSWER':'YOUR QUESTION'}</span><span class="text-[11px] text-muted">${i+1} <span class="opacity-50">/ ${list.length}</span></span></span><span class="${state.flipped?'card-answer':'card-question'}">${escape(state.flipped?item.answer:item.question)}</span><span class="flip-hint">${icon('rotate')} ${state.flipped?'Click to see the question':'Think it through. Click to reveal.'}</span></button></div><div class="text-center text-[10px] text-muted mt-5 mb-4">${rating==='known'?'You marked this card as known.':rating==='again'?'You marked this card for another look.':'Give yourself a moment before revealing the answer.'} &nbsp; <kbd class="key">Space</kbd> to flip</div><div class="deck-actions flex items-center justify-center gap-3 mb-5">${state.flipped?`<button class="btn" data-action="rate-card" data-rating="again">${icon('rotate')} Review again</button><button class="btn btn-primary" data-action="rate-card" data-rating="known">${icon('check')} Got it</button>`:`<button class="btn btn-primary px-7" data-action="flip">${icon('layers')} Reveal answer</button>`}</div><div class="border-t border-line pt-4 flex justify-between items-center gap-3"><button class="btn" data-action="previous" ${i<=0?'disabled':''}>${icon('left')} Previous</button><span class="text-[10px] text-muted">${list.filter(c=>p.cards[c.id]?.rating==='known').length} / ${list.length} known in this set</span><button class="btn" data-action="next" ${i>=list.length-1?'disabled':''}>Next ${icon('right')}</button></div></div><p class="text-[10px] text-muted text-center mt-3">Original recall cards. Confidence ratings are self-assessments, not proof of mastery.</p>`;
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

  function chooseTopic(id) {
    if(!course().topics.some(t=>t.id===id))return;
    state.topicId=id;state.cardId=null;state.scenarioId=null;state.flipped=false;state.expanded.add(topic().domain);state.expanded.add('CH'+(topic()?.chapter??1));state.sidebarOpen=false;
    renderAll();window.scrollTo({top:0,behavior:'instant'});
  }
  function switchMode(mode) {
    if(!availableModes().some(m=>m.id===mode))return;
    const current=topic();state.mode=mode;state.flipped=false;
    const items=deck(mode);const key=mode==='flashcards'?'cardId':'scenarioId';
    if(['flashcards','scenarios'].includes(mode)&&!items.some(i=>i.id===state[key]&&i.topic.id===current?.id))state[key]=items.find(i=>i.topic.id===current?.id)?.id||null;
    renderAll();
  }
  function navigate(delta) {
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
    if(state.mode!=='scenarios')return;const item=activeItem();if(!item)return;
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
      <div class="grid grid-cols-3 gap-3 mb-6"><div class="rounded-xl bg-brand-soft p-4"><div class="text-xl text-brand font-semibold">${s.reviewed}<span class="text-xs font-normal"> / ${t.topics}</span></div><div class="text-[10px] text-muted mt-1">Topics reviewed</div></div><div class="rounded-xl bg-peach p-4"><div class="text-xl text-peach-ink font-semibold">${s.known}</div><div class="text-[10px] text-muted mt-1">Cards known</div></div><div class="rounded-xl bg-mint p-4"><div class="text-xl text-mint-ink font-semibold">${s.attempted}</div><div class="text-[10px] text-muted mt-1">Scenarios attempted</div></div></div>
      <h3>Keep a copy of your work</h3><p>Backups include your progress, bookmarks, personal notes, and custom curricula. A study-pack export contains the current curriculum but not your private progress or notes.</p><div class="flex gap-3 flex-wrap mt-3"><button class="btn btn-primary" data-action="export-backup">${icon('download')} Export backup</button><button class="btn" data-action="export-pack">${icon('download')} Export study pack</button><button class="btn" data-action="import">${icon('upload')} Import / restore</button></div>
      <h3>Add another subject</h3><p>Select “My study material” and add topics, cards, and scenarios. Or import a study-pack JSON file; it appears in the curriculum dropdown. The included <code>docs/example-study-pack.json</code> shows the format.</p><button class="btn" data-action="add-topic">${icon('plus')} Add your own topic</button>
      <h3>Keyboard shortcuts</h3><div class="grid grid-cols-2 gap-3 text-[12px] text-muted"><span><kbd class="key">←</kbd> <kbd class="key">→</kbd> Previous / next</span><span><kbd class="key">Space</kbd> Flip a flashcard</span><span><kbd class="key">O</kbd> <kbd class="key">F</kbd> <kbd class="key">S</kbd> Change mode</span><span><kbd class="key">/</kbd> Search topics</span><span><kbd class="key">1–4</kbd> Choose scenario answer</span><span><kbd class="key">Enter</kbd> Check scenario answer</span></div>
      <h3>Reset this curriculum’s progress</h3><p>This clears review marks, card confidence ratings, and scenario attempts for the selected curriculum. Bookmarks, personal notes, and custom material are kept.</p><button class="btn btn-danger" data-action="reset-progress">${icon('rotate')} Reset learning progress</button>`);
  }
  function downloadJSON(filename,data) {
    const blob=new Blob([JSON.stringify(data,null,2)],{type:'application/json'});const url=URL.createObjectURL(blob);const a=document.createElement('a');a.href=url;a.download=filename;document.body.append(a);a.click();a.remove();setTimeout(()=>URL.revokeObjectURL(url),1500);
  }
  function exportBackup() {remember();downloadJSON(`study-studio-backup-${new Date().toISOString().slice(0,10)}.json`,{type:'study-studio-backup',version:1,savedAt:new Date().toISOString(),data:store});toast('Backup exported. Keep it somewhere safe.');}
  function exportPack() {const c={...course()};if(isBook()){delete c.reader;delete c.pageImages;}downloadJSON(`${course().id}-study-pack.json`,{schemaVersion:1,courses:[c]});toast('Study pack exported without private progress or notes.');}

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
    return {id:t.id,title,summary,domain:t.domain,concepts:cleanConcepts,cards:cleanCards,scenario,takeaway:text(t.takeaway,6000),source:safeURL(t.source),origin:'user-imported',chapter:Number.isInteger(t.chapter)?t.chapter:undefined,chapterTitle:text(t.chapterTitle,400),section:Number.isInteger(t.section)?t.section:undefined,part:Number.isInteger(t.part)?t.part:undefined,pages:cleanPages(t.pages),tasks:Array.isArray(t.tasks)?t.tasks.filter(x=>/^[A-I]\.\d{1,2}$/.test(x)).slice(0,104):[],domains:Array.isArray(t.domains)?t.domains.filter(validId).slice(0,10):[],kind:t.kind==='glossary'?'glossary':undefined,sourceNote:text(t.sourceNote,6000),sourceRef:text(t.sourceRef,1000),blocks:cleanBlocks(t.blocks),slide:Number.isInteger(t.slide)?t.slide:undefined,reading:isObject(t.reading)?t.reading:undefined,sectionId:validId(t.sectionId)?t.sectionId:undefined,outlineParent:validId(t.outlineParent)?t.outlineParent:undefined,sourceCompanions:Array.isArray(t.sourceCompanions)?t.sourceCompanions.filter(validId):[]};
  }
  function validateCourse(c) {
    if(!isObject(c)||!validId(c.id)||typeof c.title!=='string'||!c.title.trim()||!Array.isArray(c.domains)||!c.domains.length||c.domains.length>40||!Array.isArray(c.topics)||c.topics.length>10000)throw new Error('A study pack requires an ID, title, 1–40 domains, and up to 10,000 topics.');
    const ids=new Set();const domains=c.domains.map(d=>{if(!isObject(d)||!validId(d.id)||ids.has(d.id)||typeof d.title!=='string'||!d.title.trim())throw new Error('Domain IDs must be distinct and each domain needs a title.');ids.add(d.id);return {id:d.id,title:text(d.title,300)};});
    const topics=c.topics.map(t=>validateTopic(t,ids));const unique=new Set();const itemIds=new Set();
    for(const t of topics){if(unique.has(t.id))throw new Error('Topic IDs must be unique within a curriculum.');unique.add(t.id);for(const x of [...t.cards,...(t.scenario?[t.scenario]:[])]){if(itemIds.has(x.id))throw new Error('Practice-item IDs must be unique within a curriculum.');itemIds.add(x.id);}}
    return {id:c.id,title:text(c.title,200),subtitle:text(c.subtitle,300,'Imported study material'),description:text(c.description,1000,'Your imported study pack.'),provenance:text(c.provenance,6000,'User-imported material. Verify its accuracy and permissions.'),domains,topics,...cleanBookMetadata(c)};
  }
  function cleanProgress(p) {
    const clean=blankProgress();if(!isObject(p))return clean;
    clean.reviewed=Array.isArray(p.reviewed)?[...new Set(p.reviewed.filter(validId))].slice(0,10000):[];
    clean.bookmarks=Array.isArray(p.bookmarks)?[...new Set(p.bookmarks.filter(validId))].slice(0,10000):[];
    for(const [id,c] of Object.entries(isObject(p.cards)?p.cards:{}).slice(0,100000))if(validId(id)&&isObject(c)&&['again','known'].includes(c.rating))clean.cards[id]={rating:c.rating,updatedAt:text(c.updatedAt,60)};
    for(const [id,s] of Object.entries(isObject(p.scenarios)?p.scenarios:{}).slice(0,1000))if(validId(id)&&isObject(s)&&Number.isInteger(s.choice)&&s.choice>=0&&s.choice<4&&typeof s.correct==='boolean')clean.scenarios[id]={choice:s.choice,correct:s.correct,firstCorrect:typeof s.firstCorrect==='boolean'?s.firstCorrect:s.correct,attempts:Number.isInteger(s.attempts)&&s.attempts>0?Math.min(s.attempts,99999):1,updatedAt:text(s.updatedAt,60)};
    for(const [id,n] of Object.entries(isObject(p.notes)?p.notes:{}).slice(0,10000))if(validId(id)&&typeof n==='string')clean.notes[id]=text(n,20000);
    return clean;
  }
  function validateStore(value) {
    if(!isObject(value)||value.version!==1)throw new Error('This backup version is not supported.');
    const clean=blankStore();
    if(Array.isArray(value.customTopics)){if(value.customTopics.length>1000)throw new Error('Too many custom topics.');clean.customTopics=value.customTopics.map(t=>validateTopic(t,new Set(['1'])));}
    if(Array.isArray(value.courses)){if(value.courses.length>30)throw new Error('Too many curricula.');clean.courses=value.courses.map(validateCourse);}
    const ids=new Set(seed.courses.map(c=>c.id));for(const c of clean.courses){if(ids.has(c.id))throw new Error('Duplicate curriculum ID in backup.');ids.add(c.id);}
    for(const [id,p] of Object.entries(isObject(value.progress)?value.progress:{}))if(ids.has(id))clean.progress[id]=cleanProgress(p);
    const ui=isObject(value.ui)?value.ui:{};clean.ui.courseId=ids.has(ui.courseId)?ui.courseId:'design-patterns-cpp';clean.ui.mode=['outline','flashcards','scenarios','uml','code','lectures'].includes(ui.mode)?ui.mode:'outline';clean.ui.large=!!ui.large;
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
        store=clean;state.courseId=store.ui.courseId;state.mode=store.ui.mode;state.chapter='all';state.task='all';state.query='';state.domain='all';state.scope='all';state.bookmarksOnly=false;state.orders={flashcards:[],scenarios:[]};state.drafts={};state.retries.clear();restorePosition();
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
      case 'reset-progress':if(confirm(`Reset learning progress for “${course().title}”? Your notes, bookmarks, and material will be kept.`)){const p=progress();store.progress[state.courseId]={...blankProgress(),bookmarks:p.bookmarks,notes:p.notes};state.drafts={};state.retries.clear();renderAll();showSettings();toast('Learning progress reset. Notes and bookmarks were kept.');}break;
    }
  });
  document.addEventListener('input',event=>{
    if(event.target.id==='topic-search'){state.lessonView='lesson';const el=event.target,pos=el.selectionStart;state.query=el.value;state.cardId=null;state.scenarioId=null;state.flipped=false;renderAll();el.focus();try{el.setSelectionRange(pos,pos);}catch{}}
    if(event.target.id==='personal-note'){progress().notes[event.target.dataset.topic]=event.target.value;remember();}
  });
  document.addEventListener('change',event=>{
    const el=event.target;
    if(el.id==='course-select'){state.courseId=el.value;state.chapter='all';state.task='all';state.query='';state.domain='all';state.scope='all';state.bookmarksOnly=false;state.flipped=false;state.orders={flashcards:[],scenarios:[]};state.drafts={};state.retries.clear();restorePosition();renderAll();window.scrollTo({top:0,behavior:'instant'});}
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
    else if(state.mode==='scenarios'&&['1','2','3','4'].includes(key)){const item=activeItem();if(item&&(!progress().scenarios[item.id]||state.retries.has(item.id))){event.preventDefault();state.drafts[item.id]=Number(key)-1;renderWorkspace();}}
    else if(key==='enter'&&state.mode==='scenarios'&&!['BUTTON','A'].includes(tag)){event.preventDefault();checkAnswer();}
  });
  // Clicking the native dialog's backdrop closes it; clicks within its bounds do not.
  DIALOG.addEventListener('click',event=>{if(event.target===DIALOG){const r=DIALOG.getBoundingClientRect();if(event.clientX<r.left||event.clientX>r.right||event.clientY<r.top||event.clientY>r.bottom)DIALOG.close();}});
  let glossaryQuery='', codeKind='examples';
  function isBook(){return course().kind==='textbook';}
  function cleanPages(){return undefined;}
  function cleanBlocks(v){return (Array.isArray(v)?v:[]).slice(0,100).filter(isObject).map(b=>({type:['paragraph','code','list','reveal'].includes(b.type)?b.type:'paragraph',text:text(b.text,30000),title:text(b.title,1500),code:text(b.code,100000),items:Array.isArray(b.items)?b.items.slice(0,100).map(x=>text(x,15000)):[]}));}
  function cleanBookMetadata(c){
    if(c.kind!=='textbook')return {};
    const chapters=(c.chapters||[]).filter(x=>Number.isInteger(x.number)&&x.number>=0&&x.number<=100).map(x=>({number:x.number,title:text(x.title,400),part:Number(x.part)||0,partTitle:text(x.partTitle,400)}));
    const glossary=(c.glossary||[]).slice(0,1000).filter(x=>validId(x.id)).map(x=>({id:x.id,term:text(x.term,500),definition:text(x.definition,20000),chapter:Number(x.chapter)}));
    // Extra study payloads render as escaped text or inert image data, never executable HTML.
    return {kind:'textbook',chapters,glossary,teaching:isObject(c.teaching)?c.teaching:undefined,readingOrder:Array.isArray(c.readingOrder)?c.readingOrder.filter(validId):undefined,author:text(c.author,400),coverageLabel:text(c.coverageLabel,600),familyLabel:text(c.familyLabel,100),lectures:isObject(c.lectures)?c.lectures:{},examples:isObject(c.examples)?c.examples:{},diagrams:isObject(c.diagrams)?c.diagrams:{}};
  }
  function renderBlocks(blocks){return (blocks||[]).map(b=>b.type==='code'?`<section class="code-excerpt"><h3>${escape(b.title)}</h3><pre><code>${escape(b.code)}</code></pre></section>`:b.type==='list'?`<section class="text-block"><h3>${escape(b.title)}</h3><ul>${(b.items||[]).map(x=>`<li>${escape(x)}</li>`).join('')}</ul></section>`:b.type==='reveal'?`<details class="answer-reveal"><summary>${escape(b.title)}</summary><p>${escape(b.text)}</p></details>`:`<p class="reader-paragraph">${escape(b.text)}</p>`).join('');}
  function bookTopicBadge(t){return `<span class="pill bg-brand-soft text-brand">${t.chapter===-1?'GLOSSARY':t.chapter===0?'FOUNDATIONS':'CHAPTER '+t.chapter}</span><span class="text-[10px] text-muted">${escape(t.id)}</span>`;}
  function bookSourceLine(t){const lecture=course().lectures?.[String(t.chapter)];return `<section class="book-source"><div class="eyebrow">${lecture?'LECTURE SOURCE':'TEXTBOOK CONNECTION'}</div><p>${escape(t.chapterTitle)}</p><p class="text-[10px] text-muted">${escape(t.sourceRef||'Chapter glossary and explanations')}</p><div class="flex gap-2 flex-wrap mt-3">${lecture?`<button class="btn" data-action="mode" data-mode="lectures">${icon('source')} Original lecture materials</button>`:t.chapter>0?`<button class="btn" data-action="mode" data-mode="uml">${icon('grid')} UML &amp; theory</button><button class="btn" data-action="mode" data-mode="code">${icon('source')} Working C++</button>`:''}</div></section>`;}

  function goChapter(n){const t=course().topics.find(t=>t.chapter===Number(n));if(!t)return;state.chapter=String(n);state.domain='all';state.query='';state.bookmarksOnly=false;state.scope='chapter';state.lessonView='map';chooseTopic(t.id);}
  function renderBookControls(){
    const nav=$('#book-nav-controls'),bar=$('#book-toolbar');if(!isBook()){nav.innerHTML='';bar.innerHTML='';return;}
    nav.innerHTML=`<div class="book-nav-switch"><button data-action="book-nav" data-view="chapters" class="${state.bookNav==='chapters'?'active':''}">Chapters</button><button data-action="book-nav" data-view="domains" class="${state.bookNav==='domains'?'active':''}">${escape(course().familyLabel||'Pattern families')}</button></div>`;
    bar.innerHTML=`<section class="book-toolbar-panel"><div class="flex items-center justify-between gap-2 flex-wrap"><span class="pill bg-mint text-mint-ink">${escape(course().author||'Dr. Charles Dorner')}</span><button class="source-link" data-action="coverage">Course outline ${icon('chevron')}</button></div><div class="book-control-row mt-3"><label for="chapter-select" class="sr-only">Choose textbook chapter</label><select id="chapter-select" class="select-small" aria-label="Choose textbook chapter"><option value="all">All chapters</option>${course().chapters.map(c=>`<option value="${c.number}" ${String(c.number)===state.chapter?'selected':''}>${c.number===0?'Class 0':c.number}. ${escape(c.title)}</option>`).join('')}${course().glossary?.length?`<option value="-1" ${state.chapter==='-1'?'selected':''}>Reference · Glossary</option>`:''}</select>${course().glossary?.length?`<button class="btn" data-action="book-glossary">${icon('book')} Glossary</button>`:''}</div></section>`;
  }
  function renderLegacyBookSidebar(list,c,p){
    $('#filtered-count').textContent=list.length===c.topics.length?'':`${list.length} shown`;
    const gs=state.bookNav==='chapters'?[...c.chapters.map(x=>({id:'CH'+x.number,label:x.number===0?'00':String(x.number).padStart(2,'0'),title:x.title,topics:list.filter(t=>t.chapter===x.number)})),{id:'CH-1',label:'REF',title:'Glossary',topics:list.filter(t=>t.chapter===-1)}]:c.domains.map(d=>({id:d.id,label:d.id.slice(0,3),title:d.title,topics:list.filter(t=>t.domain===d.id)}));
    $('#topic-nav').innerHTML=gs.filter(g=>g.topics.length).map(g=>{const open=state.expanded.has(g.id)||!!state.query;return `<div class="mb-1"><button class="domain-row ${g.topics.some(t=>t.id===state.topicId)?'active':''}" data-action="domain-toggle" data-id="${g.id}" aria-expanded="${open}"><span class="domain-letter">${g.label}</span><span class="domain-title">${escape(g.title)}</span><span class="domain-count">${g.topics.length}</span><span class="domain-chevron ${open?'open':''}">${icon('chevron')}</span></button>${open?`<div>${g.topics.map(t=>`<button class="topic-row ${t.id===state.topicId?'active':''}" data-action="topic" data-id="${t.id}"><span class="topic-id">${escape(t.id)}</span><span class="topic-title">${escape(t.title)}</span>${p.reviewed.includes(t.id)?`<span class="icon-sm">${icon('check')}</span>`:''}</button>`).join('')}</div>`:''}</div>`}).join('');
  }
  function renderGlossaryOutline(t){return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">The book’s vocabulary</h2><p class="reading">${course().glossary.length} definitions with chapter context. Use Flashcards to practice the same terms.</p><label for="glossary-search" class="field-label">Find a term</label><input class="field" id="glossary-search" value="${escape(glossaryQuery)}" placeholder="Try ownership, product, or snapshot"><div id="glossary-results">${glossaryResults()}</div></div></article>`;}
  function glossaryResults(){const q=glossaryQuery.toLowerCase();const a=course().glossary.filter(g=>(g.term+' '+g.definition).toLowerCase().includes(q));return `<p class="text-[11px] text-muted mt-4">${a.length} terms${a.length>40?' · Showing the first 40. Narrow your search.':''}</p>`+a.slice(0,40).map(g=>`<section class="glossary-entry"><h3>${escape(g.term)}</h3><p>${escape(g.definition)}</p><button class="source-link" data-action="go-chapter" data-chapter="${g.chapter}">Chapter ${g.chapter} ${icon('right')}</button></section>`).join('');}
  function renderLectures(t){const l=course().lectures?.[String(t.chapter)];if(!l)return '<section class="panel empty"><h2>Choose a lecture chapter</h2></section>';const slides=course().topics.filter(x=>x.chapter===t.chapter&&x.slide);return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">${escape(l.title)}</h2><p class="reading">${l.slideCount} source slides with their matching professor transcript. Downloads preserve the supplied presentation, including its original diagrams, images, equations, and speaker notes.</p><div class="flex gap-3 flex-wrap mt-5 mb-5"><button class="btn btn-primary" data-action="download-lecture" data-kind="presentation">${icon('download')} Download PowerPoint</button><button class="btn" data-action="download-lecture" data-kind="transcript">${icon('download')} Download transcript</button></div><label class="field-label" for="lecture-slide-select">Open a slide lesson</label><select class="field" id="lecture-slide-select"><option value="">Choose slide</option>${slides.map(x=>`<option value="${escape(x.id)}">${x.slide}. ${escape(x.title)}</option>`).join('')}</select><details class="answer-reveal"><summary>Read the complete chapter transcript</summary><div class="lecture-transcript">${escape(l.transcript)}</div></details><p class="code-ref">Supplied files: ${escape(l.presentationName)}; ${escape(l.transcriptName)}. These source lectures remain as supplied. Open C++ for the separately authored, compiled chapter demonstrations and labs.</p>${chapterNav(t.chapter)}</div></article>`;}
  function downloadLecture(kind){const l=course().lectures[String(topic().chapter)];let blob,name;if(kind==='presentation'){const raw=atob(l.presentationBase64);blob=new Blob([Uint8Array.from(raw,c=>c.charCodeAt(0))],{type:'application/vnd.openxmlformats-officedocument.presentationml.presentation'});name=l.presentationName;}else{blob=new Blob([l.transcript],{type:'text/plain;charset=utf-8'});name=l.transcriptName;}const u=URL.createObjectURL(blob),a=document.createElement('a');a.href=u;a.download=name;document.body.append(a);a.click();a.remove();setTimeout(()=>URL.revokeObjectURL(u),1500);}
  function chapterNav(n){return `<div class="chapter-nav"><button class="btn" data-action="previous" ${n<=1?'disabled':''}>${icon('left')} Previous chapter</button><button class="btn" data-action="mode" data-mode="outline">Chapter lessons</button><button class="btn" data-action="next" ${n>=Math.max(...course().chapters.map(c=>c.number))?'disabled':''}>Next chapter ${icon('right')}</button></div>`;}
  function chapterNeeded(){if(course().id==='systems-programming')return `<section class="panel empty"><h2>Choose a Systems chapter</h2><p>Every Systems chapter has diagrams, focused explanations, a complete C++ demonstration, and an extension lab.</p><button class="btn btn-primary" data-action="go-chapter" data-chapter="1">Open Chapter 1 ${icon('right')}</button></section>`;if(!isBook())return `<section class="panel empty"><h2>No worked examples in this study pack</h2><p>Use Outline, Flashcards, and Scenarios for your personal material. Select Design Patterns in C++ for the textbook’s UML and full programs.</p><button class="btn btn-primary" data-action="mode" data-mode="outline">Return to outline</button></section>`;return `<section class="panel empty"><h2>Choose a worked example</h2><p>Every pattern chapter and the capstone have UML views, focused code, and full working source.</p><button class="btn btn-primary" data-action="go-chapter" data-chapter="1">Factory Method ${icon('right')}</button></section>`;}
  function renderUML(t){
    const d=course().diagrams?.[String(t.chapter)];if(!d)return chapterNeeded();
    const views=[d.overview,...(d.extra_overviews||[])];
    return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">${escape(t.chapterTitle)}: UML &amp; theory</h2><p class="reading">Follow each relationship to the code. Diagrams show ${escape(d.source)}. Code inside overview blocks is excerpted or shortened, with teaching comments. The focused blocks below show exact source lines.</p>${views.map((v,i)=>`<section class="diagram-section"><div class="flex gap-2 items-center justify-between flex-wrap"><h3>${escape(v.title)}</h3><button class="btn" data-action="expand-diagram" data-view="${i}">Open large ${icon('external')}</button></div><p class="diagram-kind">${escape(v.kind)} view</p><div class="diagram-scroll"><img class="uml-image" src="${safeImage(v.image)}" alt="${escape(v.title)}. ${escape(v.explanation)}"></div><p class="diagram-legend">${escape(v.explanation)}</p><details class="answer-reveal"><summary>Relationships as text</summary><ul>${v.edges.map(e=>`<li>${escape(v.nodes.find(n=>n.id===e.from)?.label||e.from)} → ${escape(v.nodes.find(n=>n.id===e.to)?.label||e.to)}: ${escape(e.kind)}; ${escape(e.label)}</li>`).join('')}</ul></details></section>`).join('')}<h3 class="focus-heading">Code blocks and adjacent theory</h3>${d.focus.map(f=>`<section class="focus-grid"><div><h4>${escape(f.title)}</h4><p class="code-ref">${escape(f.file)}:${f.lineStart}–${f.lineEnd}</p><pre><code>${escape(f.code.split('\n').map((l,i)=>`${f.lineStart+i}  ${l}`).join('\n'))}</code></pre><details class="answer-reveal"><summary>Line-by-line explanation</summary>${f.explain.map((x,i)=>`<p><strong>${f.lineStart+i}.</strong> ${escape(x)}</p>`).join('')}</details></div><aside class="theory-panel"><p class="annotation-label">Teaching annotation for this code block</p>${[['WHAT THIS MEANS',f.what],['WHERE IT FITS',f.where],['WHY IT IS HERE',f.why],['WHAT MUST STAY TRUE',f.invariant],['WHAT CAN GO WRONG',f.risk],['FIND THE CODE',`${f.file}:${f.lineStart}–${f.lineEnd}`]].map(([h,b])=>`<section><h5>${h}</h5><p>${escape(b)}</p></section>`).join('')}</aside></section>`).join('')}${chapterNav(t.chapter)}</div></article>`;
  }
  function safeImage(x){return typeof x==='string'&&x.startsWith('data:image/svg+xml;base64,')?x:'';}
  function renderCode(t){const ex=course().examples?.[String(t.chapter)];if(!ex)return chapterNeeded();const e=ex[codeKind]||ex.examples;return `<article class="panel"><div class="lesson-inner">${bookTopicBadge(t)}<h2 class="lesson-heading">${escape(t.chapterTitle)}: working C++</h2><div class="flex gap-2 flex-wrap mb-4">${[['examples','Example'],['exercises','Lab starter'],['solutions','Lab solution']].map(([k,l])=>`<button class="btn ${codeKind===k?'btn-primary':''}" data-action="code-kind" data-kind="${k}" aria-pressed="${codeKind===k}">${l}</button>`).join('')}<button class="btn" data-action="download-code">${icon('download')} Download .cpp</button></div><p class="code-ref">${escape(e.filename)} · C++17 · Standard library</p>${codeKind!=='examples'?`<p class="reading">${escape(ex.lab.task)}</p>${course().id==='systems-programming'?`<h4>Acceptance checks</h4><ul class="lab-checks">${ex.lab.checks.map(x=>`<li>${escape(x)}</li>`).join('')}</ul>`:''}`:''}<details class="answer-reveal" open><summary>Build and expected output</summary><pre><code>g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread ch${String(t.chapter).padStart(2,'0')}_${codeKind}.cpp -o example
./example</code></pre><h4>Expected output for this file</h4><pre><code>${escape(e.output)}</code></pre><p>${escape(codeKind==='examples'?ex.verification:codeKind==='exercises'?'The starter verifies its initial behavior. Complete the lab and add its acceptance checks.':ex.lab.answer)}</p></details><pre class="full-code"><code>${escape(e.code.split('\n').map((l,i)=>`${String(i+1).padStart(3)}  ${l}`).join('\n'))}</code></pre>${chapterNav(t.chapter)}</div></article>`;}
  function showBookSources(){const c=course(),n=totals();if(c.lectures&&Object.keys(c.lectures).length){modal('Lecture series and study coverage',`<p>${escape(c.provenance)}</p><h3>Included material</h3><p>${c.chapters.length} chapters, ${Object.values(c.lectures).reduce((n,l)=>n+l.slideCount,0)} slide lessons, ${n.cards} recall cards, and ${n.scenarios} original application cases. The Lectures view downloads each original PowerPoint and full transcript.</p><p>Notes and progress are kept separately for each course in this browser. Export a backup before moving the HTML or clearing browser data.</p>`);return;}modal('Textbook and study coverage',`<p>${escape(c.provenance||'Your imported study material.')}</p><h3>Included material</h3><p>Foundations, all 22 pattern chapters, and the capstone. ${n.topics} lessons, ${n.cards} recall cards, ${n.scenarios} original scenarios, 25 diagram views, and 51 focused code blocks with adjacent theory. Every named textbook section has a lesson. Exercises include hints and explained answers. The C++ view includes each example, lab starter, and reference solution.</p><h3>Using this companion</h3><p>Choose a chapter, read its outline, then switch to Flashcards or Scenarios. Use UML to follow relationships and C++ to inspect the complete implementation. Review marks and card ratings are study records, not a guarantee of mastery.</p><h3>Local storage</h3><p>Notes, bookmarks, and results stay in this browser. Export a backup before changing browsers or moving a locally opened HTML file. The app has no account, analytics, or remote API calls.</p>`);}
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

  /*__TEACHING_JS__*/
  restorePosition();renderShell();
})();
