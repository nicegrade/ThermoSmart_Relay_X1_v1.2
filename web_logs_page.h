#ifndef THERMOSMART_WEB_LOGS_PAGE_H
#define THERMOSMART_WEB_LOGS_PAGE_H

const char LOGS_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
    <meta http-equiv="Cache-Control" content="no-cache,no-store,must-revalidate">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="0">
    <meta name="theme-color" content="#f3efe7">
    <link rel="icon" type="image/svg+xml" href="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 64 64'%3E%3Cdefs%3E%3ClinearGradient id='g' x1='0' y1='0' x2='1' y2='1'%3E%3Cstop stop-color='%237f91b0'/%3E%3Cstop offset='1' stop-color='%23465a7b'/%3E%3C/linearGradient%3E%3C/defs%3E%3Crect width='64' height='64' rx='18' fill='url(%23g)'/%3E%3Cpath d='M25 35V19a7 7 0 0 1 14 0v16a12 12 0 1 1-14 0Z' fill='none' stroke='%23fff' stroke-width='4' stroke-linecap='round' stroke-linejoin='round'/%3E%3Cpath d='M32 27v18' stroke='%23fff' stroke-width='4' stroke-linecap='round'/%3E%3Ccircle cx='32' cy='46' r='5' fill='%23fff'/%3E%3C/svg%3E">
    <title>ThermoSmart | Журнал событий</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            color-scheme: light;
            --page: #f3efe7;
            --grid: rgba(88, 77, 61, .035);
            --surface: #fbf8f1;
            --surface-soft: #eee8dd;
            --line: #e3dbcf;
            --line-strong: #d0c4b4;
            --ink: #302f2b;
            --muted: #777269;
            --accent: #596c8e;
            --accent-strong: #465a7b;
            --accent-soft: #e9e9ee;
            --danger: #b4433d;
            --danger-soft: #fff0ee;
            --warning: #8b681b;
            --warning-soft: #fbf3dd;
            --info: #477ca5;
            --info-soft: #eaf2f8;
            --shadow: 0 12px 34px rgba(34, 39, 49, .07);
        }
        *,*::before,*::after { box-sizing: border-box; }
        html { min-width: 320px; background: var(--page); }
        body {
            min-height: 100vh;
            margin: 0;
            padding: 26px;
            background-color: var(--page);
            background-image: linear-gradient(var(--grid) 1px, transparent 1px), linear-gradient(90deg, var(--grid) 1px, transparent 1px);
            background-size: 32px 32px;
            color: var(--ink);
            font-family: 'Avenir Next', 'Segoe UI', sans-serif;
            transition: background-color .2s, color .2s;
        }
        button,input,select { font: inherit; }
        button { color: inherit; }
        .app-shell { width: min(1120px, 100%); margin: 0 auto; }
        .topbar { display: flex; align-items: center; justify-content: space-between; gap: 20px; padding: 2px 0 22px; border-bottom: 1px solid var(--line); }
        .brand { display: flex; align-items: center; gap: 12px; min-width: 0; }
        .brand-mark { display: grid; place-items: center; width: 42px; height: 42px; flex: none; border: 1px solid rgba(255,255,255,.2); border-radius: 12px; background: linear-gradient(145deg, #7f91b0, #465a7b); box-shadow: 0 5px 12px rgba(70,90,123,.2), inset 0 1px 0 rgba(255,255,255,.18); text-decoration: none; transition: transform .16s, box-shadow .16s; }
        .brand-mark:hover { transform: translateY(-1px); box-shadow: 0 7px 15px rgba(70,90,123,.24), inset 0 1px 0 rgba(255,255,255,.18); }
        .brand-mark:focus-visible { outline: 2px solid var(--accent); outline-offset: 3px; }
        .brand-mark svg { display: block; width: 27px; height: 27px; }
        .brand-name { display: block; color: var(--ink); font-family: Outfit, sans-serif; font-size: 17px; font-weight: 700; }
        .brand-caption { display: block; margin-top: 2px; color: var(--muted); font-size: 11px; }
        .top-actions { display: flex; align-items: center; gap: 10px; }
        .theme-toggle { display: grid; place-items: center; width: 40px; height: 40px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface); color: var(--ink); font-size: 19px; cursor: pointer; transition: background .16s, border-color .16s, transform .16s; }
        .theme-toggle:hover { border-color: var(--accent); background: var(--surface-soft); }
        .theme-toggle:focus-visible,.action-btn:focus-visible,.footer-link:focus-visible,.filter-btn:focus-visible,.search-input:focus-visible,.auto-toggle:focus-within { outline: 2px solid var(--accent); outline-offset: 2px; }
        .footer-link,.action-btn { display: inline-flex; align-items: center; justify-content: center; min-height: 36px; padding: 7px 11px; border: 1px solid var(--line); border-radius: 7px; background: var(--surface); color: var(--muted); font-size: 11px; font-weight: 600; text-decoration: none; cursor: pointer; transition: border-color .16s, color .16s, background .16s; }
        .action-btn { font-size: 13px; }
        .footer-link:hover,.action-btn:hover { border-color: var(--accent); color: var(--accent); }
        .action-btn.primary { border-color: var(--accent); background: var(--accent); color: #fff; }
        .action-btn.primary:hover { border-color: var(--accent-strong); background: var(--accent-strong); }
        .action-btn.danger { color: var(--danger); }
        .action-btn.danger:hover { border-color: var(--danger); background: var(--danger-soft); }
        .action-btn:disabled { opacity: .6; cursor: wait; }
        .page-heading { display: flex; align-items: end; justify-content: space-between; gap: 16px; margin: 30px 0 20px; }
        .eyebrow { margin: 0 0 5px; color: var(--accent); font-size: 10px; font-weight: 700; letter-spacing: .12em; text-transform: uppercase; }
        .page-heading h1 { margin: 0; color: var(--ink); font-family: Outfit, 'Avenir Next', sans-serif; font-size: 27px; font-weight: 600; line-height: 1.18; }
        .page-caption { margin: 6px 0 0; color: var(--muted); font-size: 12px; }
        .heading-actions { display: flex; gap: 8px; flex-wrap: wrap; }
        .panel { min-width: 0; overflow: hidden; border: 1px solid var(--line); border-radius: 12px; background: var(--surface); box-shadow: var(--shadow); }
        .summary-strip { display: grid; grid-template-columns: repeat(3, minmax(0, 1fr)); margin-bottom: 14px; }
        .summary-item { min-width: 0; padding: 15px 20px; }
        .summary-item + .summary-item { border-left: 1px solid var(--line); }
        .summary-label { display: block; color: var(--muted); font-size: 10px; font-weight: 600; }
        .summary-value { display: block; margin-top: 5px; overflow: hidden; color: var(--ink); font-family: Outfit, sans-serif; font-size: 17px; font-weight: 600; text-overflow: ellipsis; white-space: nowrap; }
        .summary-value.status { display: flex; align-items: center; gap: 7px; font-family: 'Avenir Next', 'Segoe UI', sans-serif; font-size: 12px; }
        .status-dot { width: 8px; height: 8px; flex: none; border-radius: 50%; background: #9daaa3; }
        .status-dot.live { background: #38a765; box-shadow: 0 0 0 3px rgba(56,167,101,.12); }
        .status-dot.paused { background: var(--warning); }
        .toolbar { display: flex; align-items: center; justify-content: space-between; gap: 14px; padding: 14px 16px; border-bottom: 1px solid var(--line); }
        .toolbar-filters { display: flex; align-items: center; gap: 10px; min-width: 0; }
        .filter-group { display: flex; flex-wrap: wrap; gap: 4px; padding: 4px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); }
        .filter-btn { min-height: 30px; padding: 5px 10px; border: 0; border-radius: 6px; background: transparent; color: var(--muted); font-size: 10px; font-weight: 600; cursor: pointer; }
        .filter-btn:hover:not(.active) { color: var(--ink); }
        .filter-btn.active { background: var(--surface); color: var(--accent-strong); box-shadow: 0 1px 4px rgba(34,39,49,.1); }
        .search-wrap { position: relative; flex: 1; min-width: 130px; max-width: 250px; }
        .search-input { width: 100%; min-height: 38px; padding: 8px 11px 8px 32px; border: 1px solid var(--line); border-radius: 7px; background: var(--surface); color: var(--ink); font-size: 11px; }
        .search-input::placeholder { color: var(--muted); }
        .search-icon { position: absolute; top: 50%; left: 11px; color: var(--muted); font-size: 13px; transform: translateY(-50%); pointer-events: none; }
        .toolbar-side { display: flex; align-items: center; gap: 12px; }
        .auto-toggle { display: inline-flex; align-items: center; gap: 7px; color: var(--muted); font-size: 10px; white-space: nowrap; cursor: pointer; }
        .auto-toggle input { width: 15px; height: 15px; margin: 0; accent-color: var(--accent); }
        .log-list { max-height: min(64vh, 680px); min-height: 220px; overflow: auto; overscroll-behavior: contain; }
        .log-entry { display: grid; grid-template-columns: 140px 62px minmax(0, 1fr); align-items: start; gap: 10px; padding: 10px 16px; border-bottom: 1px solid var(--line); font-size: 11px; line-height: 1.55; }
        .log-entry:last-child { border-bottom: 0; }
        .log-entry:hover { background: var(--surface-soft); }
        .log-time { padding-top: 2px; color: var(--muted); font-family: ui-monospace, Consolas, monospace; font-size: 10px; font-variant-numeric: tabular-nums; }
        .log-level { display: inline-flex; justify-content: center; min-width: 54px; padding: 3px 6px; border-radius: 5px; background: var(--info-soft); color: var(--info); font-size: 9px; font-weight: 700; letter-spacing: .03em; }
        .log-level.warn { background: var(--warning-soft); color: var(--warning); }
        .log-level.error { background: var(--danger-soft); color: var(--danger); }
        .log-level.relay { background: var(--accent-soft); color: var(--accent-strong); }
        .log-message { min-width: 0; color: var(--ink); overflow-wrap: anywhere; white-space: pre-wrap; }
        .empty-state { padding: 48px 20px; color: var(--muted); text-align: center; }
        .empty-icon { display: block; margin-bottom: 10px; font-size: 30px; }
        .empty-state strong { display: block; color: var(--ink); font-family: Outfit, sans-serif; font-size: 16px; }
        .empty-state span:last-child { display: block; margin-top: 6px; font-size: 11px; }
        .panel-footer { display: flex; align-items: center; justify-content: space-between; gap: 12px; padding: 11px 16px; border-top: 1px solid var(--line); color: var(--muted); font-size: 10px; }
        .app-footer { display: flex; align-items: center; justify-content: space-between; gap: 14px; margin-top: 18px; padding: 12px 2px 2px; color: var(--muted); font-size: 10px; }
        .footer-right { display: flex; align-items: center; gap: 8px; }
        .hidden { display: none !important; }
        .toast { position: fixed; z-index: 20; bottom: 22px; left: 50%; max-width: calc(100vw - 32px); padding: 10px 16px; transform: translate(-50%, 10px); border: 1px solid var(--line); border-radius: 8px; background: var(--ink); color: #fff; font-size: 12px; opacity: 0; pointer-events: none; transition: opacity .2s,transform .2s; }
        .toast.show { transform: translate(-50%, 0); opacity: 1; }
        .toast.error { border-color: #efc6c2; background: var(--danger-soft); color: var(--danger); }
        body.theme-dark {
            color-scheme: dark;
            --page: #111318;
            --grid: rgba(215, 220, 230, .035);
            --surface: #1b1e25;
            --surface-soft: #242831;
            --line: #353943;
            --line-strong: #4a505c;
            --ink: #eceef2;
            --muted: #a3a8b3;
            --accent: #a9b8d2;
            --accent-strong: #c2cde0;
            --accent-soft: #2b3341;
            --danger: #ffaaa2;
            --danger-soft: #3b2729;
            --warning: #e3c677;
            --warning-soft: #3c3528;
            --info: #9bc7e8;
            --info-soft: #293542;
            --shadow: 0 16px 40px rgba(0,0,0,.24);
        }
        body.theme-dark .action-btn.primary { border-color: #596b88; background: #465a7b; }
        body.theme-dark .action-btn.primary:hover { background: #53698d; }
        @media (max-width: 820px) {
            body { padding: 18px; }
            .toolbar { align-items: stretch; flex-direction: column; }
            .toolbar-filters { width: 100%; }
            .search-wrap { max-width: none; }
            .toolbar-side { justify-content: space-between; }
        }
        @media (max-width: 560px) {
            body { padding: 12px; }
            .topbar { gap: 10px; padding-bottom: 16px; }
            .brand-mark { width: 38px; height: 38px; }
            .brand-caption { display: none; }
            .top-actions { gap: 7px; }
            .theme-toggle { width: 36px; height: 36px; }
            .page-heading { align-items: flex-start; flex-direction: column; margin: 22px 0 14px; }
            .page-heading h1 { font-size: 23px; }
            .heading-actions { width: 100%; }
            .heading-actions .action-btn { flex: 1; }
            .summary-item { padding: 12px 10px; }
            .summary-label { font-size: 9px; }
            .summary-value { font-size: 14px; }
            .summary-value.status { gap: 5px; font-size: 10px; }
            .toolbar { gap: 10px; padding: 12px; }
            .toolbar-filters { align-items: stretch; flex-direction: column; gap: 8px; }
            .filter-group { justify-content: space-between; }
            .filter-btn { flex: 1; padding-right: 6px; padding-left: 6px; }
            .log-entry { grid-template-columns: minmax(0, 1fr) auto; gap: 5px 7px; padding: 9px 10px; font-size: 10px; }
            .log-time { grid-column: 1; font-size: 9px; }
            .log-level { grid-column: 2; }
            .log-message { grid-column: 1 / -1; }
            .log-level { min-width: 48px; padding: 3px 4px; font-size: 8px; }
            .log-list { max-height: 62vh; min-height: 180px; }
            .panel-footer { align-items: flex-start; flex-direction: column; gap: 5px; padding: 10px 12px; }
            .app-footer { align-items: flex-start; flex-direction: column; gap: 10px; }
        }
        @media (prefers-reduced-motion: reduce) { *,*::before,*::after { transition: none !important; } }
    </style>
</head>
<body>
<div class="app-shell">
    <div id="toast" class="toast" role="status" aria-live="polite"></div>
    <header class="topbar">
        <div class="brand">
            <a href="/" class="brand-mark" title="Домой" aria-label="Домой"><svg viewBox="0 0 42 42" fill="none" xmlns="http://www.w3.org/2000/svg" aria-hidden="true"><path d="M16 23V12.5a5 5 0 0 1 10 0V23a8.5 8.5 0 1 1-10 0Z" stroke="#fff" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/><path d="M21 19v12" stroke="#fff" stroke-width="2.5" stroke-linecap="round"/><circle cx="21" cy="31" r="2.8" fill="#fff"/></svg></a>
            <span><span class="brand-name">ThermoSmart</span><span class="brand-caption">Климат-контроль дома</span></span>
        </div>
        <div class="top-actions">
            <a href="/" class="footer-link">На главную</a>
            <button class="theme-toggle" id="themeToggle" type="button" onclick="toggleTheme()" aria-label="Включить тёмную тему" title="Включить тёмную тему"><span id="themeIcon" aria-hidden="true">☾</span></button>
        </div>
    </header>
    <main>
        <div class="page-heading">
            <div><p class="eyebrow">СИСТЕМНЫЕ СОБЫТИЯ</p><h1>Журнал событий</h1><p class="page-caption">Состояние устройства, предупреждения и ошибки</p></div>
            <div class="heading-actions">
                <button class="action-btn primary" id="refreshButton" type="button" onclick="loadLogs(true)">Обновить</button>
                <button class="action-btn danger" type="button" onclick="clearLogs()">Очистить журнал</button>
            </div>
        </div>
        <section class="panel summary-strip" aria-label="Сведения о журнале">
            <div class="summary-item"><span class="summary-label">Всего записей</span><strong class="summary-value" id="entryCount">—</strong></div>
            <div class="summary-item"><span class="summary-label">Размер журнала</span><strong class="summary-value" id="fileSize">—</strong></div>
            <div class="summary-item"><span class="summary-label">Обновление</span><strong class="summary-value status" id="refreshStatus"><span class="status-dot live" id="statusDot"></span><span id="refreshStatusText">Загрузка…</span></strong></div>
        </section>
        <section class="panel" aria-label="Список событий">
            <div class="toolbar">
                <div class="toolbar-filters">
                    <div class="filter-group" role="group" aria-label="Фильтр по типу события">
                        <button class="filter-btn active" type="button" data-level="all" onclick="setLevelFilter('all')">Все</button>
                        <button class="filter-btn" type="button" data-level="INFO" onclick="setLevelFilter('INFO')">Информация</button>
                        <button class="filter-btn" type="button" data-level="WARN" onclick="setLevelFilter('WARN')">Внимание</button>
                        <button class="filter-btn" type="button" data-level="ERROR" onclick="setLevelFilter('ERROR')">Ошибки</button>
                        <button class="filter-btn" type="button" data-level="RELAY" onclick="setLevelFilter('RELAY')">Реле</button>
                    </div>
                    <label class="search-wrap"><span class="search-icon" aria-hidden="true">⌕</span><input class="search-input" id="searchInput" type="search" placeholder="Поиск по журналу" autocomplete="off" aria-label="Поиск по журналу"></label>
                </div>
                <div class="toolbar-side">
                    <label class="auto-toggle"><input type="checkbox" id="autoRefresh" checked><span>Автообновление · 5 с</span></label>
                </div>
            </div>
            <div class="log-list" id="logContainer" aria-live="polite" aria-busy="true">
                <div class="empty-state"><span class="empty-icon" aria-hidden="true">◌</span><strong>Загрузка журнала</strong><span>Получаем последние события устройства</span></div>
            </div>
            <div class="panel-footer"><span id="visibleCount">Показано: 0</span><span>Последние события устройства</span></div>
        </section>
        <footer class="app-footer">
            <span id="lastUpdated">Последняя загрузка: —</span>
            <div class="footer-right"><a href="/stats" class="footer-link">Статистика</a></div>
        </footer>
    </main>
</div>
<script>
function getToken() {
    try { return sessionStorage.getItem('thermosmart_token') || ''; } catch(e) { return ''; }
}
function getHeaders() {
    const token = getToken();
    return token ? {'Authorization': 'Bearer ' + token} : {};
}
function showToast(message, isError) {
    const toast = document.getElementById('toast');
    toast.textContent = message;
    toast.className = 'toast' + (isError ? ' error' : '');
    requestAnimationFrame(() => toast.classList.add('show'));
    clearTimeout(toast._hide);
    toast._hide = setTimeout(() => toast.classList.remove('show'), 2500);
}
function applyTheme(theme) {
    const dark = theme === 'dark';
    document.body.classList.toggle('theme-dark', dark);
    const button = document.getElementById('themeToggle');
    const label = dark ? 'Включить светлую тему' : 'Включить тёмную тему';
    button.setAttribute('aria-label', label);
    button.title = label;
    document.getElementById('themeIcon').textContent = dark ? '☀' : '☾';
}
function loadTheme() {
    let theme = 'light';
    try { theme = localStorage.getItem('thermosmart_theme') || 'light'; } catch(e) {}
    applyTheme(theme);
}
function toggleTheme() {
    const dark = !document.body.classList.contains('theme-dark');
    applyTheme(dark ? 'dark' : 'light');
    try { localStorage.setItem('thermosmart_theme', dark ? 'dark' : 'light'); } catch(e) {}
}
let logLines = [];
let activeLevel = 'all';
let refreshTimer = null;
let loading = false;
let logsLoaded = false;

function classifyLine(line) {
    const match = line.match(/^\[([^\]]+)\]\s*\[([^\]]+)\]\s*(.*)$/);
    if (!match) return {time: '', level: 'INFO', message: line};
    const level = ['INFO', 'WARN', 'ERROR', 'RELAY'].indexOf(match[2]) !== -1 ? match[2] : 'INFO';
    return {time: match[1], level: level, message: match[3]};
}
function renderLogs() {
    const container = document.getElementById('logContainer');
    const query = document.getElementById('searchInput').value.trim().toLocaleLowerCase('ru');
    const visible = logLines.filter(entry => {
        const levelMatches = activeLevel === 'all' || entry.level === activeLevel;
        const searchMatches = !query || (entry.time + ' ' + entry.level + ' ' + entry.message).toLocaleLowerCase('ru').includes(query);
        return levelMatches && searchMatches;
    });
    container.replaceChildren();
    container.setAttribute('aria-busy', 'false');
    if (!visible.length) {
        const empty = document.createElement('div');
        empty.className = 'empty-state';
        const icon = document.createElement('span');
        icon.className = 'empty-icon';
        icon.setAttribute('aria-hidden', 'true');
        icon.textContent = logLines.length ? '⌕' : '📭';
        const title = document.createElement('strong');
        title.textContent = logLines.length ? 'Ничего не найдено' : 'Журнал пока пуст';
        const caption = document.createElement('span');
        caption.textContent = logLines.length ? 'Измените запрос или выберите другой тип события' : 'Новые события появятся здесь автоматически';
        empty.append(icon, title, caption);
        container.appendChild(empty);
    } else {
        const fragment = document.createDocumentFragment();
        visible.forEach(entry => {
            const row = document.createElement('article');
            row.className = 'log-entry';
            const time = document.createElement('time');
            time.className = 'log-time';
            time.textContent = entry.time || '—';
            const level = document.createElement('span');
            level.className = 'log-level ' + entry.level.toLowerCase();
            level.textContent = entry.level === 'WARN' ? 'ВНИМАНИЕ' :
                entry.level === 'ERROR' ? 'ОШИБКА' :
                entry.level === 'RELAY' ? 'РЕЛЕ' : 'ИНФО';
            const message = document.createElement('span');
            message.className = 'log-message';
            message.textContent = entry.message;
            row.append(time, level, message);
            fragment.appendChild(row);
        });
        container.appendChild(fragment);
    }
    document.getElementById('visibleCount').textContent =
        'Показано: ' + visible.length + (visible.length !== logLines.length ? ' из ' + logLines.length : '');
}
function setLevelFilter(level) {
    activeLevel = level;
    document.querySelectorAll('.filter-btn').forEach(button => {
        button.classList.toggle('active', button.dataset.level === level);
    });
    renderLogs();
}
function setRefreshStatus(text, isLive) {
    document.getElementById('refreshStatusText').textContent = text;
    document.getElementById('statusDot').className = 'status-dot ' + (isLive ? 'live' : 'paused');
}
function formatFileSize(bytes) {
    if (bytes < 1024) return bytes + ' Б';
    return (bytes / 1024).toFixed(1) + ' КБ';
}
async function loadLogs(showErrorToast) {
    if (loading) return;
    loading = true;
    document.getElementById('refreshButton').disabled = true;
    try {
        const headers = getHeaders();
        const responses = await Promise.all([
            fetch('/api/logs?t=' + Date.now(), {headers: headers, cache: 'no-cache'}),
            fetch('/api/logs/info?t=' + Date.now(), {headers: headers, cache: 'no-cache'})
        ]);
        if (responses.some(response => response.status === 401)) {
            window.location.href = '/login';
            return;
        }
        if (!responses[0].ok) throw new Error('Не удалось загрузить журнал (HTTP ' + responses[0].status + ')');
        const text = await responses[0].text();
        logLines = text.split(/\r?\n/).filter(line => line.trim() !== '')
            .filter(line => !['No logs available', 'Cannot open log file', 'Log file is empty', 'No logs found'].includes(line.trim()))
            .map(classifyLine).reverse();
        renderLogs();
        logsLoaded = true;
        if (!responses[1].ok) throw new Error('Не удалось загрузить сведения о журнале (HTTP ' + responses[1].status + ')');
        const info = await responses[1].json();
        document.getElementById('entryCount').textContent = String(info.entries || 0);
        document.getElementById('fileSize').textContent = formatFileSize(Number(info.size) || 0);
        const updated = new Date();
        document.getElementById('lastUpdated').textContent =
            'Последняя загрузка: ' + updated.toLocaleTimeString('ru-RU', {hour: '2-digit', minute: '2-digit', second: '2-digit'});
        setRefreshStatus(document.getElementById('autoRefresh').checked ? 'Авто · 5 с' : 'Обновлено', document.getElementById('autoRefresh').checked);
    } catch(error) {
        console.error('Logs error:', error);
        setRefreshStatus('Ошибка загрузки', false);
        if (showErrorToast) showToast(error.message || 'Не удалось загрузить журнал', true);
        if (!logsLoaded) {
            const container = document.getElementById('logContainer');
            container.setAttribute('aria-busy', 'false');
            container.replaceChildren();
            const empty = document.createElement('div');
            empty.className = 'empty-state';
            const title = document.createElement('strong');
            title.textContent = 'Не удалось загрузить журнал';
            const caption = document.createElement('span');
            caption.textContent = error.message || 'Проверьте соединение и попробуйте ещё раз';
            empty.append(title, caption);
            container.appendChild(empty);
        }
    } finally {
        loading = false;
        document.getElementById('refreshButton').disabled = false;
    }
}
async function clearLogs() {
    if (!confirm('Удалить все записи журнала?')) return;
    try {
        const response = await fetch('/api/logs/clear', {method: 'POST', headers: getHeaders()});
        if (response.status === 401) { window.location.href = '/login'; return; }
        if (!response.ok) throw new Error('HTTP ' + response.status);
        await loadLogs(false);
        showToast('Журнал событий очищен');
    } catch(error) {
        console.error('Clear logs error:', error);
        showToast('Не удалось очистить журнал', true);
    }
}
function configureAutoRefresh() {
    if (refreshTimer) {
        clearInterval(refreshTimer);
        refreshTimer = null;
    }
    const enabled = document.getElementById('autoRefresh').checked;
    setRefreshStatus(enabled ? 'Авто · 5 с' : 'Пауза', enabled);
    if (enabled) refreshTimer = setInterval(() => loadLogs(false), 5000);
}
document.addEventListener('DOMContentLoaded', function() {
    loadTheme();
    document.getElementById('searchInput').addEventListener('input', renderLogs);
    document.getElementById('autoRefresh').addEventListener('change', configureAutoRefresh);
    configureAutoRefresh();
    loadLogs(false);
});
window.addEventListener('beforeunload', function() {
    if (refreshTimer) clearInterval(refreshTimer);
});
</script>
</body>
</html>
)rawliteral";

#endif
