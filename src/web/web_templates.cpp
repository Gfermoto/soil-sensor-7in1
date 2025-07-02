#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// External function declarations
extern String navHtml();

String generatePageHeader(const String& title, const String& icon)
{
    String iconStr = icon.length() > 0 ? icon + " " : "";
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" + iconStr + title + "</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style>";
    html += "</head><body><div class='container'>";
    return html;
}

String generatePageFooter()
{
    return "</div>" + String(getToastHTML()) + "</body></html>";
}

String generateBasePage(const String& title, const String& content, const String& icon)
{
    String html = generatePageHeader(title, icon);
    html += navHtml();
    html += content;
    html += generatePageFooter();
    return html;
}

String generateErrorPage(int errorCode, const String& errorMessage)
{
    String content = "<h1>" UI_ICON_ERROR " Ошибка " + String(errorCode) + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " " + errorMessage + "</div>";
    content += "<p><a href='/' style='color: #4CAF50; text-decoration: none;'>← Вернуться на главную</a></p>";

    return generateBasePage("Ошибка " + String(errorCode), content, UI_ICON_ERROR);
}

String generateSuccessPage(const String& title, const String& message, const String& redirectUrl, int redirectDelay)
{
    String content = "<h1>" UI_ICON_SUCCESS " " + title + "</h1>";
    content += "<div class='msg msg-success'>" UI_ICON_SUCCESS " " + message + "</div>";

    if (redirectUrl.length() > 0)
    {
        content += "<p><em>Перенаправление через " + String(redirectDelay) + " секунд...</em></p>";
        content += "<script>setTimeout(function(){window.location.href='" + redirectUrl + "';}, " +
                   String(redirectDelay * JXCT_REDIRECT_DELAY_MS) + ");</script>";
    }

    return generateBasePage(title, content, UI_ICON_SUCCESS);
}

/**
 * @brief Генерация формы с общими элементами
 * @param action URL для отправки формы
 * @param method HTTP метод (GET/POST)
 * @param formContent Содержимое формы
 * @param buttonText Текст кнопки отправки
 * @param buttonIcon Иконка кнопки
 * @return HTML форма
 */
String generateForm(const String& action, const String& method, const String& formContent, const String& buttonText,
                    const String& buttonIcon)
{
    String html = "<form action='" + action + "' method='" + method + "'>";
    html += formContent;
    html += generateButton(ButtonType::PRIMARY, buttonIcon.c_str(), buttonText.c_str(), "");
    html += "</form>";
    return html;
}

/**
 * @brief Генерация секции конфигурации
 * @param title Заголовок секции
 * @param content Содержимое секции
 * @param helpText Текст подсказки (опционально)
 * @return HTML секция
 */
String generateConfigSection(const String& title, const String& content, const String& helpText)
{
    String html = "<div class='section'>";
    html += "<h2>" + title + "</h2>";
    html += content;
    if (helpText.length() > 0)
    {
        html += "<div class='help'>" UI_ICON_INFO " " + helpText + "</div>";
    }
    html += "</div>";
    return html;
}

/**
 * @brief Генерация поля ввода
 * @param id ID элемента
 * @param name Имя элемента
 * @param label Подпись
 * @param value Значение по умолчанию
 * @param type Тип поля (text, password, email, number)
 * @param required Обязательно ли поле
 * @param placeholder Placeholder текст
 * @return HTML поле ввода
 */
String generateInputField(const String& id, const String& name, const String& label, const String& value,
                          const String& type, bool required, const String& placeholder)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + id + "'>" + label + ":</label>";
    html += "<input type='" + type + "' id='" + id + "' name='" + name + "' value='" + value + "'";
    if (required)
    {
        html += " required";
    }
    if (placeholder.length() > 0)
    {
        html += " placeholder='" + placeholder + "'";
    }
    html += "></div>";
    return html;
}

/**
 * @brief Генерация поля чекбокса
 * @param id ID элемента
 * @param name Имя элемента
 * @param label Подпись
 * @param checked Состояние чекбокса
 * @return HTML чекбокс
 */
String generateCheckboxField(const String& id, const String& name, const String& label, bool checked)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + id + "'>" + label + ":</label>";
    html += "<input type='checkbox' id='" + id + "' name='" + name + "'";
    if (checked)
    {
        html += " checked";
    }
    html += "></div>";
    return html;
}

/**
 * @brief Генерация числового поля с валидацией
 * @param id ID элемента
 * @param name Имя элемента
 * @param label Подпись
 * @param value Значение по умолчанию
 * @param min Минимальное значение
 * @param max Максимальное значение
 * @param step Шаг изменения
 * @return HTML числовое поле
 */
String generateNumberField(const String& id, const String& name, const String& label, int value, int min, int max,
                           int step)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + id + "'>" + label + ":</label>";
    html += "<input type='number' id='" + id + "' name='" + name + "' value='" + String(value) + "'";
    html += " min='" + String(min) + "' max='" + String(max) + "' step='" + String(step) + "'>";
    html += "</div>";
    return html;
}

/**
 * @brief Генерация сообщения об ошибке в форме
 * @param message Текст сообщения
 * @return HTML блок с ошибкой
 */
String generateFormError(const String& message)
{
    return "<div class='msg msg-error'>" UI_ICON_ERROR " " + message + "</div>";
}

/**
 * @brief Генерация страницы "Недоступно в AP режиме"
 * @param title Заголовок страницы
 * @param icon Иконка страницы
 * @return Полная HTML страница
 */
String generateApModeUnavailablePage(const String& title, const String& icon)
{
    String content = "<h1>" + icon + " " + title + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " Недоступно в режиме точки доступа</div>";
    content += "<p>Эта функция доступна только после подключения к WiFi сети.</p>";

    return generateBasePage(title, content, icon);
}
