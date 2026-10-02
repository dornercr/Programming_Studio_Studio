  // BEGIN DESKTOP SIDEBAR TOGGLE
  function studioDesktopSidebarCollapsed() {
    if(typeof studioDesktopSidebarCollapsed.value!=='boolean'){
      try{studioDesktopSidebarCollapsed.value=localStorage.getItem('study-studio:desktop-sidebar-collapsed')==='1';}
      catch{studioDesktopSidebarCollapsed.value=false;}
    }
    return studioDesktopSidebarCollapsed.value;
  }
  function studioDesktopSidebarSetCollapsed(value) {
    studioDesktopSidebarCollapsed.value=Boolean(value);
    try{localStorage.setItem('study-studio:desktop-sidebar-collapsed',value?'1':'0');}catch{}
  }
  function studioDesktopSidebarSync() {
    const desktop=matchMedia('(min-width:801px)').matches;
    const collapsed=desktop&&studioDesktopSidebarCollapsed();
    const sidebar=$('#sidebar'),button=$('[data-action="open-menu"]');
    if(!sidebar||!button)return;
    if(collapsed&&sidebar.contains(document.activeElement))button.focus();
    document.body.classList.toggle('desktop-sidebar-collapsed',collapsed);
    sidebar.hidden=collapsed;
    sidebar.inert=collapsed;
    button.setAttribute('aria-controls','sidebar');
    button.setAttribute('aria-expanded',String(desktop?!collapsed:state.sidebarOpen));
    const label=desktop?(collapsed?'Show sidebar':'Hide sidebar'):'Open curriculum and topics';
    button.setAttribute('aria-label',label);
    button.title=label;
  }
  window.addEventListener('resize',()=>renderDrawer());
  // END DESKTOP SIDEBAR TOGGLE
