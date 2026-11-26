(() => {
  const menuToggle = document.getElementById('menu-toggle');
  const nav = document.getElementById('site-nav');

  if (menuToggle && nav) {
    const header = menuToggle.closest('.site-header');
    const mobile = window.matchMedia('(max-width: 800px)');
    const isOpen = () => menuToggle.getAttribute('aria-expanded') === 'true';
    const setMenuOpen = (open) => {
      menuToggle.setAttribute('aria-expanded', String(open));
      header?.toggleAttribute('data-menu-open', open);
    };

    menuToggle.addEventListener('click', () => setMenuOpen(!isOpen()));

    nav.addEventListener('click', (event) => {
      const link = event.target.closest?.('a[href]');
      if (link && nav.contains(link) && isOpen()) setMenuOpen(false);
    });

    document.addEventListener('keydown', (event) => {
      if (event.key !== 'Escape' || !isOpen()) return;
      setMenuOpen(false);
      menuToggle.focus();
    });

    mobile.addEventListener('change', (event) => {
      if (!event.matches) setMenuOpen(false);
    });
  }

  document.querySelectorAll('[data-demo-tablist]').forEach((tablist) => {
    const tabs = Array.from(tablist.querySelectorAll('[data-demo-tab]'), (tab) => ({
      tab,
      panel: document.getElementById(tab.getAttribute('aria-controls')),
    })).filter(({ panel }) => panel);

    const selectTab = (selectedIndex) => {
      tabs.forEach(({ tab, panel }, index) => {
        const selected = index === selectedIndex;
        tab.setAttribute('aria-selected', String(selected));
        tab.tabIndex = selected ? 0 : -1;
        panel.hidden = !selected;
      });
    };

    tabs.forEach(({ tab }, index) => {
      tab.addEventListener('click', () => selectTab(index));

      tab.addEventListener('keydown', (event) => {
        let nextIndex;

        switch (event.key) {
          case 'ArrowRight':
          case 'ArrowDown':
            nextIndex = (index + 1) % tabs.length;
            break;
          case 'ArrowLeft':
          case 'ArrowUp':
            nextIndex = (index + tabs.length - 1) % tabs.length;
            break;
          case 'Home':
            nextIndex = 0;
            break;
          case 'End':
            nextIndex = tabs.length - 1;
            break;
          default:
            return;
        }

        event.preventDefault();
        selectTab(nextIndex);
        tabs[nextIndex].tab.focus();
      });
    });
  });
})();
