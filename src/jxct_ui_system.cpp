#include "jxct_ui_system.h"

// 🎨 ЕДИНЫЙ CSS ДЛЯ ВСЕХ СТРАНИЦ
const char* getUnifiedCSS()
{
    static const char css[] = R"(
/* === JXCT UI DESIGN SYSTEM v2.3.1 === */
* { box-sizing: border-box; }

body {
    font-family: Arial, -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
    margin: 0;
    padding: 20px;
    background: #f5f5f5;
    color: #333;
    font-size: 16px;
    line-height: 1.5;
}

.container {
    max-width: 1000px;
    margin: 0 auto;
    background: white;
    border-radius: 6px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    padding: 30px;
}

/* === ТИПОГРАФИКА === */
h1 {
    color: #333;
    font-size: 22px;
    margin: 0 0 20px 0;
    font-weight: 600;
}

h2 {
    color: #333;
    font-size: 18px;
    margin: 20px 0 12px 0;
    font-weight: 500;
    border-bottom: 2px solid #4CAF50;
    padding-bottom: 6px;
}

/* === НАВИГАЦИЯ === */
.nav {
    margin-bottom: 30px;
    padding: 15px 0;
    border-bottom: 1px solid #ddd;
}

.nav a {
    display: inline-block;
    margin-right: 10px;
    text-decoration: none;
    color: #4CAF50;
    font-weight: 600;
    padding: 8px 12px;
    border-radius: 6px;
    transition: 0.2s ease;
}

.nav a:hover {
    background: #4CAF50;
    color: white;
    transform: translateY(-1px);
}

/* === СЕКЦИИ === */
.section {
    margin-bottom: 25px;
    padding: 15px;
    border: 1px solid #ddd;
    border-radius: 6px;
    background: #fafafa;
}

/* === ФОРМЫ === */
.form-group {
    margin-bottom: 20px;
}

label {
    display: block;
    margin-bottom: 6px;
    font-weight: 600;
    color: #333;
}

input[type=text], input[type=password], input[type=number], input[type=email], input[type=file], select, textarea {
    width: 100%;
    padding: 10px;
    border: 2px solid #ddd;
    border-radius: 6px;
    font-size: 16px;
    transition: 0.2s ease;
}

input:focus, select:focus, textarea:focus {
    outline: none;
    border-color: #4CAF50;
    box-shadow: 0 0 0 3px rgba(76, 175, 80, 0.1);
}

/* === КНОПКИ (ЕДИНАЯ СИСТЕМА) === */
.btn {
    display: inline-block;
    padding: 8px 16px;
    border: none;
    border-radius: 6px;
    font-size: 14px;
    font-weight: 500;
    cursor: pointer;
    text-decoration: none;
    transition: 0.3s ease;
    margin-right: 10px;
    margin-bottom: 10px;
    min-width: 120px;
    text-align: center;
}

.btn:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 12px rgba(0,0,0,0.15);
}

.btn-primary {
    background: #4CAF50;
    color: white;
}

.btn-primary:hover {
    background: #45a049;
}

.btn-secondary {
    background: #2196F3;
    color: white;
}

.btn-secondary:hover {
    background: #0b7dda;
}

.btn-danger {
    background: #F44336;
    color: white;
}

.btn-danger:hover {
    background: #d32f2f;
}

.btn-outline {
    background: transparent;
    color: #4CAF50;
    border: 2px solid #4CAF50;
}

.btn-outline:hover {
    background: #4CAF50;
    color: white;
}

/* === СООБЩЕНИЯ === */
.msg {
    padding: 15px 20px;
    margin-bottom: 20px;
    border-radius: 6px;
    font-weight: 500;
    border-left: 4px solid;
}

.msg-success {
    background: #e8f5e8;
    color: #2e7d32;
    border-left-color: #4CAF50;
}

.msg-error {
    background: #ffebee;
    color: #c62828;
    border-left-color: #F44336;
}

.msg-warning {
    background: #fff8e1;
    color: #f57c00;
    border-left-color: #FFC107;
}

.msg-info {
    background: #e3f2fd;
    color: #1565c0;
    border-left-color: #2196F3;
}

