#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структуры для типобезопасности (предотвращение перепутывания параметров)
struct PageInfo
{
    const String& title;
    const String& icon;

    PageInfo(const String& titleValue, const String& iconValue) : title(titleValue), icon(iconValue) {}
};

struct FormInfo
{
    const String& action;
    const String& method;
    const String& formContent;
    const String& buttonText;
    const String& buttonIcon;

    FormInfo(const String& actionValue, const String& methodValue, const String& formContentValue, const String& buttonTextValue, const String& buttonIconValue)
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
                   const String& typeText, bool isRequired, const String& placeholderText)
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

    NumberFieldInfo(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum)
        : id(fieldId), name(fieldName), label(labelText), value(valueNum), min(minNum), max(maxNum), step(stepNum)
    {
    }
};

String generatePageHeader(const String& titleText, const String& iconText)  // NOLINT(misc-use-internal-linkage)
{
    const String iconStr = iconText.length() > 0 ? iconText + " " : "";
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" + iconStr + titleText + "</title>";
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

String generateBasePage(const String& titleText, const String& contentText,
                        const String& iconText)  // NOLINT(misc-use-internal-linkage)
{
    String html = generatePageHeader(titleText, iconText);
    html += navHtml();
    html += contentText;
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

String generateSuccessPage(const String& titleText, const String& messageText, const String& redirectUrlText,
                           int redirectDelaySeconds)  // NOLINT(misc-use-internal-linkage)
{
    String content = "<h1>" UI_ICON_SUCCESS " " + titleText + "</h1>";
    content += "<div class='msg msg-success'>" UI_ICON_SUCCESS " " + messageText + "</div>";

    if (redirectUrlText.length() > 0)
    {
        content += "<p><em>Перенаправление через " + String(redirectDelaySeconds) + " секунд...</em></p>";
        content += "<script>setTimeout(function(){window.location.href='" + redirectUrlText + "';}, " +
                   String(redirectDelaySeconds * JXCT_REDIRECT_DELAY_MS) + ");</script>";
    }

    return generateBasePage(titleText, content, UI_ICON_SUCCESS);
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
String generateForm(const String& actionUrl, const String& methodType, const String& formContentText, const String& buttonTextValue,
                    const String& buttonIconValue)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<form action='" + actionUrl + "' method='" + methodType + "'>";
    html += formContentText;
    html += generateButton(ButtonType::PRIMARY, ButtonConfig{buttonIconValue.c_str(), buttonTextValue.c_str(), ""});
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
String generateInputField(const String& fieldId, const String& fieldName, const String& labelText, const String& valueText,
                          const String& typeText, bool required,
                          const String& placeholderText)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + fieldId + "'>" + labelText + ":</label>";
    html += "<input type='" + typeText + "' id='" + fieldId + "' name='" + fieldName + "' value='" + valueText + "'";
    if (required)
    {
        html += " required";
    }
    if (placeholderText.length() > 0)
    {
        html += " placeholder='" + placeholderText + "'";
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
String generateCheckboxField(const String& fieldId, const String& fieldName, const String& labelText,
                             bool checked)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + fieldId + "'>" + labelText + ":</label>";
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
String generateNumberField(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum,
                           int stepNum)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + fieldId + "'>" + labelText + ":</label>";
    html += "<input type='number' id='" + fieldId + "' name='" + fieldName + "' value='" + String(valueNum) + "'";
    html += " min='" + String(minNum) + "' max='" + String(maxNum) + "' step='" + String(stepNum) + "'>";
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
String generateApModeUnavailablePage(const String& titleText, const String& iconText)  // NOLINT(misc-use-internal-linkage)
{
    String content = "<h1>" + iconText + " " + titleText + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " Недоступно в режиме точки доступа</div>";
    content += "<p>Эта функция доступна только после подключения к WiFi сети.</p>";

    return generateBasePage(titleText, content, iconText);
}
