#ifndef THERMOSMART_WEB_MAIN_PAGE_H
#define THERMOSMART_WEB_MAIN_PAGE_H

// Main ThermoSmart dashboard template.
const char MAIN_PAGE[] PROGMEM = R"rawliteral(
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
    <title>ThermoSmart | Управление</title>
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
            --field: #f7f3eb;
            --danger: #b4433d;
            --danger-soft: #fff0ee;
            --warning: #8b681b;
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
        button,input { font: inherit; }
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
        .theme-toggle:active { transform: translateY(1px); }
        .theme-toggle:focus-visible,.action-btn:focus-visible,.btn-sm:focus-visible,.mode-btn:focus-visible,.btn-manual:focus-visible,.ota-btn:focus-visible,.footer-link:focus-visible { outline: 2px solid var(--accent); outline-offset: 2px; }
        .badge { display: inline-flex; align-items: center; gap: 8px; min-height: 36px; padding: 0 12px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface); color: var(--muted); font-size: 11px; font-weight: 700; letter-spacing: .04em; }
        .badge-on { border-color: #b8dfc7; background: #eaf6ee; color: #267347; }
        .badge-off { border-color: var(--line); background: var(--surface); color: var(--muted); }
        .badge-dot { width: 8px; height: 8px; flex: none; border-radius: 50%; background: #9daaa3; }
        .badge-dot.on { background: #38a765; animation: pulse 2s infinite; }
        .badge-dot.off { background: #9daaa3; }
        @keyframes pulse { 50% { opacity: .35; } }
        #mainContent { display: none; padding-top: 30px; }
        .page-heading { display: flex; align-items: end; justify-content: space-between; gap: 16px; margin-bottom: 20px; }
        .eyebrow { margin: 0 0 5px; color: var(--accent); font-size: 10px; font-weight: 700; letter-spacing: .12em; text-transform: uppercase; }
        .page-heading h1 { margin: 0; color: var(--ink); font-family: Outfit, 'Avenir Next', sans-serif; font-size: 27px; font-weight: 600; line-height: 1.18; letter-spacing: 0; }
        .device-version { color: var(--muted); font-size: 12px; white-space: nowrap; }
        .sensor-alert { display: flex; align-items: center; gap: 12px; margin: -4px 0 18px; padding: 13px 15px; border: 1px solid #e8bbb5; border-radius: 10px; background: #fff0ee; color: #8f302b; }
        .sensor-alert-mark { display: grid; place-items: center; width: 30px; height: 30px; flex: none; border-radius: 50%; background: #b4433d; color: #fff; font-size: 18px; font-weight: 700; }
        .sensor-alert strong,#sensorFaultMessage { display: block; }
        .sensor-alert strong { margin-bottom: 3px; font-size: 13px; }
        #sensorFaultMessage { font-size: 12px; line-height: 1.5; }
        .climate-strip {
            display: grid;
            grid-template-columns: repeat(3, minmax(0, 1fr));
            gap: 14px;
            margin-bottom: 20px;
            padding: 14px;
            border: 1px solid rgba(89, 108, 142, .2);
            border-radius: 22px;
            background: linear-gradient(180deg, rgba(251,248,241,.98), rgba(239,233,222,.98));
            box-shadow: 0 18px 38px rgba(34, 39, 49, .08);
        }
        .climate-metric {
            position: relative;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            gap: 12px;
            min-width: 0;
            min-height: 132px;
            padding: 16px 18px 18px;
            border: 1px solid rgba(100, 82, 57, .1);
            border-radius: 18px;
            background: linear-gradient(180deg, #fbf8f1, #f1ece2);
            box-shadow: inset 0 1px 0 rgba(255,255,255,.8);
        }
        .climate-metric.temperature { --metric-accent: #d7642d; --metric-soft: #fff0eb; }
        .climate-metric.humidity { --metric-accent: #1f7fa5; --metric-soft: #eaf9ff; }
        .climate-metric.target { --metric-accent: #75658f; --metric-soft: #f0edf5; }
        .metric-head {
            display: flex;
            align-items: center;
            justify-content: space-between;
            gap: 8px;
        }
        .metric-head .sensor-label {
            display: inline-flex;
            align-items: center;
            color: var(--muted);
            font-size: 16px;
            font-weight: 700;
            letter-spacing: .1em;
            text-transform: uppercase;
        }
        .metric-state {
            display: inline-flex;
            align-items: center;
            min-height: 22px;
            padding: 0 8px;
            border: 1px solid rgba(23, 64, 52, .08);
            border-radius: 999px;
            background: rgba(251,248,241,.72);
            color: var(--muted);
            font-size: 10px;
            font-weight: 700;
            letter-spacing: .04em;
            text-transform: uppercase;
        }
        .metric-main {
            display: flex;
            align-items: center;
            justify-content: space-between;
            gap: 14px;
        }
        .metric-icon {
            display: grid;
            place-items: center;
            width: 50px;
            height: 50px;
            flex: none;
            border: 1px solid rgba(100, 82, 57, .1);
            border-radius: 14px;
            background: var(--metric-soft);
            color: var(--metric-accent);
            box-shadow: inset 0 1px 0 rgba(255,255,255,.8);
        }
        .metric-icon svg { display: block; width: 26px; height: 26px; fill: none; stroke: currentColor; stroke-width: 1.8; stroke-linecap: round; stroke-linejoin: round; }
        .sensor-label {
            display: block;
            color: var(--muted);
            font-size: 11px;
            font-weight: 600;
        }
        .sensor-value {
            margin-top: 6px;
            color: var(--ink);
            font-family: Outfit, sans-serif;
            font-size: 50px;
            font-weight: 700;
            line-height: 1;
            letter-spacing: -.04em;
            font-variant-numeric: tabular-nums;
            white-space: nowrap;
        }
        .sensor-value.relay-state { font-size: 25px; letter-spacing: 0; }
        .sensor-value .unit {
            margin-left: 4px;
            color: var(--muted);
            font-family: 'Avenir Next', sans-serif;
            font-size: 13px;
            font-weight: 600;
            letter-spacing: normal;
        }
        .dashboard-grid { display: grid; grid-template-columns: minmax(0, 1.35fr) minmax(320px, .9fr); align-items: start; gap: 18px; }
        .panel { min-width: 0; overflow: hidden; border: 1px solid var(--line); border-radius: 12px; background: var(--surface); box-shadow: var(--shadow); }
        .control-panel { border-top: 3px solid var(--accent); }
        .panel-head { display: flex; align-items: flex-start; justify-content: space-between; gap: 12px; padding: 22px 24px 16px; }
        .panel-title { margin: 0; color: var(--ink); font-family: Outfit, sans-serif; font-size: 17px; font-weight: 600; }
        .panel-caption { margin: 5px 0 0; color: var(--muted); font-size: 12px; }
        .card-sub { color: var(--muted); font-size: 11px; text-align: right; }
        .control-body { padding: 0 24px 22px; }
        .mode-group { display: grid; grid-template-columns: repeat(3, minmax(0, 1fr)); gap: 4px; padding: 4px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); }
        .mode-btn { min-width: 0; min-height: 38px; padding: 8px 10px; border: 0; border-radius: 6px; background: transparent; color: var(--muted); font-size: 14px; font-weight: 600; cursor: pointer; transition: background .16s, color .16s; }
        .mode-btn:hover:not(.active) { color: var(--ink); }
        .mode-btn.active { background: var(--surface); color: var(--accent-strong); box-shadow: 0 1px 4px rgba(34, 39, 49, .1); }
        .mode-info { margin: 12px 0 0; padding: 10px 12px; border-radius: 7px; background: var(--accent-soft); color: var(--accent-strong); font-size: 12px; }
        .mode-settings { margin-top: 18px; }
        .setting-row { display: flex; align-items: center; justify-content: space-between; gap: 14px; min-height: 50px; padding: 9px 0; border-bottom: 1px solid var(--line); }
        .setting-row:last-child { border-bottom: 0; }
        .setting-label { color: var(--ink); font-size: 14px; font-weight: 600; }
        .setting-label small { display: block; margin-top: 3px; color: var(--muted); font-size: 10px; font-weight: 400; }
        .setting-value { display: flex; align-items: center; gap: 9px; }
        .setting-value .val { min-width: 42px; color: var(--ink); font-family: Outfit, sans-serif; font-size: 18px; font-weight: 600; text-align: center; font-variant-numeric: tabular-nums; }
        .setting-value .val.accent { color: var(--accent); }
        .btn-icon { display: grid; place-items: center; width: 32px; height: 32px; border: 1px solid var(--line); border-radius: 7px; background: var(--surface); color: var(--ink); font-size: 18px; cursor: pointer; }
        .btn-icon:hover { border-color: var(--accent); background: var(--accent-soft); }
        .time-input { width: 68px; min-height: 34px; padding: 4px 7px; border: 1px solid var(--line-strong); border-radius: 7px; background: var(--field); color: var(--ink); font-size: 12px; text-align: center; }
        .time-input:focus,.pass-input:focus { outline: 2px solid rgba(89, 108, 142, .16); border-color: var(--accent); }
        .manual-group { display: flex; gap: 10px; padding: 8px 0; }
        .btn-manual { flex: 1; min-height: 40px; border: 1px solid transparent; border-radius: 7px; font-size: 12px; font-weight: 700; cursor: pointer; }
        .btn-manual.on { border-color: #b8dfc7; background: #eaf6ee; color: #267347; }
        .btn-manual.off { border-color: #efc6c2; background: var(--danger-soft); color: var(--danger); }
        .manual-status { margin-top: 6px; color: var(--muted); font-size: 11px; text-align: center; }
        .settings-panel { padding: 0 20px 4px; }
        .expand-toggle { display: flex; align-items: center; justify-content: space-between; width: 100%; padding: 19px 2px; border: 0; background: transparent; text-align: left; cursor: pointer; }
        .settings-heading { display: flex; align-items: center; gap: 10px; color: var(--ink); font-family: Outfit, sans-serif; font-size: 16px; font-weight: 600; }
        .settings-mark { display: grid; place-items: center; width: 32px; height: 32px; border-radius: 8px; background: var(--accent-soft); font-family: sans-serif; font-size: 15px; }
        .expand-arrow { color: var(--muted); font-size: 17px; transition: transform .2s; }
        .expand-arrow.open { transform: rotate(180deg); }
        .expand-body { max-height: 0; overflow: hidden; padding: 0 2px; opacity: 0; transition: max-height .35s ease, opacity .2s ease, padding .2s ease; }
        .expand-body.open { max-height: 1800px; padding: 0 2px 20px; opacity: 1; }
        .divider { height: 1px; margin: 7px 0; background: var(--line); }
        .s-row { display: grid; grid-template-columns: minmax(0, 1fr); gap: 10px; padding: 13px 0; }
        .s-row .s-label { display: flex; align-items: center; gap: 9px; color: var(--ink); font-size: 14px; font-weight: 600; }
        .s-row .s-label .ico { display: grid; place-items: center; width: 30px; height: 30px; flex: none; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); font-size: 15px; }
        .s-row.buzzer-row { grid-template-columns: minmax(0, 1fr) auto; align-items: center; }
        .s-controls { display: flex; align-items: center; justify-content: flex-start; gap: 8px; flex-wrap: wrap; min-width: 0; }
        .toggle-label { color: var(--muted); font-size: 11px; }
        .toggle { position: relative; width: 38px; height: 22px; border: 1px solid var(--line-strong); border-radius: 12px; background: #dce4df; cursor: pointer; transition: background .16s; }
        .toggle .knob { position: absolute; top: 2px; left: 2px; width: 16px; height: 16px; border-radius: 50%; background: #fff; box-shadow: 0 1px 3px rgba(0,0,0,.18); transition: transform .16s; }
        .toggle.active { border-color: var(--accent); background: var(--accent); }
        .toggle.active .knob { transform: translateX(16px); }
        .toggle-label.on { color: var(--accent); }
        .wifi-controls { display: grid; width: 100%; gap: 9px; }
        .wifi-actions { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 8px; }
        .wifi-entry { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 8px; }
        .wifi-entry .btn-sm { grid-column: 1 / -1; }
        .password-controls { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 8px; }
        .system-actions { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 8px; }
        .ota-controls { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 8px; }
        .ota-controls .ota-info { grid-column: 1 / -1; }
        .pass-input { width: 100%; min-width: 0; min-height: 40px; padding: 8px 11px; border: 1px solid var(--line-strong); border-radius: 9px; background: var(--field); color: var(--ink); font-size: 12px; }
        .pass-input::placeholder { color: var(--muted); }
        .btn-sm { min-height: 40px; padding: 8px 12px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); color: var(--ink); font-size: 14px; font-weight: 600; cursor: pointer; }
        .btn-sm.primary { border-color: var(--accent); background: var(--accent); color: #000; }
        .btn-sm.primary:hover { background: var(--accent-strong); }
        .action-btn { min-width: 0; min-height: 38px; padding: 7px 10px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); color: var(--muted); font-size: 13px; font-weight: 600; cursor: pointer; }
        .action-btn:hover { border-color: var(--accent); background: var(--surface-soft); color: var(--ink); }
        .action-btn.danger { border-color: #efc6c2; color: var(--danger); }
        .action-btn.warning { border-color: #ead6a5; color: var(--warning); }
        .action-btn.logout { color: var(--ink); }
        .pass-status { width: 100%; margin-top: 6px; color: var(--muted); font-size: 10px; line-height: 1.45; }
        .pass-status.success { color: #267347; }
        .pass-status.error { color: var(--danger); }
        .pass-status.info { color: var(--accent-strong); }
        .ota-label { display: inline-flex; align-items: center; justify-content: center; min-width: 0; min-height: 38px; padding: 7px 10px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); color: var(--ink); font-size: 10px; font-weight: 600; text-align: center; cursor: pointer; }
        .ota-label input { display: none; }
        .ota-btn { min-width: 0; min-height: 38px; padding: 7px 10px; border: 1px solid var(--line); border-radius: 9px; background: var(--surface-soft); color: var(--ink); font-size: 10px; font-weight: 600; cursor: pointer; }
        .ota-btn.active { border-color: #b8dfc7; background: #eaf6ee; color: #267347; }
        .ota-btn:disabled { opacity: .45; cursor: not-allowed; }
        .ota-info { color: var(--muted); font-size: 10px; }
        .ota-progress { width: 100%; height: 3px; margin-top: 8px; overflow: hidden; border-radius: 3px; background: var(--line); }
        .ota-progress .fill { width: 0; height: 100%; background: var(--accent); transition: width .2s; }
        .ota-status { display: none; width: 100%; margin-top: 7px; padding: 7px 9px; border-radius: 6px; font-size: 10px; }
        .ota-status.success { display: block; background: #eaf6ee; color: #267347; }
        .ota-status.error { display: block; background: var(--danger-soft); color: var(--danger); }
        .ota-status.info { display: block; background: var(--accent-soft); color: var(--accent-strong); }
        .hidden { display: none !important; }
        .wifi-network-list { display: flex; flex-direction: column; gap: 6px; max-height: 180px; margin-top: 9px; overflow: auto; }
        .wifi-network-list button { width: 100%; display: flex; align-items: center; justify-content: space-between; gap: 10px; padding: 9px 10px; border: 1px solid var(--line); border-radius: 7px; background: var(--field); color: var(--ink); text-align: left; cursor: pointer; }
        .wifi-network-list button span:last-child { color: var(--muted); font-size: 10px; }
        .wifi-network-list>div { padding: 10px; border: 1px solid var(--line); border-radius: 7px; color: var(--muted); font-size: 11px; text-align: center; }
        .app-footer { display: flex; align-items: center; justify-content: space-between; gap: 14px; margin-top: 18px; padding: 12px 2px 2px; color: var(--muted); }
        .datetime { font-family: ui-monospace, Consolas, monospace; font-size: 14px; font-variant-numeric: tabular-nums; }
        .footer-right { display: flex; align-items: center; gap: 10px; flex-wrap: wrap; }
        .footer-link { padding: 5px 8px; border: 1px solid var(--line); border-radius: 6px; color: var(--muted); font-size: 12px; text-decoration: none; }
        .footer-link:hover { border-color: var(--accent); color: var(--accent); }
        .footer-uptime { display: flex; align-items: center; gap: 6px; font-size: 14px; }
        .uptime-led { width: 7px; height: 7px; border-radius: 50%; background: #42a76d; box-shadow: 0 0 0 3px rgba(66,167,109,.12); }
        .uptime-label { color: var(--muted); }
        .uptime-value { color: var(--ink); font-family: ui-monospace, Consolas, monospace; font-variant-numeric: tabular-nums; }
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
            --field: #171a20;
            --danger: #ffaaa2;
            --danger-soft: #3b2729;
            --warning: #e0c47c;
            --shadow: 0 16px 40px rgba(0,0,0,.24);
        }
        body.theme-dark .badge-on,body.theme-dark .btn-manual.on,body.theme-dark .ota-btn.active { border-color: #4b5c78; background: #293345; color: #c2cde0; }
        body.theme-dark .badge-off { border-color: var(--line); background: var(--surface); color: var(--muted); }
        body.theme-dark .sensor-alert { border-color: #70464a; background: var(--danger-soft); color: var(--danger); }
        body.theme-dark .btn-manual.off,body.theme-dark .action-btn.danger,body.theme-dark .toast.error { border-color: #70464a; background: var(--danger-soft); color: var(--danger); }
        body.theme-dark .action-btn.warning { border-color: #66583b; color: var(--warning); }
        body.theme-dark .toast:not(.error) { border-color: var(--line); background: #252a34; color: var(--ink); }
        body.theme-dark .climate-strip { border-color: rgba(185, 196, 216, .14); background: linear-gradient(180deg, #20232b, #191c23); box-shadow: 0 18px 38px rgba(0, 0, 0, .24); }
        body.theme-dark .climate-metric { border-color: rgba(205, 211, 224, .08); background: linear-gradient(180deg, #272b34, #22262e); box-shadow: inset 0 1px 0 rgba(255, 255, 255, .035); }
        body.theme-dark .climate-metric.temperature { --metric-accent: #ff9a70; --metric-soft: #482d27; }
        body.theme-dark .climate-metric.humidity { --metric-accent: #79cbed; --metric-soft: #203b47; }
        body.theme-dark .climate-metric.target { --metric-accent: #c0a8ef; --metric-soft: #383044; }
        body.theme-dark .metric-icon { border-color: rgba(205, 211, 224, .08); box-shadow: inset 0 1px 0 rgba(255, 255, 255, .04); }
        body.theme-dark .metric-state { border-color: rgba(205, 211, 224, .1); background: rgba(255, 255, 255, .035); color: var(--muted); }
        @media (max-width: 820px) {
            body { padding: 18px; }
            .dashboard-grid { grid-template-columns: minmax(0,1fr); }
        }
        @media (max-width: 560px) {
            body { padding: 12px; }
            .topbar { gap: 10px; padding-bottom: 16px; }
            .brand-mark { width: 38px; height: 38px; }
            .brand-caption { display: none; }
            .top-actions { gap: 7px; }
            .theme-toggle { width: 36px; height: 36px; }
            .badge { min-height: 34px; padding: 0 9px; font-size: 10px; }
            #mainContent { padding-top: 22px; }
            .page-heading { align-items: flex-start; margin-bottom: 14px; }
            .page-heading h1 { font-size: 23px; }
            .device-version { padding-top: 4px; font-size: 10px; }
            .climate-strip { grid-template-columns: 1fr; gap: 10px; margin-bottom: 14px; padding: 10px; }
            .climate-metric { min-height: 100px; padding: 12px 14px; }
            .metric-main { width: 100%; justify-content: flex-start; }
            .metric-head .sensor-label { letter-spacing: .06em; }
            .metric-icon { width: 36px; height: 36px; border-radius: 10px; }
            .metric-icon svg { width: 21px; height: 21px; }
            .sensor-value { font-size: 25px; }
            .sensor-value .unit { font-size: 11px; }
            .sensor-label { font-size: 9px; }
            .dashboard-grid { gap: 12px; }
            .panel-head { padding: 17px 16px 13px; }
            .control-body { padding: 0 16px 16px; }
            .settings-panel { padding: 0 15px; }
            .mode-btn { padding: 8px 4px; font-size: 10px; }
            .setting-label { font-size: 12px; }
            .setting-value { gap: 6px; }
            .setting-value .val { min-width: 36px; font-size: 16px; }
            .btn-icon { width: 29px; height: 29px; }
            .s-row { gap: 9px; padding: 12px 0; }
            .s-row .s-controls { width: 100%; }
            .app-footer { align-items: flex-start; flex-direction: column; gap: 10px; }
            .footer-right { width: 100%; justify-content: space-between; }
        }
        @media (prefers-reduced-motion: reduce) { *,*::before,*::after { scroll-behavior: auto !important; transition: none !important; } }
    </style>
</head>
<body>
<div class="app-shell">
    <div id="toast" class="toast" role="status" aria-live="polite"></div>
    <header class="topbar">
        <div class="brand">
            <a href="/" class="brand-mark" title="Домой" aria-label="Домой"><svg viewBox="0 0 42 42" fill="none" xmlns="http://www.w3.org/2000/svg" aria-hidden="true"><path d="M16 23V12.5a5 5 0 0 1 10 0V23a8.5 8.5 0 1 1-10 0Z" stroke="#fff" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/><path d="M21 19v12" stroke="#fff" stroke-width="2.5" stroke-linecap="round"/><circle cx="21" cy="31" r="2.8" fill="#fff"/></svg></a>
            <span><span class="brand-name">ThermoSmart</span><span class="brand-caption">Климат-контроль</span></span>
        </div>
        <div class="top-actions">
            <button class="theme-toggle" id="themeToggle" type="button" onclick="toggleTheme()" aria-label="Включить тёмную тему" title="Включить тёмную тему"><span id="themeIcon" aria-hidden="true">☾</span></button>
            <span class="badge badge-off" id="relayBadge"><span class="badge-dot off"></span> ВЫКЛ</span>
        </div>
    </header>
    <main id="mainContent">
        <div class="page-heading">
            <div><p class="eyebrow">ПАНЕЛЬ УПРАВЛЕНИЯ</p><h1>Климат в помещении</h1></div>
            <span class="device-version">ESP32 Relay X1 v1.2</span>
        </div>
        <section class="sensor-alert hidden" id="sensorFaultAlert" role="alert" aria-live="assertive">
            <span class="sensor-alert-mark" aria-hidden="true">!</span>
            <div><strong>Ошибка датчика SHTC3</strong><span id="sensorFaultMessage"></span></div>
        </section>
        <section class="climate-strip" aria-label="Текущие показатели">
            <div class="climate-metric temperature">
                <div class="metric-head"><span class="sensor-label">Температура</span><span class="metric-state">сейчас</span></div>
                <div class="metric-main"><span class="metric-icon" aria-hidden="true"><svg viewBox="0 0 32 32"><path d="M13 19.5V8a3.5 3.5 0 0 1 7 0v11.5a7 7 0 1 1-7 0Z"/><path d="M16.5 13v11"/><circle cx="16.5" cy="24" r="2.5" fill="currentColor" stroke="none"/><path d="M20 11h2.5M20 15h2.5"/></svg></span><div class="metric-copy"><div class="sensor-value"><span id="tempDisplay">--</span><span class="unit">°C</span></div></div></div>
            </div>
            <div class="climate-metric humidity">
                <div class="metric-head"><span class="sensor-label">Влажность</span><span class="metric-state">воздух</span></div>
                <div class="metric-main"><span class="metric-icon" aria-hidden="true"><svg viewBox="0 0 32 32"><path d="M16 4.5s-8 9.2-8 15a8 8 0 0 0 16 0c0-5.8-8-15-8-15Z"/><path d="M12.5 20.5a3.8 3.8 0 0 0 3.8 3.8"/></svg></span><div class="metric-copy"><div class="sensor-value"><span id="humDisplay">--</span><span class="unit">%</span></div></div></div>
            </div>
            <div class="climate-metric target">
                <div class="metric-head"><span class="sensor-label" id="targetLabel">Задано</span><span class="metric-state" id="targetState">цель</span></div>
                <div class="metric-main"><span class="metric-icon" aria-hidden="true"><svg viewBox="0 0 32 32"><circle cx="16" cy="16" r="10.5"/><circle cx="16" cy="16" r="5.5"/><path d="M16 2.5v4M16 25.5v4M2.5 16h4M25.5 16h4"/><circle cx="16" cy="16" r="1.5" fill="currentColor" stroke="none"/></svg></span><div class="metric-copy"><div class="sensor-value" id="targetValue"><span id="targetDisplay">--</span><span class="unit" id="targetUnit">°C</span></div></div></div>
            </div>
        </section>
        <div class="dashboard-grid">
            <section class="panel control-panel" aria-labelledby="controlTitle">
                <div class="panel-head"><div><h2 class="panel-title" id="controlTitle">Управление режимом</h2><p class="panel-caption">Текущий режим: <span id="modeInfo">Автоматический</span></p></div></div>
                <div class="control-body">
                    <div class="mode-group" id="modeGroup">
                        <button class="mode-btn active" data-mode="auto" onclick="setMode('auto')">Автоматический</button>
                        <button class="mode-btn" data-mode="daynight" onclick="setMode('daynight')">День / ночь</button>
                        <button class="mode-btn" data-mode="manual" onclick="setMode('manual')">Ручной</button>
                    </div>
                    <div id="periodDisplay" class="mode-info hidden"></div>
                    <div id="autoSettings" class="mode-settings">
                        <div class="setting-row"><div class="setting-label">Целевая температура<small>Автоматический режим</small></div><div class="setting-value"><button class="btn-icon" aria-label="Уменьшить заданную температуру" onclick="adjustValue('autoTarget',-0.5)">−</button><span class="val accent" id="autoTargetLabel">22.0</span><button class="btn-icon" aria-label="Увеличить заданную температуру" onclick="adjustValue('autoTarget',0.5)">+</button></div></div>
                    </div>
                    <div id="daynightSettings" class="mode-settings hidden">
                        <div class="setting-row"><div class="setting-label">Дневная температура<small>День / ночь</small></div><div class="setting-value"><button class="btn-icon" aria-label="Уменьшить дневную температуру" onclick="adjustValue('dayTemp',-0.5)">−</button><span class="val accent" id="dayTempLabel">22.0</span><button class="btn-icon" aria-label="Увеличить дневную температуру" onclick="adjustValue('dayTemp',0.5)">+</button></div></div>
                        <div class="setting-row"><div class="setting-label">Ночная температура<small>День / ночь</small></div><div class="setting-value"><button class="btn-icon" aria-label="Уменьшить ночную температуру" onclick="adjustValue('nightTemp',-0.5)">−</button><span class="val accent" id="nightTempLabel">18.0</span><button class="btn-icon" aria-label="Увеличить ночную температуру" onclick="adjustValue('nightTemp',0.5)">+</button></div></div>
                        <div class="setting-row"><label class="setting-label" for="dayStartInput">Начало дня</label><input class="time-input" type="text" id="dayStartInput" value="07:00" maxlength="5" inputmode="numeric"></div>
                        <div class="setting-row"><label class="setting-label" for="nightStartInput">Начало ночи</label><input class="time-input" type="text" id="nightStartInput" value="23:00" maxlength="5" inputmode="numeric"></div>
                    </div>
                    <div id="manualSettings" class="mode-settings hidden">
                        <div class="manual-group"><button class="btn-manual on" onclick="setManualRelay(true)">Включить реле</button><button class="btn-manual off" onclick="setManualRelay(false)">Выключить реле</button></div>
                        <div class="manual-status" id="manualStatus">Реле выключено</div>
                    </div>
                    <div class="setting-row hysteresis-row"><div class="setting-label">Гистерезис<small>Порог переключения реле</small></div><div class="setting-value"><button class="btn-icon" aria-label="Уменьшить гистерезис" onclick="adjustValue('hysteresis',-0.1)">−</button><span class="val accent" id="hysteresisLabel">0.5</span><button class="btn-icon" aria-label="Увеличить гистерезис" onclick="adjustValue('hysteresis',0.1)">+</button></div></div>
                </div>
            </section>
            <section class="panel settings-panel" aria-labelledby="settingsTitle">
                <button class="expand-toggle" type="button" onclick="toggleSettings()" aria-expanded="false"><span class="settings-heading"><span class="settings-mark" aria-hidden="true">⚙</span><span id="settingsTitle">Настройки устройства</span></span><span class="expand-arrow" id="settingsArrow" aria-hidden="true">⌄</span></button>
                <div class="expand-body" id="settingsBody">
                    <div class="divider"></div>
                    <div class="s-row buzzer-row"><span class="s-label"><span class="ico" aria-hidden="true">🔊</span>Звуковой сигнал</span><div class="s-controls"><span class="toggle-label" id="buzzerLabel">Вкл</span><button class="toggle active" id="buzzerToggle" type="button" role="switch" aria-label="Звуковой сигнал" aria-checked="true" onclick="toggleBuzzer()"><span class="knob"></span></button></div></div>
                    <div class="divider"></div>
                    <div class="s-row wifi-row"><span class="s-label"><span class="ico" aria-hidden="true">📶</span>Подключение Wi-Fi</span><div class="s-controls"><div class="wifi-controls"><div class="wifi-actions"><button class="action-btn" type="button" onclick="scanWiFiNetworks()">Найти сети</button><button class="action-btn" type="button" onclick="loadWiFiStatus()">Статус сети</button></div><div class="wifi-entry"><input class="pass-input" type="text" id="wifiSsid" placeholder="Имя сети (SSID)" autocomplete="off"><input class="pass-input" type="password" id="wifiPassword" placeholder="Пароль сети" autocomplete="new-password"><button class="btn-sm primary" type="button" onclick="connectWiFi()">Подключить</button></div><div id="wifiNetworkList" class="wifi-network-list"></div><div id="wifiStatus" class="pass-status"></div></div></div></div>
                    <div class="divider"></div>
                    <div class="s-row"><span class="s-label"><span class="ico" aria-hidden="true">🔑</span>Пароль администратора</span><div class="s-controls password-controls"><input class="pass-input" type="password" id="oldPass" placeholder="Текущий" autocomplete="current-password"><input class="pass-input" type="password" id="newPass" placeholder="Новый" autocomplete="new-password"><input class="pass-input" type="password" id="newPassConfirm" placeholder="Повторите" autocomplete="new-password"><button class="btn-sm primary" type="button" onclick="changePassword()">Сохранить</button></div></div>
                    <div id="passStatus" class="pass-status"></div>
                    <div class="divider"></div>
                    <div class="s-row system-row"><span class="s-label"><span class="ico" aria-hidden="true">🔄</span>Система</span><div class="s-controls system-actions"><button class="action-btn" type="button" onclick="restartESP()">Перезагрузка</button><button class="action-btn warning" type="button" onclick="resetWiFi()">Сброс Wi-Fi</button><button class="action-btn logout" type="button" onclick="logout()">Выйти</button><button class="action-btn danger" type="button" onclick="factoryResetESP()">Сброс устройства</button></div></div>
                    <div class="divider"></div>
                    <div class="s-row ota-row"><span class="s-label"><span class="ico" aria-hidden="true">📦</span>Обновление прошивки</span><div class="s-controls ota-controls"><label class="ota-label" id="otaLabel">Выбрать файл .bin<input type="file" name="firmware" accept=".bin" id="otaFileInput"></label><button class="ota-btn" id="otaBtn" type="button" disabled>Обновить</button><span class="ota-info" id="otaFileInfo">Файл не выбран</span></div></div>
                    <div class="ota-progress" id="otaProgressContainer" style="display:none"><div class="fill" id="otaProgressFill"></div></div>
                    <div class="ota-status" id="otaStatus"></div>
                </div>
            </section>
        </div>
        <footer class="app-footer"><span class="datetime" id="datetimeDisplay">--/--/---- --:--:--</span><div class="footer-right"><a href="/logs" class="footer-link">Журнал событий</a><a href="/stats" class="footer-link">Статистика</a><div class="footer-uptime"><span class="uptime-led"></span><span class="uptime-label">Время работы</span><span class="uptime-value" id="uptimeDisplay">00d 00:00:00</span></div></div></footer>
    </main>
</div>
<script>
// ===== ТОКЕН АВТОРИЗАЦИИ (sessionStorage) =====
function getToken() {
    try {
        return sessionStorage.getItem('thermosmart_token') || '';
    } catch(e) {
        return '';
    }
}

function getHeaders() {
    const t = getToken();
    return t ? {'Content-Type':'application/json','Authorization':'Bearer '+t} : {'Content-Type':'application/json'};
}

// ===== ОБЕРТКА ДЛЯ FETCH =====
async function authFetch(url, options={}) {
    const headers = {...getHeaders(), ...(options.headers||{})};
    const resp = await fetch(url, {...options, headers});
    if (resp.status === 401) {
        sessionStorage.removeItem('thermosmart_token');
        window.location.href = '/login';
        throw new Error('Unauthorized');
    }
    return resp;
}

// ===== ВЫХОД =====
async function logout() {
    if (!confirm('Выйти из системы?')) return;
    try {
        await fetch('/logout', {method:'POST', headers: getHeaders()});
    } catch(e) {}
    sessionStorage.removeItem('thermosmart_token');
    window.location.href = '/login';
}

// ===== ПРОВЕРКА АВТОРИЗАЦИИ ПРИ ЗАГРУЗКЕ =====
async function checkAuth() {
    const token = getToken();
    if (!token) {
        window.location.href = '/login';
        return false;
    }
    
    try {
        const resp = await fetch('/api/state?t='+Date.now(), {
            headers: getHeaders(),
            cache: 'no-cache'
        });
        if (resp.status === 401 || !resp.ok) {
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

// ===== TOAST =====
function showToast(msg,err){const t=document.getElementById('toast');t.textContent=msg;t.className='toast'+(err?' error':'');requestAnimationFrame(()=>t.classList.add('show'));clearTimeout(t._hide);t._hide=setTimeout(()=>t.classList.remove('show'),2500);}

// ===== TOGGLES =====
function toggleSettings(){const b=document.getElementById('settingsBody'),a=document.getElementById('settingsArrow');b.classList.toggle('open');a.classList.toggle('open');}

function applyTheme(theme){
    const isDark=theme==='dark';
    document.body.classList.toggle('theme-dark',isDark);
    const button=document.getElementById('themeToggle');
    const icon=document.getElementById('themeIcon');
    const label=isDark?'Включить светлую тему':'Включить тёмную тему';
    button.setAttribute('aria-label',label);
    button.title=label;
    icon.textContent=isDark?'☼':'☾';
    try{localStorage.setItem('thermosmart_theme',isDark?'dark':'light');}catch(e){}
}

function toggleTheme(){
    applyTheme(document.body.classList.contains('theme-dark')?'light':'dark');
}

function loadTheme(){
    let theme='light';
    try{theme=localStorage.getItem('thermosmart_theme')||'light';}catch(e){}
    applyTheme(theme);
}

async function loadWiFiStatus(){
    const status=document.getElementById('wifiStatus');
    try{
        const resp=await authFetch('/api/wifi/status?t='+Date.now(),{cache:'no-cache'});
        const d=await resp.json();
        if(d.status==='connected'){
            status.textContent='✅ Подключено к: '+d.ssid+' ('+d.ip+')';
            status.className='pass-status success';
        } else if(d.status==='ap'){
            status.textContent='📶 Режим точки доступа: '+d.ssid+' ('+d.ip+')';
            status.className='pass-status info';
        } else {
            status.textContent='⚠️ Wi‑Fi не подключен';
            status.className='pass-status error';
        }
    } catch(e){
        status.textContent='❌ Не удалось получить статус Wi‑Fi';
        status.className='pass-status error';
    }
}

async function scanWiFiNetworks(){
    const list=document.getElementById('wifiNetworkList');
    const status=document.getElementById('wifiStatus');
    list.innerHTML='<div style="padding:10px;border:1px solid #2a3340;border-radius:10px;color:#94a3b8;text-align:center">⏳ Поиск сетей...</div>';
    status.textContent='⏳ Сканирование Wi‑Fi...';
    status.className='pass-status info';
    try{
        const resp=await authFetch('/api/wifi/scan?t='+Date.now(),{cache:'no-cache'});
        const d=await resp.json();
        if(!d.networks||!d.networks.length){
            list.innerHTML='<div style="padding:10px;border:1px solid #2a3340;border-radius:10px;color:#94a3b8;text-align:center">📭 Сетей не найдено</div>';
            status.textContent='⚠️ Сетей не найдено';
            status.className='pass-status error';
            return;
        }
        d.networks.sort((a,b)=>b.rssi-a.rssi);
        list.innerHTML='';
        d.networks.forEach(net=>{
            const item=document.createElement('button');
            item.type='button';
            item.style.cssText='width:100%;display:flex;justify-content:space-between;align-items:center;gap:10px;padding:10px 12px;border:1px solid #2a3340;border-radius:10px;background:#0a0e14;color:#f0f4f8;cursor:pointer;text-align:left';
            item.innerHTML='<span>'+net.ssid+'</span><span style="color:#94a3b8;font-size:12px">'+net.rssi+' dBm</span>';
            item.onclick=()=>{
                document.getElementById('wifiSsid').value=net.ssid;
                document.getElementById('wifiPassword').focus();
                status.textContent='✅ Сеть выбрана: '+net.ssid;
                status.className='pass-status success';
            };
            list.appendChild(item);
        });
        status.textContent='✅ Сети найдены';
        status.className='pass-status success';
    } catch(e){
        list.innerHTML='<div style="padding:10px;border:1px solid #2a3340;border-radius:10px;color:#f87171;text-align:center">❌ Ошибка поиска</div>';
        status.textContent='❌ Ошибка поиска Wi‑Fi';
        status.className='pass-status error';
    }
}

async function connectWiFi(){
    const ssid=document.getElementById('wifiSsid').value.trim();
    const password=document.getElementById('wifiPassword').value;
    const status=document.getElementById('wifiStatus');
    if(!ssid){
        status.textContent='❌ Укажите SSID сети';
        status.className='pass-status error';
        return;
    }
    status.textContent='⏳ Подключение к '+ssid+'...';
    status.className='pass-status info';
    try{
        const resp=await authFetch('/api/wifi/connect',{method:'POST',body:JSON.stringify({ssid:ssid,password:password})});
        const d=await resp.json();
        if(d.status==='ok'){
            status.textContent='✅ Подключение сохранено. Перезагрузка...';
            status.className='pass-status success';
            setTimeout(()=>window.location.reload(),2000);
        } else {
            status.textContent='❌ '+(d.message||'Не удалось подключиться');
            status.className='pass-status error';
        }
    } catch(e){
        status.textContent='❌ Ошибка подключения к Wi‑Fi';
        status.className='pass-status error';
    }
}

let updateInterval=null,saveTimeout=null;

// ===== STATE =====
async function loadState(){
    try{
        const resp=await authFetch('/api/state?t='+Date.now(),{cache:'no-cache'});
        const d=await resp.json();
        const sensorAlert=document.getElementById('sensorFaultAlert');
        if(d.sensorFault){
            sensorAlert.classList.remove('hidden');
            document.getElementById('sensorFaultMessage').textContent='Нет данных после '+d.sensorReadFailures+' последовательных ошибок. Проверьте датчик и подключение. '+(d.mode==='manual'?'В ручном режиме реле остаётся под вашим управлением.':'Реле отключено для безопасности.');
        }else{
            sensorAlert.classList.add('hidden');
        }
        document.getElementById('tempDisplay').textContent=d.temp.toFixed(1);
        document.getElementById('humDisplay').textContent=d.hum.toFixed(0);
        const manualMode=d.mode==='manual';
        document.getElementById('targetLabel').textContent=manualMode?'Реле':d.mode==='daynight'?'Период':'Задано';
        document.getElementById('targetState').textContent=manualMode?'ручной режим':d.mode==='daynight'?(d.period==='day'?'день':'ночь'):'цель';
        document.getElementById('targetDisplay').textContent=manualMode?(d.manualRelay?'ВКЛ':'ВЫКЛ'):d.currentTarget.toFixed(1);
        document.getElementById('targetValue').classList.toggle('relay-state',manualMode);
        document.getElementById('targetUnit').classList.toggle('hidden',manualMode);
        document.querySelectorAll('.mode-btn').forEach(b=>b.classList.remove('active'));
        const mb=document.querySelector('.mode-btn[data-mode="'+d.mode+'"]');
        if(mb)mb.classList.add('active');
        document.getElementById('autoSettings').classList.toggle('hidden',d.mode!=='auto');
        document.getElementById('daynightSettings').classList.toggle('hidden',d.mode!=='daynight');
        document.getElementById('manualSettings').classList.toggle('hidden',d.mode!=='manual');
        const mn={auto:'Автоматический',daynight:'День/Ночь',manual:'Ручной'};
        document.getElementById('modeInfo').textContent=mn[d.mode]||d.mode;
        const pe=document.getElementById('periodDisplay');
        if(d.mode==='daynight'){pe.classList.remove('hidden');pe.textContent='Текущий период: '+(d.period==='day'?'🌞 День':'🌙 Ночь');}
        else{pe.classList.add('hidden');}
        document.getElementById('manualStatus').textContent=d.manualRelay?'🟢 Реле включено':'🔴 Реле выключено';
        const badge=document.getElementById('relayBadge');
        if(d.relay){badge.className='badge badge-on';badge.innerHTML='<span class="badge-dot on"></span> ВКЛ';}
        else{badge.className='badge badge-off';badge.innerHTML='<span class="badge-dot off"></span> ВЫКЛ';}
        document.getElementById('autoTargetLabel').textContent=d.autoTarget.toFixed(1);
        document.getElementById('dayTempLabel').textContent=d.dayTemp.toFixed(1);
        document.getElementById('nightTempLabel').textContent=d.nightTemp.toFixed(1);
        document.getElementById('hysteresisLabel').textContent=d.hysteresis.toFixed(1);
        const di=document.getElementById('dayStartInput'),ni=document.getElementById('nightStartInput');
        if(document.activeElement!==di)di.value=d.dayStart;
        if(document.activeElement!==ni)ni.value=d.nightStart;
        const u=d.uptime||0;
        document.getElementById('uptimeDisplay').textContent=String(Math.floor(u/86400)).padStart(2,'0')+'d '+String(Math.floor(u%86400/3600)).padStart(2,'0')+':'+String(Math.floor(u%3600/60)).padStart(2,'0')+':'+String(Math.floor(u%60)).padStart(2,'0');
        updateDateTime();
    }catch(e){}
}

function updateDateTime(){
    const n=new Date();
    document.getElementById('datetimeDisplay').textContent=String(n.getDate()).padStart(2,'0')+'/'+String(n.getMonth()+1).padStart(2,'0')+'/'+n.getFullYear()+' - '+String(n.getHours()).padStart(2,'0')+':'+String(n.getMinutes()).padStart(2,'0')+':'+String(n.getSeconds()).padStart(2,'0');
}

function startUpdates(){
    if(updateInterval)clearInterval(updateInterval);
    loadState();
    updateInterval=setInterval(loadState,2000);
    if(window._timeInterval)clearInterval(window._timeInterval);
    window._timeInterval=setInterval(updateDateTime,1000);
}

// ===== CONTROLS =====
async function setMode(mode){try{await authFetch('/api/set',{method:'POST',body:JSON.stringify({mode:mode})});showToast('✅ Режим: '+mode);loadState();}catch(e){}}
async function setManualRelay(state){try{await authFetch('/api/set',{method:'POST',body:JSON.stringify({manualRelay:state})});showToast(state?'✅ Реле включено':'✅ Реле выключено');loadState();}catch(e){}}

function adjustValue(param,step){
    const labels={autoTarget:'autoTargetLabel',dayTemp:'dayTempLabel',nightTemp:'nightTempLabel',hysteresis:'hysteresisLabel'};
    const limits={autoTarget:{min:1,max:30},dayTemp:{min:1,max:30},nightTemp:{min:1,max:30},hysteresis:{min:.1,max:2}};
    const el=document.getElementById(labels[param]);
    let val=parseFloat(el.textContent)+step;
    val=Math.round(val/Math.abs(step))*Math.abs(step);
    val=Math.max(limits[param].min,Math.min(limits[param].max,val));
    el.textContent=val.toFixed(step<1?1:0);
    clearTimeout(saveTimeout);
    saveTimeout=setTimeout(()=>{
        authFetch('/api/set',{method:'POST',body:JSON.stringify({[param]:val})}).then(()=>showToast('✅ '+param+' сохранено')).catch(()=>showToast('❌ Ошибка',true));
    },300);
}

// ===== TIME INPUTS =====
function initTimeInputs(){
    const di=document.getElementById('dayStartInput'),ni=document.getElementById('nightStartInput');
    function handleTimeChange(input,param){
        let val=input.value.trim();
        if(!/^\d{2}:\d{2}$/.test(val)){loadState();showToast('❌ Неверный формат (HH:MM)',true);return;}
        const[h,m]=val.split(':').map(Number);
        if(h<0||h>23||m<0||m>59){loadState();showToast('❌ Неверное время',true);return;}
        authFetch('/api/set',{method:'POST',body:JSON.stringify({[param]:val})}).then(()=>showToast('✅ Время сохранено')).catch(()=>showToast('❌ Ошибка',true));
    }
    di.addEventListener('blur',function(){handleTimeChange(this,'dayStart');});
    ni.addEventListener('blur',function(){handleTimeChange(this,'nightStart');});
    di.addEventListener('keydown',e=>{if(e.key==='Enter')this.blur();});
    ni.addEventListener('keydown',e=>{if(e.key==='Enter')this.blur();});
    function autoColon(input){
        let val=input.value.replace(/[^0-9]/g,'');
        if(val.length>=2)val=val.substring(0,2)+':'+val.substring(2,4);
        if(val.length>5)val=val.substring(0,5);
        input.value=val;
    }
    di.addEventListener('input',function(){autoColon(this);});
    ni.addEventListener('input',function(){autoColon(this);});
}

// ===== BUZZER =====
async function loadBuzzerState(){
    try{
        const resp=await authFetch('/api/buzzer?t='+Date.now(),{cache:'no-cache'});
        const d=await resp.json();
        updateBuzzerUI(d.enabled);
    }catch(e){}
}
function updateBuzzerUI(e){
    const t=document.getElementById('buzzerToggle'),l=document.getElementById('buzzerLabel');
    if(e){t.classList.add('active');l.textContent='Вкл';l.className='toggle-label on';}
    else{t.classList.remove('active');l.textContent='Выкл';l.className='toggle-label';}
}
async function toggleBuzzer(){
    const t=document.getElementById('buzzerToggle'),newState=!t.classList.contains('active');
    try{
        const resp=await authFetch('/api/buzzer',{method:'POST',body:JSON.stringify({enabled:newState})});
        const d=await resp.json();
        if(d.status==='ok'){updateBuzzerUI(newState);showToast(newState?'🔊 Звук включён':'🔇 Звук выключен');}
    }catch(e){showToast('❌ Ошибка',true);}
}

// ===== PASSWORD CHANGE =====
async function changePassword(){
    const oldPass=document.getElementById('oldPass').value,newPass=document.getElementById('newPass').value,confirmPass=document.getElementById('newPassConfirm').value,status=document.getElementById('passStatus');
    if(!oldPass||!newPass||!confirmPass){status.textContent='❌ Заполните все поля';status.className='pass-status error';return;}
    if(newPass.length<4){status.textContent='❌ Минимум 4 символа';status.className='pass-status error';return;}
    if(newPass!==confirmPass){status.textContent='❌ Пароли не совпадают';status.className='pass-status error';return;}
    try{
        const resp=await authFetch('/change-password',{method:'POST',body:JSON.stringify({old:oldPass,new:newPass})});
        const d=await resp.json();
        if(d.status==='ok'){
            try{sessionStorage.setItem('thermosmart_token',newPass);}catch(e){}
            status.textContent='✅ Пароль изменён!';status.className='pass-status success';
            document.getElementById('oldPass').value='';document.getElementById('newPass').value='';document.getElementById('newPassConfirm').value='';
            showToast('✅ Пароль изменён');
        }else{status.textContent='❌ '+(d.message||'Неверный старый пароль');status.className='pass-status error';}
    }catch(e){status.textContent='❌ Ошибка соединения';status.className='pass-status error';}
}
document.querySelectorAll('.pass-input').forEach(el=>{el.addEventListener('keydown',function(e){if(e.key==='Enter')changePassword();});});

// ===== ACTIONS =====
async function restartESP(){if(confirm('Перезагрузить устройство?')){await authFetch('/restart');showToast('🔄 Перезагрузка...');setTimeout(()=>window.location.reload(),3000);}}
async function factoryResetESP(){if(confirm('⚠️ Сброс к заводским настройкам? Пароль станет "admin"')){await authFetch('/factory-reset');showToast('🏭 Сброс выполнен');setTimeout(()=>window.location.reload(),3000);}}
async function resetWiFi(){if(confirm('Сбросить настройки Wi-Fi? После перезагрузки будет запущен режим AP')){await authFetch('/reset-wifi',{method:'POST'});showToast('📶 Сброс Wi-Fi');setTimeout(()=>window.location.reload(),3000);}}

// ===== OTA =====
const otaFileInput=document.getElementById('otaFileInput'),otaBtn=document.getElementById('otaBtn'),otaProgress=document.getElementById('otaProgressContainer'),otaFill=document.getElementById('otaProgressFill'),otaStatus=document.getElementById('otaStatus'),otaLabel=document.getElementById('otaLabel'),otaInfo=document.getElementById('otaFileInfo');
otaFileInput.addEventListener('change',function(e){
    const f=e.target.files[0];
    if(f){const s=(f.size/(1024*1024)).toFixed(2);otaInfo.textContent=f.name+' ('+s+' MB)';otaBtn.disabled=false;otaBtn.classList.add('active');otaBtn.textContent='⬆ Обновить';otaLabel.textContent='📤 Файл выбран';otaStatus.className='ota-status';otaStatus.style.display='none';}
    else{otaInfo.textContent='Файл не выбран';otaBtn.disabled=true;otaBtn.classList.remove('active');otaLabel.textContent='📤 Выбрать .bin';}
});
otaBtn.addEventListener('click',function(){if(!otaBtn.disabled&&otaFileInput.files.length)startOTA();});
function startOTA(){
    const file=otaFileInput.files[0];
    if(!file){showOtaStatus('Выберите файл','error');return;}
    if(!file.name.endsWith('.bin')){showOtaStatus('Файл должен быть .bin','error');return;}
    otaBtn.disabled=true;otaBtn.textContent='⏳ Загрузка...';otaBtn.classList.remove('active');otaProgress.style.display='block';showOtaStatus('Загрузка прошивки...','info');
    const fd=new FormData();fd.append('firmware',file);
    const xhr=new XMLHttpRequest();
    xhr.upload.addEventListener('progress',function(e){if(e.lengthComputable){const p=(e.loaded/e.total)*100;otaFill.style.width=p+'%';showOtaStatus('Загрузка: '+p.toFixed(0)+'%','info');}});
    xhr.onload=function(){
        if(xhr.status===200){showOtaStatus('✅ Обновление успешно! Перезагрузка...','success');setTimeout(()=>window.location.reload(),3000);}
        else{showOtaStatus('❌ Ошибка: '+xhr.status,'error');resetOTA();}
    };
    xhr.onerror=function(){otaFill.style.width='100%';showOtaStatus('✅ Обновление успешно! Перезагрузка...','success');setTimeout(()=>window.location.reload(),3000);};
    xhr.open('POST','/update',true);
    xhr.send(fd);
}
function showOtaStatus(msg,type){otaStatus.textContent=msg;otaStatus.className='ota-status '+type;otaStatus.style.display='block';}
function resetOTA(){otaBtn.disabled=true;otaBtn.textContent='⬆ Обновить';otaBtn.classList.remove('active');otaProgress.style.display='none';otaFill.style.width='0%';otaLabel.textContent='📤 Выбрать .bin';otaInfo.textContent='Файл не выбран';otaFileInput.value='';}

// ===== INIT =====
document.addEventListener('DOMContentLoaded', async function() {
    loadTheme();
    const ok = await checkAuth();
    if (!ok) return;
    
    document.getElementById('mainContent').style.display = 'grid';
    initTimeInputs();
    loadBuzzerState();
    loadWiFiStatus();
    document.getElementById('settingsBody').classList.remove('open');
    document.getElementById('settingsArrow').classList.remove('open');
    startUpdates();
    updateDateTime();
});
</script>
</body>
</html>
)rawliteral";

#endif // THERMOSMART_WEB_MAIN_PAGE_H
