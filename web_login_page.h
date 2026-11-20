#ifndef THERMOSMART_WEB_LOGIN_PAGE_H
#define THERMOSMART_WEB_LOGIN_PAGE_H

// Страница входа в веб-интерфейс.
const char LOGIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover">
    <meta name="theme-color" content="#f3efe7">
    <link rel="icon" type="image/svg+xml" href="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 64 64'%3E%3Cdefs%3E%3ClinearGradient id='g' x1='0' y1='0' x2='1' y2='1'%3E%3Cstop stop-color='%237f91b0'/%3E%3Cstop offset='1' stop-color='%23465a7b'/%3E%3C/linearGradient%3E%3C/defs%3E%3Crect width='64' height='64' rx='18' fill='url(%23g)'/%3E%3Cpath d='M25 35V19a7 7 0 0 1 14 0v16a12 12 0 1 1-14 0Z' fill='none' stroke='%23fff' stroke-width='4' stroke-linecap='round' stroke-linejoin='round'/%3E%3Cpath d='M32 27v18' stroke='%23fff' stroke-width='4' stroke-linecap='round'/%3E%3Ccircle cx='32' cy='46' r='5' fill='%23fff'/%3E%3C/svg%3E">
    <title>Вход | ThermoSmart</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root{color-scheme:light;--page:#f3efe7;--surface:#fbf8f1;--field:#f7f3eb;--ink:#302f2b;--muted:#777269;--line:#e3dbcf;--line-strong:#d0c4b4;--accent:#596c8e;--accent-dark:#465a7b;--accent-soft:#e9e9ee;}
        *{box-sizing:border-box;}
        body{display:grid;place-items:center;min-height:100vh;min-height:100svh;margin:0;padding:24px;background-color:var(--page);background-image:linear-gradient(rgba(88,77,61,.035) 1px,transparent 1px),linear-gradient(90deg,rgba(88,77,61,.035) 1px,transparent 1px);background-size:32px 32px;color:var(--ink);font-family:'Avenir Next','Segoe UI',sans-serif;transition:background-color .2s,color .2s;}
        .login-shell{display:grid;grid-template-columns:1fr 1fr;width:min(820px,100%);min-height:470px;overflow:hidden;border:1px solid var(--line);border-radius:16px;background:var(--surface);box-shadow:0 24px 70px rgba(34,39,49,.12);}
        .brand-panel{position:relative;display:flex;flex-direction:column;justify-content:space-between;overflow:hidden;padding:42px;background:#394354;color:#f4f5f8;}
        .brand-panel::after{position:absolute;right:-72px;bottom:-90px;width:290px;height:290px;border:1px solid rgba(255,255,255,.12);border-radius:50%;box-shadow:0 0 0 28px rgba(255,255,255,.025),0 0 0 58px rgba(255,255,255,.025);content:'';pointer-events:none;}
        .brand-lockup{position:relative;z-index:1;display:flex;align-items:center;gap:12px;}
        .brand-mark{display:grid;place-items:center;width:42px;height:42px;flex:none;border:1px solid rgba(255,255,255,.2);border-radius:12px;background:linear-gradient(145deg,#7f91b0,#465a7b);box-shadow:0 5px 12px rgba(0,0,0,.16),inset 0 1px 0 rgba(255,255,255,.18);text-decoration:none;transition:transform .16s,box-shadow .16s;}
        .brand-mark:hover{transform:translateY(-1px);box-shadow:0 7px 15px rgba(0,0,0,.2),inset 0 1px 0 rgba(255,255,255,.18);}
        .brand-mark:focus-visible{outline:2px solid #a9b8d2;outline-offset:3px;}
        .brand-mark svg{display:block;width:27px;height:27px;}
        .brand-name{font-family:Outfit,'Avenir Next',sans-serif;font-size:17px;font-weight:700;letter-spacing:0;}
        .brand-copy{position:relative;z-index:1;max-width:270px;margin:42px 0;}
        .brand-copy h1{margin:0 0 12px;font-family:Outfit,'Avenir Next',sans-serif;font-size:32px;line-height:1.15;letter-spacing:0;font-weight:600;}
        .brand-copy p{margin:0;color:#c7ccd6;font-size:14px;line-height:1.6;}
        .device-state{position:relative;z-index:1;display:inline-flex;align-items:center;align-self:flex-start;gap:8px;padding:7px 10px;border:1px solid rgba(224,229,239,.16);border-radius:999px;background:rgba(255,255,255,.06);color:#d6dae2;font-size:12px;line-height:1.2;white-space:nowrap;}
        .status-dot{width:8px;height:8px;flex:none;border-radius:50%;background:#a9b8d2;box-shadow:0 0 0 3px rgba(169,184,210,.14);}
        .login-panel{position:relative;display:flex;flex-direction:column;justify-content:center;padding:58px 52px 48px;}
        .theme-toggle{position:absolute;top:16px;right:18px;display:grid;place-items:center;width:36px;height:36px;border:1px solid var(--line);border-radius:9px;background:var(--surface);color:var(--ink);font-size:18px;cursor:pointer;transition:background .16s,border-color .16s;}
        .theme-toggle:hover{border-color:var(--accent);background:var(--accent-soft);}
        .theme-toggle:focus-visible{outline:2px solid var(--accent);outline-offset:2px;}
        .login-heading{margin:0;color:var(--ink);font-family:Outfit,'Avenir Next',sans-serif;font-size:24px;line-height:1.25;letter-spacing:0;font-weight:600;}
        .login-subtitle{margin:8px 0 30px;color:var(--muted);font-size:14px;line-height:1.5;}
        .login-form{display:grid;gap:10px;}
        .field-label{color:var(--ink);font-size:13px;font-weight:600;}
        .input-field{width:100%;height:48px;padding:0 14px;border:1px solid var(--line-strong);border-radius:8px;background:var(--field);color:var(--ink);font:inherit;font-size:15px;transition:border-color .16s,box-shadow .16s;}
        .input-field::placeholder{color:var(--muted);}
        .input-field:hover{border-color:var(--accent);}
        .input-field:focus{outline:none;border-color:var(--accent);box-shadow:0 0 0 3px rgba(89,108,142,.15);}
        .login-btn{height:48px;margin-top:6px;border:0;border-radius:8px;background:var(--accent);color:#fff;font:inherit;font-size:14px;font-weight:650;cursor:pointer;transition:background .16s,transform .16s;}
        .login-btn:hover{background:var(--accent-dark);}
        .login-btn:active{transform:translateY(1px);}
        .login-btn:focus-visible{outline:3px solid rgba(89,108,142,.3);outline-offset:2px;}
        .login-btn:disabled{opacity:.6;cursor:wait;}
        .error-msg{min-height:20px;margin:2px 0 0;color:#b33d36;font-size:13px;line-height:1.45;}
        .info-text{margin-top:22px;padding-top:18px;border-top:1px solid var(--line);color:var(--muted);font-size:12px;line-height:1.6;}
        .info-text code{padding:2px 6px;border-radius:4px;background:var(--accent-soft);color:var(--accent-dark);font-family:ui-monospace,Consolas,monospace;font-size:11px;}
        .info-text .hint{display:block;margin-top:4px;color:var(--muted);}
        body.theme-dark{color-scheme:dark;--page:#111318;--surface:#1b1e25;--field:#171a20;--ink:#eceef2;--muted:#a3a8b3;--line:#353943;--line-strong:#4a505c;--accent:#a9b8d2;--accent-dark:#c2cde0;--accent-soft:#2b3341;}
        body.theme-dark{background-image:linear-gradient(rgba(215,220,230,.035) 1px,transparent 1px),linear-gradient(90deg,rgba(215,220,230,.035) 1px,transparent 1px);}
        body.theme-dark .login-shell{box-shadow:0 24px 70px rgba(0,0,0,.32);}
        body.theme-dark .brand-panel{background:#242832;}
        body.theme-dark .login-btn{color:#171a20;}
        body.theme-dark .login-btn:focus-visible{outline-color:rgba(169,184,210,.35);}
        @media(max-width:640px){
            body{padding:16px;}
            .login-shell{grid-template-columns:1fr;max-width:430px;min-height:0;}
            .brand-panel{min-height:184px;padding:20px 24px;}
            .brand-panel::after{right:-95px;bottom:-190px;width:250px;height:250px;}
            .brand-copy{margin:20px 0 14px;}
            .brand-copy h1{margin-bottom:0;font-size:23px;}
            .brand-copy p{display:none;}
            .device-state{font-size:11px;}
            .login-panel{padding:52px 26px 26px;}
            .login-subtitle{margin-bottom:24px;}
        }
        @media(prefers-reduced-motion:reduce){*,*::before,*::after{scroll-behavior:auto!important;transition:none!important;}}
    </style>
</head>
<body>
<main class="login-shell">
    <section class="brand-panel" aria-label="ThermoSmart">
        <div class="brand-lockup">
            <a href="/" class="brand-mark" title="Домой" aria-label="Домой"><svg viewBox="0 0 42 42" fill="none" xmlns="http://www.w3.org/2000/svg" aria-hidden="true"><path d="M16 23V12.5a5 5 0 0 1 10 0V23a8.5 8.5 0 1 1-10 0Z" stroke="#fff" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/><path d="M21 19v12" stroke="#fff" stroke-width="2.5" stroke-linecap="round"/><circle cx="21" cy="31" r="2.8" fill="#fff"/></svg></a>
            <span class="brand-name">ThermoSmart</span>
        </div>
        <div class="brand-copy">
            <h1>Управление климатом</h1>
            <p>Локальная панель управления устройством</p>
        </div>
        <div class="device-state"><span class="status-dot" aria-hidden="true"></span>Устройство готово</div>
    </section>
    <section class="login-panel" aria-labelledby="loginTitle">
        <button class="theme-toggle" id="themeToggle" type="button" onclick="toggleTheme()" aria-label="Включить тёмную тему" title="Включить тёмную тему"><span id="themeIcon" aria-hidden="true">☾</span></button>
        <h2 class="login-heading" id="loginTitle">Вход в систему</h2>
        <p class="login-subtitle">Введите пароль администратора, чтобы продолжить.</p>
        <form class="login-form" id="loginForm" onsubmit="event.preventDefault();doLogin()">
            <label class="field-label" for="password">Пароль</label>
            <input type="password" id="password" class="input-field" placeholder="Введите пароль" autocomplete="current-password" autofocus aria-describedby="errorMsg">
            <button class="login-btn" id="loginBtn" type="submit">Войти</button>
            <div id="errorMsg" class="error-msg" role="status" aria-live="polite"></div>
        </form>
        <div class="info-text">
            <code>admin</code> — пароль по умолчанию
            <span class="hint">Для сброса настроек зажмите кнопку BOOT на 5 секунд.</span>
        </div>
    </section>
</main>
<script>
const errorEl = document.getElementById('errorMsg');
const btn = document.getElementById('loginBtn');
const passInput = document.getElementById('password');
function applyTheme(theme) {
    const dark = theme === 'dark';
    document.body.classList.toggle('theme-dark', dark);
    const button = document.getElementById('themeToggle');
    const label = dark ? 'Включить светлую тему' : 'Включить тёмную тему';
    button.setAttribute('aria-label', label);
    button.title = label;
    document.getElementById('themeIcon').textContent = dark ? '☀' : '☾';
}
function toggleTheme() {
    const dark = !document.body.classList.contains('theme-dark');
    applyTheme(dark ? 'dark' : 'light');
    try { localStorage.setItem('thermosmart_theme', dark ? 'dark' : 'light'); } catch(e) {}
}
let savedTheme = 'light';
try { savedTheme = localStorage.getItem('thermosmart_theme') || 'light'; } catch(e) {}
applyTheme(savedTheme);

async function doLogin() {
    const pass = passInput.value.trim();
    
    if (!pass) {
        errorEl.textContent = '❌ Введите пароль';
        passInput.focus();
        return;
    }
    
    btn.disabled = true;
    btn.textContent = '⏳ Вход...';
    errorEl.textContent = '';
    
    try {
        const resp = await fetch('/auth', {
            method: 'POST',
            headers: {'Content-Type':'application/json'},
            body: JSON.stringify({password: pass})
        });
        const data = await resp.json();
        
        if (data.status === 'ok') {
            try { sessionStorage.setItem('thermosmart_token', pass); } catch(e) {}
            window.location.href = '/';
        } else {
            errorEl.textContent = '❌ Неверный пароль';
            btn.disabled = false;
            btn.textContent = 'Войти';
            passInput.value = '';
            passInput.focus();
        }
    } catch(e) {
        errorEl.textContent = '❌ Ошибка соединения с устройством';
        btn.disabled = false;
        btn.textContent = 'ВОЙТИ';
    }
}
passInput.focus();
</script>
</body>
</html>
)rawliteral";

#endif // THERMOSMART_WEB_LOGIN_PAGE_H
