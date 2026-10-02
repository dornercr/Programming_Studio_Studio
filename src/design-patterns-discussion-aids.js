  // BEGIN DESIGN PATTERNS DISCUSSION AIDS
  function designPatternsDiscussionWorkbench(s) {
    const context = slidesDeck()?.designPatternsDiscussionAids;
    const example = context?.examples?.[s.designPatternsDiscussionAid?.example];
    if (!context?.graph || !example) return '';
    const graph = context.graph;
    return `<section class="dp-discussion-aid" aria-label="Design Patterns discussion diagram and C++ example">
      <span class="slides-eyebrow">DISCUSSION DIAGRAM</span>
      <h3>${escape(graph.title)}</h3>
      <div class="dp-aid-figure"><img src="${safeImage(graph.image)}" alt="${escape(graph.title + '. ' + graph.caption)}"></div>
      <button class="slides-btn" type="button" data-action="dp-aid-zoom">Enlarge diagram</button>
      <p class="slides-diagram-caption">${escape(graph.caption)}</p>
      <div class="dp-aid-example"><h4>${escape(example.title)}</h4>
        ${slidesExcerpt(example)}
        <button class="slides-btn primary" type="button" data-action="dp-aid-example" data-slide-id="${escape(example.fullSlideId)}">Edit and run the complete example</button>
        <details class="slides-expected"><summary>Expected output of the complete original program</summary><pre><code>${escape(example.output)}</code></pre><p>${escape(example.invariant)}</p></details>
      </div>
      ${s.reading?.length ? `<details class="slides-check dp-aid-reading" open><summary>The chapter discussion</summary><div class="slides-reading">${s.reading.map(p => `<p>${escape(p)}</p>`).join('')}</div></details>` : ''}
    </section>`;
  }
  document.addEventListener('click', event => {
    if (!(event.target instanceof Element)) return;
    const zoom = event.target.closest('[data-action="dp-aid-zoom"]');
    if (zoom) {
      const source = zoom.closest('.dp-discussion-aid')?.querySelector('.dp-aid-figure img');
      if (!source) return;
      let viewer = document.querySelector('.dp-aid-viewer');
      if (!viewer) {
        viewer = document.createElement('dialog');
        viewer.className = 'dp-aid-viewer';
        viewer.setAttribute('aria-label', 'Full-screen Design Patterns diagram');
        const frame = document.createElement('div');
        frame.className = 'dp-aid-viewer-frame';
        const header = document.createElement('div');
        header.className = 'dp-aid-viewer-header';
        const title = document.createElement('h2');
        const close = document.createElement('button');
        close.type = 'button';
        close.className = 'slides-btn';
        close.dataset.action = 'dp-aid-close';
        close.textContent = 'Close';
        header.append(title, close);
        const image = document.createElement('img');
        image.className = 'dp-aid-viewer-image';
        frame.append(header, image);
        viewer.append(frame);
        viewer.addEventListener('close', () => viewer._opener?.isConnected && viewer._opener.focus());
        document.body.append(viewer);
      }
      viewer._opener = zoom;
      (document.fullscreenElement || document.body).append(viewer);
      viewer.querySelector('h2').textContent = slidesDeck()?.designPatternsDiscussionAids?.graph.title || source.alt;
      const image = viewer.querySelector('img');
      image.src = source.currentSrc || source.src;
      image.alt = source.alt;
      if (!viewer.open) viewer.showModal();
      return;
    }
    const close = event.target.closest('[data-action="dp-aid-close"]');
    if (close) { close.closest('.dp-aid-viewer')?.close(); return; }
    if (event.target.matches('.dp-aid-viewer')) { event.target.close(); return; }
    const button = event.target.closest('[data-action="dp-aid-example"]');
    if (!button || state.mode !== 'slides') return;
    const index = slidesDeck()?.slides.findIndex(s => s.id === button.dataset.slideId);
    if (index >= 0) slidesGo(index);
  });
  // END DESIGN PATTERNS DISCUSSION AIDS
