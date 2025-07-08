#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структуры для типобезопасности (предотвращение перепутывания параметров)
struct PageInfo
{
    const String& title;
    const String& icon;

    PageInfo(const String& titleValue, const String& iconValue) : title(titleValue), icon(iconValue) {}  // NOLINT(bugprone-easily-swappable-parameters)
};

struct FormInfo
{
    const String& action;
    const String& method;
    const String& formContent;
    const String& buttonText;
    const String& buttonIcon;

    FormInfo(const String& actionValue, const String& methodValue, const String& formContentValue, const String& buttonTextValue, const String& buttonIconValue)  // NOLINT(bugprone-easily-swappable-parameters)
        : action(actionValue), method(methodValue), formContent(formContentValue), buttonText(buttonTextValue), buttonIcon(buttonIconValue)
    {
    }
};

struct InputFieldInfo
{
    const String& id;
    const String& name;
    const String& label;
    const String& value;
    const String& type;
    const bool required;
    const String& placeholder;

    InputFieldInfo(const String& fieldId, const String& fieldName, const String& labelText, const String& valueText,
                   const String& typeText, bool isRequired, const String& placeholderText)  // NOLINT(bugprone-easily-swappable-parameters)
        : id(fieldId),
          name(fieldName),
          label(labelText),
          value(valueText),
          type(typeText),
          required(isRequired),
          placeholder(placeholderText)
    {
    }
};

struct NumberFieldInfo
{
    String id;
    String name;
    String label;
    int value;
    int min;
    int max;
    int step;

    NumberFieldInfo(const String& fieldId, const String& name, const String& label, int value, int min, int max, int step)  // NOLINT(bugprone-easily-swappable-parameters)
        : id(fieldId), name(name), label(label), value(value), min(min), max(max), step(step)
    {
    }
};

String generatePageHeader(const String& title, const String& icon)  // NOLINT(misc-use-internal-linkage)
{
    const String iconStr = icon.length() > 0 ? icon + " " : "";
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" + iconStr + title + "</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style>";
    html += "</head><body><div class='container'>";
    return html;
}

// ✅ Типобезопасная версия
String generatePageHeader(const PageInfo& page)  // NOLINT(misc-use-internal-linkage)
{
    return generatePageHeader(page.title, page.icon);
}

String generatePageFooter()  // NOLINT(misc-use-internal-linkage)
{
    return "</div>" + String(getToastHTML()) + "</body></html>";
}

String generateBasePage(const String& title, const String& content,
                        const String& icon)  // NOLINT(misc-use-internal-linkage)
{
    String html = generatePageHeader(title, icon);
    html += navHtml();
    html += content;
    html += generatePageFooter();
    return html;
}

// ✅ Типобезопасная версия
String generateBasePage(const PageInfo& page, const String& content)  // NOLINT(misc-use-internal-linkage)
{
    return generateBasePage(page.title, content, page.icon);
}

String generateErrorPage(int errorCode, const String& errorMessage)  // NOLINT(misc-use-internal-linkage)
{
    String content = "<h1>" UI_ICON_ERROR " Ошибка " + String(errorCode) + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " " + errorMessage + "</div>";
    content += "<p><a href='/' style='color: #4CAF50; text-decoration: none;'>← Вернуться на главную</a></p>";

    return generateBasePage("Ошибка " + String(errorCode), content, UI_ICON_ERROR);
}

String generateSuccessPage(const String& title, const String& message, const String& redirectUrl,
                           int redirectDelay)  // NOLINT(misc-use-internal-linkage,bugprone-easily-swappable-parameters)
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
                    const String& buttonIcon)  // NOLINT(misc-use-internal-linkage,bugprone-easily-swappable-parameters)
{
    String html = "<form action='" + action + "' method='" + method + "'>";
    html += formContent;
    html += generateButton(ButtonType::PRIMARY, ButtonConfig{buttonIcon.c_str(), buttonText.c_str(), ""});
    html += "</form>";
    return html;
}

// ✅ Типобезопасная версия (предотвращает перепутывание 5 String параметров)
String generateForm(const FormInfo& form)  // NOLINT(misc-use-internal-linkage)
{
    return generateForm(form.action, form.method, form.formContent, form.buttonText, form.buttonIcon);
}

