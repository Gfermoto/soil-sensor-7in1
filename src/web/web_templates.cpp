#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структуры для типобезопасности (предотвращение перепутывания параметров)
struct PageInfo
{
    const String& title;
    const String& icon;

    PageInfo(const String& titleValue, const String& iconValue) // NOLINT(bugprone-easily-swappable-parameters)
        : title(titleValue), icon(iconValue) {}
    
    // Статический метод-фабрика для безопасного создания
    static PageInfo create(const String& titleValue, const String& iconValue) {
        return {titleValue, iconValue};
    }
};

struct FormInfo
{
    const String& action;
    const String& method;
    const String& formContent;
    const String& buttonText;
    const String& buttonIcon;

    FormInfo(const String& actionValue, const String& methodValue, const String& formContentValue, const String& buttonTextValue, const String& buttonIconValue) // NOLINT(bugprone-easily-swappable-parameters)
        : action(actionValue), method(methodValue), formContent(formContentValue), buttonText(buttonTextValue), buttonIcon(buttonIconValue)
    {
    }
    
    // Статический метод-фабрика для безопасного создания
    static FormInfo create(const String& actionValue, const String& methodValue, const String& formContentValue, const String& buttonTextValue, const String& buttonIconValue) {
        return {actionValue, methodValue, formContentValue, buttonTextValue, buttonIconValue};
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

    // Конструктор с именованными параметрами для безопасности
    struct Builder {
        const String& id;
        const String& name;
        const String& label;
        const String& value;
        const String& type;
        const bool required = false;
        const String& placeholder;

        // Используем именованные параметры для безопасности
        Builder& setId(const String& fieldId) { const_cast<String&>(id) = fieldId; return *this; }
        Builder& setName(const String& fieldName) { const_cast<String&>(name) = fieldName; return *this; }
        Builder& setLabel(const String& labelText) { const_cast<String&>(label) = labelText; return *this; }
        Builder& setValue(const String& valueText) { const_cast<String&>(value) = valueText; return *this; }
        Builder& setType(const String& typeText) { const_cast<String&>(type) = typeText; return *this; }
        Builder& setRequired(bool isRequired) { const_cast<bool&>(required) = isRequired; return *this; }
        Builder& setPlaceholder(const String& placeholderText) { const_cast<String&>(placeholder) = placeholderText; return *this; }

        // Конструктор по умолчанию
        Builder() : id(String()), name(String()), label(String()), value(String()), 
                    type(String()), placeholder(String()) {}
    };

    InputFieldInfo(const Builder& builder)
        : id(builder.id),
          name(builder.name),
          label(builder.label),
          value(builder.value),
          type(builder.type),
          required(builder.required),
          placeholder(builder.placeholder)
    {
    }
    
    // Статический метод-фабрика для безопасного создания
    static InputFieldInfo create(const String& fieldId, const String& fieldName, const String& labelText, const String& valueText,
                                const String& typeText, bool isRequired, const String& placeholderText) {
        Builder builder;
        builder.setId(fieldId).setName(fieldName).setLabel(labelText).setValue(valueText)
               .setType(typeText).setRequired(isRequired).setPlaceholder(placeholderText);
        return InputFieldInfo(builder); // NOLINT(modernize-return-braced-init-list)
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

    NumberFieldInfo(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum) // NOLINT(bugprone-easily-swappable-parameters)
        : id(fieldId), name(fieldName), label(labelText), value(valueNum), min(minNum), max(maxNum), step(stepNum)
    {
    }
    
    // Статический метод-фабрика для безопасного создания
    static NumberFieldInfo create(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum) {
        return {fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum}; // NOLINT(modernize-return-braced-init-list)
    }
};

struct ConfigSectionInfo
{
    const String& title;
    const String& content;
    const String& helpText;

    ConfigSectionInfo(const String& titleText, const String& contentText, const String& helpTextValue) // NOLINT(bugprone-easily-swappable-parameters)
        : title(titleText), content(contentText), helpText(helpTextValue)
    {
    }
    
    // Статический метод-фабрика для безопасного создания
    static ConfigSectionInfo create(const String& titleText, const String& contentText, const String& helpTextValue) {
        return {titleText, contentText, helpTextValue};
    }
};

String generatePageHeader(const PageInfo& page)  // NOLINT(misc-use-internal-linkage)
{
    const String iconStr = page.icon.length() > 0 ? page.icon + " " : "";
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" + iconStr + page.title + "</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style>";
    html += "</head><body><div class='container'>";
    return html;
}

// Устаревшая версия для совместимости
String generatePageHeader(const String& titleText, const String& iconText)  // NOLINT(misc-use-internal-linkage)
{
    return generatePageHeader(PageInfo::create(titleText, iconText));
}

String generatePageFooter()  // NOLINT(misc-use-internal-linkage)
{
    return "</div>" + String(getToastHTML()) + "</body></html>";
}

String generateBasePage(const PageInfo& page, const String& content)  // NOLINT(misc-use-internal-linkage)
{
    String html = generatePageHeader(page);
    html += navHtml();
    html += content;
    html += generatePageFooter();
    return html;
}

// Устаревшая версия для совместимости
String generateBasePage(const String& titleText, const String& contentText,
                        const String& iconText)  // NOLINT(misc-use-internal-linkage)
{
    return generateBasePage(PageInfo::create(titleText, iconText), contentText);
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
String generateForm(const FormInfo& form)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<form action='" + form.action + "' method='" + form.method + "'>";
    html += form.formContent;
    html += generateButton(ButtonType::PRIMARY, ButtonConfig{form.buttonIcon.c_str(), form.buttonText.c_str(), ""});
    html += "</form>";
    return html;
}

// Устаревшая версия для совместимости
String generateForm(const String& actionUrl, const String& methodType, const String& formContentText, const String& buttonTextValue,
                    const String& buttonIconValue)  // NOLINT(misc-use-internal-linkage)
{
    return generateForm(FormInfo::create(actionUrl, methodType, formContentText, buttonTextValue, buttonIconValue));
}

/**
 * @brief Генерация секции конфигурации
 * @param section Информация о секции
 * @return HTML секция
 */
String generateConfigSection(const ConfigSectionInfo& section)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='section'>";
    html += "<h2>" + section.title + "</h2>";
    html += section.content;
    if (section.helpText.length() > 0)
    {
        html += "<div class='help'>" UI_ICON_INFO " " + section.helpText + "</div>";
    }
    html += "</div>";
    return html;
}

// Устаревшая версия для совместимости
String generateConfigSection(const String& title, const String& content,
                             const String& helpText)  // NOLINT(misc-use-internal-linkage)
{
    return generateConfigSection(ConfigSectionInfo::create(title, content, helpText));
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
String generateInputField(const InputFieldInfo& field)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + field.id + "'>" + field.label + ":</label>";
    html += "<input type='" + field.type + "' id='" + field.id + "' name='" + field.name + "' value='" + field.value + "'";
    if (field.required)
    {
        html += " required";
    }
    if (field.placeholder.length() > 0)
    {
        html += " placeholder='" + field.placeholder + "'";
    }
    html += "></div>";
    return html;
}

// Устаревшая версия для совместимости
String generateInputField(const String& fieldId, const String& fieldName, const String& labelText, const String& valueText,
                          const String& typeText, bool required,
                          const String& placeholderText)  // NOLINT(misc-use-internal-linkage)
{
    return generateInputField(InputFieldInfo::create(fieldId, fieldName, labelText, valueText, typeText, required, placeholderText));
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
String generateNumberField(const NumberFieldInfo& field)  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='form-group'>";
    html += "<label for='" + field.id + "'>" + field.label + ":</label>";
    html += "<input type='number' id='" + field.id + "' name='" + field.name + "' value='" + String(field.value) + "'";
    html += " min='" + String(field.min) + "' max='" + String(field.max) + "' step='" + String(field.step) + "'></div>";
    return html;
}

// Устаревшая версия для совместимости
String generateNumberField(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum,
                           int stepNum)  // NOLINT(misc-use-internal-linkage)
{
    return generateNumberField(NumberFieldInfo::create(fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum));
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
