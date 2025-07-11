#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структуры для типобезопасности (предотвращение перепутывания параметров)
struct PageInfo
{
    const String& title;
    const String& icon;
private:
    PageInfo(const String& title, const String& icon) : title(title), icon(icon) {}
public:
    static PageInfo fromValues(const String& title, const String& icon) {
        return PageInfo(title, icon);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        const String& title = String();
        const String& icon = String();
        Builder& setTitle(const String& titleText) { const_cast<String&>(title) = titleText; return *this; }
        Builder& setIcon(const String& iconText) { const_cast<String&>(icon) = iconText; return *this; }
        PageInfo build() const {
            return PageInfo::fromValues(title, icon);
        }
    };
    static Builder builder() { return {}; }
    static PageInfo createWithTitle(const String& title, const String& icon) {
        return PageInfo::fromValues(title, icon);
    }
    static PageInfo createWithIcon(const String& icon, const String& title) {
        return PageInfo::fromValues(title, icon);
    }
};

struct FormInfo
{
    const String& action;
    const String& method;
    const String& formContent;
    const String& buttonText;
    const String& buttonIcon;
private:
    FormInfo(const String& action, const String& method, const String& formContent, const String& buttonText, const String& buttonIcon)
        : action(action), method(method), formContent(formContent), buttonText(buttonText), buttonIcon(buttonIcon) {}
public:
    static FormInfo fromValues(const String& action, const String& method, const String& formContent, const String& buttonText, const String& buttonIcon) {
        return FormInfo(action, method, formContent, buttonText, buttonIcon);
    }
    static FormInfo createWithAction(const String& action, const String& method, const String& formContent, const String& buttonText, const String& buttonIcon) {
        return FormInfo::fromValues(action, method, formContent, buttonText, buttonIcon);
    }
    static FormInfo createWithMethod(const String& method, const String& action, const String& formContent, const String& buttonText, const String& buttonIcon) {
        return FormInfo::fromValues(action, method, formContent, buttonText, buttonIcon);
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
private:
    InputFieldInfo(const String& id, const String& name, const String& label, const String& value, const String& type, bool required, const String& placeholder)
        : id(id), name(name), label(label), value(value), type(type), required(required), placeholder(placeholder) {}
public:
    static InputFieldInfo fromValues(const String& id, const String& name, const String& label, const String& value, const String& type, bool required, const String& placeholder) {
        return InputFieldInfo(id, name, label, value, type, required, placeholder);
    }
    struct Builder {
        const String& id;
        const String& name;
        const String& label;
        const String& value;
        const String& type;
        const bool required = false;
        const String& placeholder;
        Builder& setId(const String& fieldId) { const_cast<String&>(id) = fieldId; return *this; }
        Builder& setName(const String& fieldName) { const_cast<String&>(name) = fieldName; return *this; }
        Builder& setLabel(const String& labelText) { const_cast<String&>(label) = labelText; return *this; }
        Builder& setValue(const String& valueText) { const_cast<String&>(value) = valueText; return *this; }
        Builder& setType(const String& typeText) { const_cast<String&>(type) = typeText; return *this; }
        Builder& setRequired(bool isRequired) { const_cast<bool&>(required) = isRequired; return *this; }
        Builder& setPlaceholder(const String& placeholderText) { const_cast<String&>(placeholder) = placeholderText; return *this; }
        Builder() = default;
        InputFieldInfo build() const {
            return InputFieldInfo::fromValues(id, name, label, value, type, required, placeholder);
        }
    };
    InputFieldInfo(const Builder& builder)
        : id(builder.id), name(builder.name), label(builder.label), value(builder.value), type(builder.type), required(builder.required), placeholder(builder.placeholder) {}
    static InputFieldInfo createWithId(const String& fieldId, const String& fieldName, const String& labelText, const String& valueText, const String& typeText, bool required, const String& placeholderText) {
        return InputFieldInfo::fromValues(fieldId, fieldName, labelText, valueText, typeText, required, placeholderText);
    }
    static InputFieldInfo createWithName(const String& fieldName, const String& fieldId, const String& labelText, const String& valueText, const String& typeText, bool required, const String& placeholderText) {
        return InputFieldInfo::fromValues(fieldId, fieldName, labelText, valueText, typeText, required, placeholderText);
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
private:
    NumberFieldInfo(const String& id, const String& name, const String& label, int value, int min, int max, int step)
        : id(id), name(name), label(label), value(value), min(min), max(max), step(step) {}
public:
    static NumberFieldInfo fromValues(const String& id, const String& name, const String& label, int value, int min, int max, int step) {
        return NumberFieldInfo(id, name, label, value, min, max, step);
    }
    static NumberFieldInfo createWithId(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum) {
        return NumberFieldInfo::fromValues(fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum);
    }
    static NumberFieldInfo createWithName(const String& fieldName, const String& fieldId, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum) {
        return NumberFieldInfo::fromValues(fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum);
    }
};

struct ConfigSectionInfo
{
    const String& title;
    const String& content;
    const String& helpText;
private:
    ConfigSectionInfo(const String& title, const String& content, const String& helpText)
        : title(title), content(content), helpText(helpText) {}
public:
    static ConfigSectionInfo fromValues(const String& title, const String& content, const String& helpText) {
        return ConfigSectionInfo(title, content, helpText);
    }
    static ConfigSectionInfo createWithTitle(const String& title, const String& content, const String& helpText) {
        return ConfigSectionInfo::fromValues(title, content, helpText);
    }
    static ConfigSectionInfo createWithContent(const String& content, const String& title, const String& helpText) {
        return ConfigSectionInfo::fromValues(title, content, helpText);
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
    return generatePageHeader(PageInfo::builder()
        .setTitle(titleText)
        .setIcon(iconText)
        .build());
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
                        const String& iconText) // NOLINT(misc-use-internal-linkage)
{
    return generateBasePage(PageInfo::builder()
        .setTitle(titleText)
        .setIcon(iconText)
        .build(), contentText);
}

String generateErrorPage(int errorCode, const String& errorMessage)  // NOLINT(misc-use-internal-linkage)
{
    String content = "<h1>" UI_ICON_ERROR " Ошибка " + String(errorCode) + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " " + errorMessage + "</div>";
    content += "<p><a href='/' style='color: #4CAF50; text-decoration: none;'>← Вернуться на главную</a></p>";

    return generateBasePage("Ошибка " + String(errorCode), content, UI_ICON_ERROR);
}

String generateSuccessPage(const String& titleText, const String& messageText, const String& redirectUrlText,
                           int redirectDelaySeconds) // NOLINT(misc-use-internal-linkage)
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
    const String html = "<form action='" + form.action + "' method='" + form.method + "'>" +
                       form.formContent +
                       generateButton(ButtonType::PRIMARY, ButtonConfig{form.buttonIcon.c_str(), form.buttonText.c_str(), ""}) +
                       "</form>";
    return html;
}

// Устаревшая версия для совместимости
String generateForm(const String& actionUrl, const String& methodType, const String& formContentText, const String& buttonTextValue,
                    const String& buttonIconValue) // NOLINT(misc-use-internal-linkage)
{
    return generateForm(FormInfo::createWithAction(actionUrl, methodType, formContentText, buttonTextValue, buttonIconValue));
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
                             const String& helpText) // NOLINT(misc-use-internal-linkage)
{
    return generateConfigSection(ConfigSectionInfo::createWithTitle(title, content, helpText));
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
                          const String& placeholderText) // NOLINT(misc-use-internal-linkage)
{
    return generateInputField(InputFieldInfo::createWithId(fieldId, fieldName, labelText, valueText, typeText, required, placeholderText));
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
                           int stepNum) // NOLINT(misc-use-internal-linkage)
{
    return generateNumberField(NumberFieldInfo::createWithId(fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum));
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