/**
 * @brief Генерация секции конфигурации
 * @param title Заголовок секции
 * @param content Содержимое секции
 * @param helpText Текст подсказки (опционально)
 * @return HTML секция
 */
String generateConfigSection(const String& title, const String& content,
                             const String& helpText)  // NOLINT(misc-use-internal-linkage)
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
 * @param fieldId ID элемента
 * @param fieldName Имя элемента
 * @param label Подпись
 * @param value Значение по умолчанию
 * @param type Тип поля (text, password, email, number)
 * @param required Обязательно ли поле
 * @param placeholder Placeholder текст
 * @return HTML поле ввода
 */
String generateInputField(const String& fieldId, const String& fieldName, const String& label, const String& value,
                          const String& type, bool required,
                          const String& placeholder)  // NOLINT(misc-use-internal-linkage,bugprone-easily-swappable-parameters)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + fieldId + "'>" + label + ":</label>";
    html += "<input type='" + type + "' id='" + fieldId + "' name='" + fieldName + "' value='" + value + "'";
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

// ✅ Типобезопасная версия (предотвращает перепутывание 6 String параметров)
String generateInputField(const InputFieldInfo& field)  // NOLINT(misc-use-internal-linkage)
{
    return generateInputField(field.id, field.name, field.label, field.value, field.type, field.required,
                              field.placeholder);
}

/**
 * @brief Генерация поля чекбокса
 * @param fieldId ID элемента
 * @param fieldName Имя элемента
 * @param label Подпись
 * @param checked Состояние чекбокса
 * @return HTML чекбокс
 */
String generateCheckboxField(const String& fieldId, const String& fieldName, const String& label,
                             bool checked)  // NOLINT(misc-use-internal-linkage,bugprone-easily-swappable-parameters)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + fieldId + "'>" + label + ":</label>";
    html += "<input type='checkbox' id='" + fieldId + "' name='" + fieldName + "'";
    if (checked)
    {
        html += " checked";
    }
    html += "></div>";
    return html;
}

/**
 * @brief Генерация числового поля с валидацией
 * @param fieldId ID элемента
 * @param fieldName Имя элемента
 * @param label Подпись
 * @param value Значение по умолчанию
 * @param min Минимальное значение
 * @param max Максимальное значение
 * @param step Шаг изменения
 * @return HTML числовое поле
 */
String generateNumberField(const String& fieldId, const String& fieldName, const String& label, int value, int min, int max,
                           int step)  // NOLINT(misc-use-internal-linkage,bugprone-easily-swappable-parameters)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + fieldId + "'>" + label + ":</label>";
    html += "<input type='number' id='" + fieldId + "' name='" + fieldName + "' value='" + String(value) + "'";
    html += " min='" + String(min) + "' max='" + String(max) + "' step='" + String(step) + "'>";
    html += "</div>";
    return html;
}

// ✅ Типобезопасная версия (предотвращает перепутывание 4 int параметров)
String generateNumberField(const NumberFieldInfo& field)  // NOLINT(misc-use-internal-linkage)
{
    return generateNumberField(field.id, field.name, field.label, field.value, field.min, field.max, field.step);
}

/**
 * @brief Генерация сообщения об ошибке в форме
 * @param message Текст сообщения
 * @return HTML блок с ошибкой
 */
String generateFormError(const String& message)  // NOLINT(misc-use-internal-linkage)
{
    return "<div class='msg msg-error'>" UI_ICON_ERROR " " + message + "</div>";
}

/**
 * @brief Генерация страницы "Недоступно в AP режиме"
 * @param title Заголовок страницы
 * @param icon Иконка страницы
 * @return Полная HTML страница
 */
String generateApModeUnavailablePage(const String& title, const String& icon)  // NOLINT(misc-use-internal-linkage)
{
    String content = "<h1>" + icon + " " + title + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " Недоступно в режиме точки доступа</div>";
    content += "<p>Эта функция доступна только после подключения к WiFi сети.</p>";

    return generateBasePage(title, content, icon);
}
