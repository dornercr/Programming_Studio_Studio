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
    return `<section class="workshop-panel" id="chapter-workshop"><div class="teaching-section-head"><span class="teaching-kicker">WORKED EXAMPLE · ${escape(w.kind)}</span><h3>${escape(w.title)}</h3><div class="teaching-prose">${prose(w.problem)}</div></div>${w.design?`<section class="design-reason"><h4>Why this design?</h4><div class="teaching-prose">${prose(w.design)}</div></section>`:''}<div class="workshop-code-grid"><div class="min-w-0"><h4>Read the code</h4><p class="teaching-caption">${escape(w.excerptLabel||w.filename)} · Excerpt; complete program below.</p><pre class="teaching-code"><code>${escape(teachingCode(w))}</code></pre><h4>Expected output of the complete program</h4><pre class="expected-output"><code>${escape(w.output)}</code></pre></div><aside class="worked-explanation"><span class="teaching-kicker">HOW TO READ THIS EXAMPLE</span><h4>Predict, then trace.</h4><p>Before looking at the output, follow each change in the table. Keep the assumptions with the result.</p>${w.invariants?`<h4>Rules that must stay true</h4><ul>${w.invariants.map(x=>`<li>${escape(x)}</li>`).join('')}</ul>`:''}<div class="pitfall-panel"><h4>A common wrong turn</h4>${prose(w.pitfall)}</div></aside></div>${traceTable(w)}<section class="alternative-panel"><h4>Compare another design</h4><div class="teaching-prose">${prose(w.alternative)}</div></section>${!compact?discussionPanel(w.discussion):''}${w.maintenance?`<section class="maintenance-panel"><h4>Maintaining a real program</h4><div class="teaching-prose">${prose(w.maintenance)}</div></section>`:''}<details class="answer-reveal full-program"><summary>Complete program, build command, and verification</summary><p class="teaching-caption">${escape(w.filename)} · C++17 · Standard library</p><div class="flex gap-2 flex-wrap"><button class="btn" data-action="download-workshop">${icon('download')} Download complete .cpp</button></div><pre><code>g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread ${escape(w.filename)} -o workshop
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
    if(t.scenario){result+=`<section class="lesson-case"><h3>Compare the possible decisions</h3><ol type="A">${t.scenario.options.map(s=>`<li>${escape(s)}</li>`).join('')}</ol>${discussionPanel({question:'Which choice preserves the requirement, and what breaks in the others?',answer:t.scenario.rationales?.map((r,i)=>String.fromCharCode(65+i)+'. '+r).join('\n\n')||t.scenario.explanation},'Explain your choice')}<button class="btn" data-action="mode" data-mode="scenarios">Answer in Scenarios ${icon('right')}</button></section>`;}
    return result;
  }
  function renderPatternLesson(t,g){
    const discussion=t.reading?.discussion;
    return `<article class="panel teaching-reader pattern-focused-lesson">${guideTrail(t,g)}<div class="teaching-lesson-head"><div class="flex items-center justify-between gap-3">${topicBadge(t)}${bookmarkButton(t)}</div><h2 class="lesson-heading">${escape(t.reading?.title||t.title)}</h2></div><section class="lesson-explanation" id="lesson-idea">${lessonTheory(t)}</section>${discussionPanel(discussion,'Discuss this lesson')}<section class="context-link"><h3>Put this lesson into practice</h3><p>The complete chapter example, trace, output and design comparison are on the chapter map. The Code view keeps the original example, extension starting point and solution separate.</p><div class="flex gap-2 flex-wrap"><button class="btn" data-action="chapter-map">Open ${escape(g.title)} worked example</button><button class="btn" data-action="mode" data-mode="code">Read the complete C++</button><button class="btn" data-action="mode" data-mode="uml">Follow the diagrams</button><button class="btn" data-action="lab-open-question" data-id="dpx-${String(t.chapter).padStart(2,'0')}-a">Repair and test this chapter</button></div></section>${sourceMaterial(t)}${bookSourceLine(t)}${teachingNotes(t)}${readingFooter(t)}</article>`;
  }
  function renderGuidedLesson(t,g){
    if(course().id==='design-patterns-cpp'&&t.chapter>=1&&t.chapter<=23)return renderPatternLesson(t,g);
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