/* === ВСПОМОГАТЕЛЬНЫЕ ЭЛЕМЕНТЫ === */
.help {
    color: #666;
    font-size: 14px;
    margin-top: 5px;
    font-style: italic;
}

.status-dot {
    display: inline-block;
    width: 12px;
    height: 12px;
    border-radius: 50%;
    margin-right: 8px;
    vertical-align: middle;
}

.dot-ok { background: #4CAF50; }
.dot-warn { background: #FFC107; }
.dot-err { background: #F44336; }
.dot-off { background: #bbb; }

/* === ЛОАДЕР === */
.loader {
    border: 3px solid #f3f3f3;
    border-top: 3px solid #4CAF50;
    border-radius: 50%;
    width: 20px;
    height: 20px;
    animation: spin 1s linear infinite;
    display: inline-block;
    margin-right: 10px;
}

@keyframes spin {
    0% { transform: rotate(0deg); }
    100% { transform: rotate(360deg); }
}

/* === TOAST УВЕДОМЛЕНИЯ === */
.toast {
    position: fixed;
    top: 20px;
    right: 20px;
    padding: 15px 25px;
    border-radius: 6px;
    color: white;
    font-weight: 600;
    z-index: 9999;
    opacity: 0;
    transform: translateX(100%);
    transition: all 0.3s ease;
}

.toast.show {
    opacity: 1;
    transform: translateX(0);
}



/* === МОБИЛЬНАЯ АДАПТАЦИЯ === */
@media (max-width: 768px) {
    body { padding: 10px; }
    .container { padding: 20px; margin: 5px; }
    h1 { font-size: 20px; }
    h2 { font-size: 16px; }
    .nav a {
        display: block;
        margin: 5px 0;
        text-align: center;
    }
    .btn {
        width: 100%;
        margin-right: 0;
        margin-bottom: 15px;
    }
    .section { padding: 12px; }
    .form-group { margin-bottom: 15px; }


}
    )";
    return css;
}

// 🎯 ГЕНЕРАЦИЯ HTML КНОПОК
String generateButton(ButtonType type, const ButtonConfig& config)
{
    String cssClass = "btn ";

    switch (type)
    {
        case ButtonType::PRIMARY:
            cssClass += "btn-primary";
            break;
        case ButtonType::SECONDARY:
            cssClass += "btn-secondary";
            break;
        case ButtonType::DANGER:
            cssClass += "btn-danger";
            break;
        case ButtonType::SUCCESS:
            cssClass += "btn-primary";
            break;
        case ButtonType::OUTLINE:
            cssClass += "btn-outline";
            break;
    }

    String html = "<button type='submit' class='" + cssClass + "'";
    if (strlen(config.action) > 0)
    {
        html = "<button type='button' class='" + cssClass + "' onclick=\"" + String(config.action) + "\"";
    }
    html += ">" + String(config.icon) + " " + String(config.text) + "</button>";

    return html;
}

// 🍞 TOAST УВЕДОМЛЕНИЯ
const char* getToastHTML()
{
    return R"(
<script>
function showToast(message, type) {
    const toast = document.createElement('div');
    toast.className = 'toast';
    toast.textContent = message;

    const colors = {
        'success': '#4CAF50',
        'error': '#F44336',
        'warning': '#FFC107',
        'info': '#2196F3'
    };

    toast.style.background = colors[type] || colors['info'];
    document.body.appendChild(toast);

    setTimeout(() => toast.classList.add('show'), 100);
    setTimeout(() => {
        toast.classList.remove('show');
        setTimeout(() => document.body.removeChild(toast), 300);
    }, 3000);
}

// Показать toast при загрузке если есть сообщение
window.addEventListener('load', function() {
    const urlParams = new URLSearchParams(window.location.search);
    const msg = urlParams.get('msg');
    const type = urlParams.get('type') || 'info';
    if (msg) {
        showToast(decodeURIComponent(msg), type);
    }
});
</script>
    )";
}

// ⌛ ЛОАДЕР
const char* getLoaderHTML()
{
    return "<div class='loader'></div>";
}
