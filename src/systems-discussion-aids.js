  // BEGIN SYSTEMS DISCUSSION AIDS
  function systemsDiscussionWorkbench(s) {
    const context = slidesDeck()?.discussionAids;
    const graph = context?.graphs?.[s.discussionAid?.graph];
    if (!graph) return '';
    const example = context.example;
    const names = new Map(graph.nodes.map(x => [x.id, x.label]));
    const focus = graph.highlighted.map(id => names.get(id)).filter(Boolean);
    return `<section class="systems-discussion-aid" aria-label="Systems discussion diagram and C++ example">
      <span class="slides-eyebrow">DISCUSSION DIAGRAM</span>
      <h3>${escape(graph.title)}</h3>
      <p class="systems-aid-location">${escape(focus.length ? 'Highlighted: ' + focus.join('; ') : 'Chapter context for: ' + s.title)}</p>
      <div class="systems-aid-figure"><img src="${safeImage(graph.image)}" alt="${escape(graph.title + '. ' + graph.caption)}"></div>
      <button class="slides-btn" data-action="systems-aid-zoom" aria-pressed="false">Enlarge diagram</button>
      <p class="slides-diagram-caption">${escape(graph.caption)}</p>
      <details class="slides-check systems-aid-relationships"><summary>Diagram relationships</summary><ul>${graph.edges.map(x => `<li>${escape(names.get(x.from))} &rarr; ${escape(names.get(x.to))}: ${escape(x.label)}</li>`).join('')}</ul></details>
      <div class="systems-aid-example"><h4>Chapter C++ example: ${escape(example.title)}</h4>
        ${slidesExcerpt(example)}
        <button class="slides-btn primary" data-action="systems-aid-example" data-slide-id="${escape(example.fullSlideId)}">Edit and run the complete example</button>
        <details class="slides-expected"><summary>Output of the complete chapter program</summary><pre><code>${escape(example.output)}</code></pre><p>${escape(example.invariant)}</p></details>
      </div>
      ${s.reading?.length ? `<details class="slides-check systems-aid-reading"><summary>The chapter discussion</summary><div class="slides-reading">${s.reading.map(p => `<p>${escape(p)}</p>`).join('')}</div></details>` : ''}
    </section>`;
  }
  document.addEventListener('click', event => {
    const zoom = event.target.closest('[data-action="systems-aid-zoom"]');
    if (zoom) {
      const source = zoom.closest('.systems-discussion-aid').querySelector('.systems-aid-figure img');
      let viewer = document.querySelector('.systems-aid-viewer');
      if (!viewer) {
        viewer = document.createElement('dialog');
        viewer.className = 'systems-aid-viewer';
        viewer.setAttribute('aria-label', 'Full-screen diagram viewer');
        const frame = document.createElement('div');
        frame.className = 'systems-aid-viewer-frame';
        const header = document.createElement('div');
        header.className = 'systems-aid-viewer-header';
        const title = document.createElement('h2');
        const close = document.createElement('button');
        close.type = 'button';
        close.className = 'slides-btn';
        close.dataset.action = 'systems-aid-viewer-close';
        close.textContent = 'Close';
        header.append(title, close);
        const image = document.createElement('img');
        image.className = 'systems-aid-viewer-image';
        frame.append(header, image);
        viewer.append(frame);
        document.body.append(viewer);
      }
      viewer.querySelector('h2').textContent = source.alt;
      const image = viewer.querySelector('.systems-aid-viewer-image');
      image.src = source.currentSrc || source.src;
      image.alt = source.alt;
      if (!viewer.open) viewer.showModal();
      return;
    }
    const close = event.target.closest('[data-action="systems-aid-viewer-close"]');
    if (close) {
      close.closest('.systems-aid-viewer')?.close();
      return;
    }
    const button = event.target.closest('[data-action="systems-aid-example"]');
    if (!button || state.mode !== 'slides') return;
    const index = slidesDeck()?.slides.findIndex(s => s.id === button.dataset.slideId);
    if (index >= 0) slidesGo(index);
  });
  document.addEventListener('click', event => {
    if (event.target.matches('.systems-aid-viewer')) event.target.close();
  });
  // END SYSTEMS DISCUSSION AIDS
