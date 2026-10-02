  // BEGIN BOOK II DISCUSSION AIDS
  function bookTwoCourseLabel(deck,fallback) {
    return deck?.courseTitle||(deck?.courseId==='cpp-book-02'?'Book II · Modern C++ Programming':fallback);
  }
  function bookTwoProjectWorkbench(s) {
    const e=slidesDeck()?.bookTwoDiscussionAids?.examples?.[s.bookTwoProjectExample];
    return `<section class="b2-discussion-aid"><h3>Original multi-file source</h3>${slidesExcerpt({text:s.code.text,filename:s.code.filename})}<p>${escape(e.adaptationNote)}</p><button type="button" class="slides-btn primary" data-action="b2-aid-example" data-slide-id="${escape(e.fullSlideId)}">Edit and run the verified single-file adaptation</button><button type="button" class="slides-btn" data-action="b2-aid-download">Download original source</button></section>`;
  }
  function bookTwoDiagramWorkbench(s) {
    const graph=slidesDeck()?.bookTwoDiscussionAids?.graphs?.[s.bookTwoDiagram];
    if(!graph)return '';
    return `<section class="b2-discussion-aid" aria-label="Book II original chapter diagram">
      <span class="slides-eyebrow">CHAPTER DIAGRAM</span><h3>${escape(graph.title)}</h3>
      <div class="b2-aid-figure"><img src="${safeImage(graph.image)}" data-diagram-title="${escape(graph.title)}" alt="${escape(graph.title+'. '+graph.caption)}"></div>
      <button type="button" class="slides-btn" data-action="b2-aid-zoom">Enlarge diagram</button>
      <p class="slides-diagram-caption">${escape(graph.caption)}</p></section>`;
  }
  function bookTwoDiscussionWorkbench(s) {
    const context=slidesDeck()?.bookTwoDiscussionAids;
    const example=context?.examples?.[s.bookTwoDiscussionAid?.example];
    const graph=context?.graphs?.[s.bookTwoDiscussionAid?.graph];
    if(!graph)return '';
    return `<section class="b2-discussion-aid" aria-label="Book II discussion diagram and C++ example">
      <span class="slides-eyebrow">DISCUSSION DIAGRAM</span><h3>${escape(graph.title)}</h3>
      <div class="b2-aid-figure"><img src="${safeImage(graph.image)}" data-diagram-title="${escape(graph.title)}" alt="${escape(graph.title+'. '+graph.caption)}"></div>
      <button type="button" class="slides-btn" data-action="b2-aid-zoom">Enlarge diagram</button>
      <p class="slides-diagram-caption">${escape(graph.caption)}</p>
      ${example?`<div class="b2-aid-example"><h4>${escape(example.title)}</h4>${slidesExcerpt(example)}
      ${example.adaptationNote?`<p>${escape(example.adaptationNote)}</p>`:''}
      ${example.fullSlideId?`<button type="button" class="slides-btn primary" data-action="b2-aid-example" data-slide-id="${escape(example.fullSlideId)}">${example.adaptationNote?'Edit and run the single-file adaptation':'Edit and run the complete example'}</button>`:`<button type="button" class="slides-btn" data-action="b2-aid-download">Download original source</button>`}
      ${example.output!==null?`<details class="slides-expected"><summary>${example.recordedObservation?'Recorded reference output (toolchain-dependent)':'Expected output with the supplied sample input'}</summary>${example.sampleInput?`<strong>Sample input</strong><pre><code>${escape(example.sampleInput)}</code></pre>`:''}<strong>Output</strong><pre><code>${escape(example.output)}</code></pre><p>${escape(example.invariant)}</p></details>`:''}</div>`:''}
      ${s.reading?.length?`<details class="slides-check b2-aid-reading" open><summary>The chapter discussion</summary><div class="slides-reading">${s.reading.map(p=>`<p>${escape(p)}</p>`).join('')}</div></details>`:''}
    </section>`;
  }
  function bookTwoRecordedOutput(s) {
    return `<details class="slides-expected"><summary>Recorded reference output (toolchain-dependent)</summary><p>This is an observation from the supplied source verification, not live output. Compiler versions, numeric limits, and addresses can vary.</p><pre>${escape('stdout:\n'+(s.code.expectedStdout||'(empty)')+'\nstderr:\n'+(s.code.expectedStderr||'(empty)'))}</pre></details>`;
  }
  function bookTwoOpenDiagram(source,opener) {
    if(!source)return;
    let viewer=document.querySelector('.b2-aid-viewer');
    if(!viewer){
      viewer=document.createElement('dialog');viewer.className='b2-aid-viewer';
      viewer.setAttribute('aria-label','Full-screen Book II diagram');
      const frame=document.createElement('div');frame.className='b2-aid-viewer-frame';
      const header=document.createElement('div');header.className='b2-aid-viewer-header';
      const title=document.createElement('h2'),close=document.createElement('button');
      close.type='button';close.className='slides-btn';close.dataset.action='b2-aid-close';close.textContent='Close';
      const image=document.createElement('img');image.className='b2-aid-viewer-image';
      header.append(title,close);frame.append(header,image);viewer.append(frame);
      viewer.addEventListener('close',()=>viewer._opener?.isConnected&&viewer._opener.focus());
    }
    (document.fullscreenElement||document.body).append(viewer);
    viewer._opener=opener;
    viewer.querySelector('h2').textContent=source.dataset.diagramTitle||slidesCurrent()?.title||'Book II diagram';
    const image=viewer.querySelector('img');image.src=source.currentSrc||source.src;image.alt=source.alt;
    if(!viewer.open)viewer.showModal();
  }
  document.addEventListener('click',event=>{
    if(!(event.target instanceof Element))return;
    const zoom=event.target.closest('[data-action="b2-aid-zoom"]');
    if(zoom){bookTwoOpenDiagram(zoom.closest('.b2-discussion-aid')?.querySelector('img'),zoom);return;}
    const close=event.target.closest('[data-action="b2-aid-close"]');
    if(close){close.closest('.b2-aid-viewer')?.close();return;}
    if(event.target.matches('.b2-aid-viewer')){event.target.close();return;}
    const button=event.target.closest('[data-action="b2-aid-example"]');
    if(button&&state.mode==='slides'){
      const index=slidesDeck()?.slides.findIndex(s=>s.id===button.dataset.slideId);
      if(index>=0)slidesGo(index);return;
    }
    if(event.target.closest('[data-action="b2-aid-download"]')&&state.mode==='slides'){
      const s=slidesCurrent(),example=slidesDeck()?.bookTwoDiscussionAids?.examples?.[s.bookTwoProjectExample||s.bookTwoDiscussionAid?.example];
      if(example?.sourceText)slidesDownload(example.filename.split('/').pop(),example.sourceText);
    }
  });
  // END BOOK II DISCUSSION AIDS
