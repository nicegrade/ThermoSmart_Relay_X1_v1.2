#ifndef THERMOSMART_WEB_STATS_PAGE_H
#define THERMOSMART_WEB_STATS_PAGE_H

const char STATS_PAGE[] PROGMEM = R"rawliteral(
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
    <title>ThermoSmart | История показателей</title>
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
        button { font: inherit; color: inherit; }
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
        .theme-toggle:focus-visible,.action-btn:focus-visible,.period-btn:focus-visible,.footer-link:focus-visible { outline: 2px solid var(--accent); outline-offset: 2px; }
        .footer-link,.action-btn { display: inline-flex; align-items: center; justify-content: center; min-height: 36px; padding: 7px 11px; border: 1px solid var(--line); border-radius: 7px; background: var(--surface); color: var(--muted); font-size: 11px; font-weight: 600; text-decoration: none; cursor: pointer; transition: border-color .16s, color .16s, background .16s; }
        .action-btn { font-size: 13px; }
        .footer-link:hover,.action-btn:hover { border-color: var(--accent); color: var(--accent); }
        .action-btn.primary { border-color: var(--accent); background: var(--accent); color: #fff; }
        .action-btn.primary:hover { border-color: var(--accent-strong); background: var(--accent-strong); }
        .action-btn.danger { color: var(--danger); }
        .action-btn.danger:hover { border-color: var(--danger); background: var(--danger-soft); }
        .page-heading { display: flex; align-items: end; justify-content: space-between; gap: 16px; margin: 30px 0 20px; }
        .eyebrow { margin: 0 0 5px; color: var(--accent); font-size: 10px; font-weight: 700; letter-spacing: .12em; text-transform: uppercase; }
        .page-heading h1 { margin: 0; color: var(--ink); font-family: Outfit, 'Avenir Next', sans-serif; font-size: 27px; font-weight: 600; line-height: 1.18; }
        .page-caption { margin: 6px 0 0; color: var(--muted); font-size: 12px; }
        .heading-actions { display: flex; gap: 8px; flex-wrap: wrap; }
        .panel { min-width: 0; overflow: hidden; border: 1px solid var(--line); border-radius: 12px; background: var(--surface); box-shadow: var(--shadow); }
        .period-panel { display: flex; align-items: center; justify-content: space-between; gap: 16px; margin-bottom: 14px; padding: 15px 20px; }
        .period-label { color: var(--ink); font-size: 12px; font-weight: 600; }
        .period-group { display: flex; gap: 4px; padding: 4px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); }
        .period-btn { min-width: 66px; min-height: 34px; padding: 7px 12px; border: 0; border-radius: 6px; background: transparent; color: var(--muted); font-size: 11px; font-weight: 600; cursor: pointer; transition: background .16s, color .16s; }
        .period-btn:hover:not(.active) { color: var(--ink); }
        .period-btn.active { background: var(--surface); color: var(--accent-strong); box-shadow: 0 1px 4px rgba(34, 39, 49, .1); }
        .charts { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 14px; }
        .chart-panel { border-top: 3px solid var(--accent); }
        .chart-panel.humidity-panel { border-top-color: #477ca5; }
        .panel-head { display: flex; align-items: flex-start; justify-content: space-between; gap: 12px; padding: 18px 20px 14px; }
        .panel-title { margin: 0; color: var(--ink); font-family: Outfit, sans-serif; font-size: 16px; font-weight: 600; }
        .panel-caption { margin: 5px 0 0; color: var(--muted); font-size: 11px; }
        .chart-summary { color: var(--muted); font-size: 10px; line-height: 1.6; text-align: right; white-space: nowrap; }
        .chart-summary strong { color: var(--ink); font-size: 12px; font-weight: 600; }
        .chart-body { height: 290px; padding: 0 14px 14px; }
        .chart-body canvas { width: 100% !important; height: 100% !important; }
        .empty-state { padding: 48px 20px; border-top: 3px solid var(--accent); text-align: center; }
        .empty-icon { display: block; margin-bottom: 10px; font-size: 32px; }
        .empty-state h2 { margin: 0; color: var(--ink); font-family: Outfit, sans-serif; font-size: 17px; }
        .empty-state p { margin: 7px 0 0; color: var(--muted); font-size: 12px; }
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
            --shadow: 0 16px 40px rgba(0,0,0,.24);
        }
        body.theme-dark .action-btn.primary { border-color: #596b88; background: #465a7b; }
        body.theme-dark .action-btn.primary:hover { background: #53698d; }
        body.theme-dark .chart-panel.humidity-panel { border-top-color: #89b7df; }
        @media (max-width: 820px) {
            body { padding: 18px; }
            .charts { grid-template-columns: minmax(0, 1fr); }
            .chart-body { height: 280px; }
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
            .period-panel { align-items: flex-start; flex-direction: column; gap: 9px; padding: 14px; }
            .period-group { width: 100%; }
            .period-btn { flex: 1; min-width: 0; padding: 7px 6px; }
            .panel-head { padding: 16px 15px 12px; }
            .chart-body { height: 250px; padding: 0 8px 12px; }
            .chart-summary { font-size: 9px; }
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
            <div><p class="eyebrow">ИСТОРИЯ ПОКАЗАТЕЛЕЙ</p><h1>Температура и влажность</h1><p class="page-caption">Изменения показаний датчика за выбранный период</p></div>
            <div class="heading-actions">
                <button class="action-btn primary" type="button" onclick="loadStats(currentDays)">Обновить</button>
                <button class="action-btn danger" type="button" onclick="clearStats()">Очистить историю</button>
            </div>
        </div>
        <section class="panel period-panel" aria-label="Период отображения">
            <span class="period-label">Период</span>
            <div class="period-group" role="group" aria-label="Выберите период">
                <button class="period-btn" type="button" data-days="1" onclick="setPeriod(1)">1 день</button>
                <button class="period-btn active" type="button" data-days="7" onclick="setPeriod(7)">7 дней</button>
                <button class="period-btn" type="button" data-days="30" onclick="setPeriod(30)">30 дней</button>
            </div>
        </section>
        <section id="charts" class="charts hidden" aria-label="Графики показателей">
            <article class="panel chart-panel" aria-labelledby="temperatureTitle">
                <div class="panel-head">
                    <div><h2 class="panel-title" id="temperatureTitle">Температура</h2><p class="panel-caption">Градусы Цельсия (°C)</p></div>
                    <div class="chart-summary" id="temperatureSummary"></div>
                </div>
                <div class="chart-body"><canvas id="temperatureChart" role="img" aria-label="График температуры за выбранный период"></canvas></div>
            </article>
            <article class="panel chart-panel humidity-panel" aria-labelledby="humidityTitle">
                <div class="panel-head">
                    <div><h2 class="panel-title" id="humidityTitle">Влажность</h2><p class="panel-caption">Относительная влажность (%)</p></div>
                    <div class="chart-summary" id="humiditySummary"></div>
                </div>
                <div class="chart-body"><canvas id="humidityChart" role="img" aria-label="График влажности за выбранный период"></canvas></div>
            </article>
        </section>
        <section id="emptyState" class="panel empty-state hidden" role="status">
            <span class="empty-icon" aria-hidden="true">📊</span>
            <h2>Пока нет данных за этот период</h2>
            <p>Показания сохраняются автоматически один раз в час.</p>
        </section>
        <footer class="app-footer">
            <span id="dataCount">Записей: 0</span>
            <div class="footer-right"><span id="updateTime">Обновление: --</span><a href="/logs" class="footer-link">Журнал событий</a></div>
        </footer>
    </main>
</div>
<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
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
async function checkAuth() {
    const token = getToken();
    if (!token) { window.location.href = '/login'; return false; }
    try {
        const response = await fetch('/api/state?t=' + Date.now(), {headers: getHeaders(), cache: 'no-cache'});
        if (response.status === 401 || !response.ok) {
            sessionStorage.removeItem('thermosmart_token');
            window.location.href = '/login';
            return false;
        }
        return true;
    } catch(e) {
        window.location.href = '/login';
        return false;
    }
}
let temperatureChart = null;
let humidityChart = null;
let currentDays = 7;

function setPeriod(days) {
    if ([1, 7, 30].indexOf(days) === -1) return;
    currentDays = days;
    document.querySelectorAll('.period-btn').forEach(button => {
        button.classList.toggle('active', Number(button.dataset.days) === days);
    });
    loadStats(days);
}
function formatFullDate(timestamp) {
    return new Date(timestamp * 1000).toLocaleString('ru-RU', {
        day: '2-digit', month: '2-digit', year: 'numeric',
        hour: '2-digit', minute: '2-digit'
    });
}
function formatAxisDate(timestamp, days) {
    const options = days === 1
        ? {hour: '2-digit', minute: '2-digit'}
        : {day: '2-digit', month: '2-digit'};
    return new Date(timestamp * 1000).toLocaleString('ru-RU', options);
}
function summaryText(values, unit) {
    const min = Math.min.apply(null, values);
    const max = Math.max.apply(null, values);
    const latest = values[values.length - 1];
    return '<strong>' + latest.toFixed(1) + ' ' + unit + '</strong><br>Мин. ' +
        min.toFixed(1) + ' · макс. ' + max.toFixed(1);
}
function updateChartTheme() {
    const dark = document.body.classList.contains('theme-dark');
    const palette = dark
    ? {text: '#a3a8b3', grid: 'rgba(205,211,224,.12)', temperature: '#a9b8d2', humidity: '#83b8d8', temperatureFill: 'rgba(169,184,210,.16)', humidityFill: 'rgba(131,184,216,.14)'}
    : {text: '#777269', grid: 'rgba(88,77,61,.10)', temperature: '#596c8e', humidity: '#477ca5', temperatureFill: 'rgba(89,108,142,.08)', humidityFill: 'rgba(71,124,165,.08)'};
    [temperatureChart, humidityChart].forEach((chart, index) => {
        if (!chart) return;
        const color = index === 0 ? palette.temperature : palette.humidity;
        chart.data.datasets[0].borderColor = color;
        chart.data.datasets[0].backgroundColor = index === 0 ? palette.temperatureFill : palette.humidityFill;
        chart.data.datasets[0].pointHoverBorderColor = color;
        chart.options.scales.x.ticks.color = palette.text;
        chart.options.scales.y.ticks.color = palette.text;
        chart.options.scales.x.grid.color = palette.grid;
        chart.options.scales.y.grid.color = palette.grid;
        chart.options.scales.y.title.color = palette.text;
        chart.update('none');
    });
}
function applyTheme(theme) {
    const dark = theme === 'dark';
    document.body.classList.toggle('theme-dark', dark);
    const button = document.getElementById('themeToggle');
    const icon = document.getElementById('themeIcon');
    button.setAttribute('aria-label', dark ? 'Включить светлую тему' : 'Включить тёмную тему');
    button.title = dark ? 'Включить светлую тему' : 'Включить тёмную тему';
    icon.textContent = dark ? '☀' : '☾';
    updateChartTheme();
}
function toggleTheme() {
    const dark = !document.body.classList.contains('theme-dark');
    applyTheme(dark ? 'dark' : 'light');
    try { localStorage.setItem('thermosmart_theme', dark ? 'dark' : 'light'); } catch(e) {}
}
function loadTheme() {
    let theme = 'light';
    try { theme = localStorage.getItem('thermosmart_theme') || 'light'; } catch(e) {}
    applyTheme(theme);
}
function renderCharts(records, days) {
    if (typeof Chart === 'undefined') throw new Error('Chart.js is unavailable');
    const labels = records.map(record => formatAxisDate(record.t, days));
    function createChartOptions() {
        return {
        responsive: true,
        maintainAspectRatio: false,
        animation: false,
        interaction: {intersect: false, mode: 'index'},
        plugins: {
            legend: {display: false},
            tooltip: {
                displayColors: false,
                callbacks: {
                    title: items => items.length ? formatFullDate(records[items[0].dataIndex].t) : '',
                    label: item => item.dataset.label + ': ' + Number(item.parsed.y).toFixed(1) + ' ' + item.dataset.unit
                }
            }
        },
        scales: {
            x: {
                ticks: {maxTicksLimit: 7, maxRotation: 0, autoSkip: true, font: {size: 10}},
                grid: {drawTicks: false}
            },
            y: {
                beginAtZero: false,
                grace: '8%',
                title: {display: true, font: {size: 10}},
                ticks: {font: {size: 10}, maxTicksLimit: 6},
                grid: {drawTicks: false}
            }
        }
        };
    }
    if (temperatureChart) temperatureChart.destroy();
    if (humidityChart) humidityChart.destroy();
    const temperatureOptions = createChartOptions();
    temperatureOptions.scales.y.title.text = 'Температура, °C';
    temperatureChart = new Chart(document.getElementById('temperatureChart'), {
        type: 'line',
        data: {labels: labels, datasets: [{
            label: 'Температура', unit: '°C',
            data: records.map(record => Number(record.tmp)),
            borderColor: '#596c8e', backgroundColor: 'rgba(89,108,142,.08)',
            fill: true, tension: .25, pointRadius: 0, pointHoverRadius: 4,
            pointHitRadius: 8, borderWidth: 2
        }]},
        options: temperatureOptions
    });
    const humidityOptions = createChartOptions();
    humidityOptions.scales.y.min = 0;
    humidityOptions.scales.y.max = 100;
    humidityOptions.scales.y.grace = 0;
    humidityOptions.scales.y.title.text = 'Влажность, %';
    humidityChart = new Chart(document.getElementById('humidityChart'), {
        type: 'line',
        data: {labels: labels, datasets: [{
            label: 'Влажность', unit: '%',
            data: records.map(record => Number(record.hum)),
            borderColor: '#477ca5', backgroundColor: 'rgba(71,124,165,.08)',
            fill: true, tension: .25, pointRadius: 0, pointHoverRadius: 4,
            pointHitRadius: 8, borderWidth: 2
        }]},
        options: humidityOptions
    });
    updateChartTheme();
}
async function loadStats(days) {
    try {
        const response = await fetch('/api/stats?days=' + days + '&t=' + Date.now(), {
            headers: getHeaders(), cache: 'no-cache'
        });
        if (response.status === 401) { window.location.href = '/login'; return; }
        if (!response.ok) throw new Error('HTTP ' + response.status);
        const result = await response.json();
        const records = Array.isArray(result.data)
            ? result.data.filter(record => Number.isFinite(Number(record.t)) &&
                Number.isFinite(Number(record.tmp)) && Number.isFinite(Number(record.hum)))
                .sort((a, b) => Number(a.t) - Number(b.t))
            : [];
        document.getElementById('updateTime').textContent =
            'Обновлено: ' + new Date().toLocaleTimeString('ru-RU', {hour: '2-digit', minute: '2-digit'});
        document.getElementById('dataCount').textContent = 'Записей: ' + records.length;
        const hasData = records.length > 0;
        document.getElementById('charts').classList.toggle('hidden', !hasData);
        document.getElementById('emptyState').classList.toggle('hidden', hasData);
        if (!hasData) {
            if (temperatureChart) { temperatureChart.destroy(); temperatureChart = null; }
            if (humidityChart) { humidityChart.destroy(); humidityChart = null; }
            return;
        }
        document.getElementById('temperatureSummary').innerHTML =
            summaryText(records.map(record => Number(record.tmp)), '°C');
        document.getElementById('humiditySummary').innerHTML =
            summaryText(records.map(record => Number(record.hum)), '%');
        renderCharts(records, days);
    } catch(error) {
        console.error('Stats error:', error);
        showToast('Не удалось загрузить статистику', true);
    }
}
async function clearStats() {
    if (!confirm('Удалить всю историю показаний?')) return;
    try {
        const response = await fetch('/api/clear-stats', {method: 'POST', headers: getHeaders()});
        if (response.status === 401) { window.location.href = '/login'; return; }
        if (!response.ok) throw new Error('HTTP ' + response.status);
        if (temperatureChart) { temperatureChart.destroy(); temperatureChart = null; }
        if (humidityChart) { humidityChart.destroy(); humidityChart = null; }
        document.getElementById('charts').classList.add('hidden');
        document.getElementById('emptyState').classList.remove('hidden');
        document.getElementById('dataCount').textContent = 'Записей: 0';
        document.getElementById('updateTime').textContent =
            'Обновлено: ' + new Date().toLocaleTimeString('ru-RU', {hour: '2-digit', minute: '2-digit'});
        showToast('История показаний удалена');
    } catch(error) {
        console.error('Clear stats error:', error);
        showToast('Не удалось очистить статистику', true);
    }
}
document.addEventListener('DOMContentLoaded', async function() {
    loadTheme();
    if (await checkAuth()) loadStats(currentDays);
});
</script>
</body>
</html>
)rawliteral";

#endif
