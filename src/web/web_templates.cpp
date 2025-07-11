#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структуры для типобезопасности (предотвращение перепутывания параметров)
struct PageInfo
{
    String title;
    String icon;
private:
    PageInfo(const String& title, const String& icon) : title(title), icon(icon) {}
public:
    static PageInfo fromValues(const String& title, const String& icon) {
        return PageInfo(title, icon);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        String title;
        String icon;
        Builder& setTitle(const String& titleText) { title = titleText; return *this; }
        Builder& setIcon(const String& iconText) { icon = iconText; return *this; }
        PageInfo build() const {
            return PageInfo(title, icon);
        }
    };
    static Builder builder() { return {}; }
    static PageInfo createWithTitle(const String& title, const String& icon) {
        return PageInfo(title, icon);
    }
    static PageInfo createWithIcon(const String& icon, const String& title) {
        return PageInfo(title, icon);
    }
};

struct FormInfo
{
    String action;
    String method;
    String formContent;
    String buttonText;
    String buttonIcon;
private:
    FormInfo(const String& action, const String& method, const String& formContent, const String& buttonText, const String& buttonIcon)
        : action(action), method(method), formContent(formContent), buttonText(buttonText), buttonIcon(buttonIcon) {}
public:
    static FormInfo fromValues(const String& action, const String& method, const String& formContent, const String& buttonText, const String& buttonIcon) {
        return FormInfo(action, method, formContent, buttonText, buttonIcon);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        String action;
        String method;
        String formContent;
        String buttonText;
        String buttonIcon;
        Builder& setAction(const String& actionUrl) { action = actionUrl; return *this; }
        Builder& setMethod(const String& methodType) { method = methodType; return *this; }
        Builder& setFormContent(const String& content) { formContent = content; return *this; }
        Builder& setButtonText(const String& text) { buttonText = text; return *this; }
        Builder& setButtonIcon(const String& icon) { buttonIcon = icon; return *this; }
        FormInfo build() const {
            return FormInfo(action, method, formContent, buttonText, buttonIcon);
        }
    };
    static Builder builder() { return {}; }
    static FormInfo createWithAction(const String& action, const String& method, const String& formContent, const String& buttonText, const String& buttonIcon) {
        return FormInfo(action, method, formContent, buttonText, buttonIcon);
    }
    static FormInfo createWithMethod(const String& method, const String& action, const String& formContent, const String& buttonText, const String& buttonIcon) {
        return FormInfo(action, method, formContent, buttonText, buttonIcon);
    }
};

struct InputFieldInfo
{
    String id;
    String name;
    String label;
    String value;
    String type;
    bool required;
    String placeholder;
private:
    InputFieldInfo(const String& id, const String& name, const String& label, const String& value, const String& type, bool required, const String& placeholder)
        : id(id), name(name), label(label), value(value), type(type), required(required), placeholder(placeholder) {}
public:
    static InputFieldInfo fromValues(const String& id, const String& name, const String& label, const String& value, const String& type, bool required, const String& placeholder) {
        return InputFieldInfo(id, name, label, value, type, required, placeholder);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        String id;
        String name;
        String label;
        String value;
        String type;
        bool required = false;
        String placeholder;
        Builder& setId(const String& fieldId) { id = fieldId; return *this; }
        Builder& setName(const String& fieldName) { name = fieldName; return *this; }
        Builder& setLabel(const String& labelText) { label = labelText; return *this; }
        Builder& setValue(const String& valueText) { value = valueText; return *this; }
        Builder& setType(const String& typeText) { type = typeText; return *this; }
        Builder& setRequired(bool isRequired) { required = isRequired; return *this; }
        Builder& setPlaceholder(const String& placeholderText) { placeholder = placeholderText; return *this; }
        Builder() = default;
        InputFieldInfo build() const {
            return InputFieldInfo(id, name, label, value, type, required, placeholder);
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
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        const String& id = String();
        const String& name = String();
        const String& label = String();
        int value = 0;
        int min = 0;
        int max = 100;
        int step = 1;
        Builder& setId(const String& fieldId) { const_cast<String&>(id) = fieldId; return *this; }
        Builder& setName(const String& fieldName) { const_cast<String&>(name) = fieldName; return *this; }
        Builder& setLabel(const String& labelText) { const_cast<String&>(label) = labelText; return *this; }
        Builder& setValue(int valueNum) { value = valueNum; return *this; }
        Builder& setMin(int minNum) { min = minNum; return *this; }
        Builder& setMax(int maxNum) { max = maxNum; return *this; }
        Builder& setStep(int stepNum) { step = stepNum; return *this; }
        NumberFieldInfo build() const {
            return NumberFieldInfo::fromValues(id, name, label, value, min, max, step);
        }
    };
    static Builder builder() { return {}; }
    static NumberFieldInfo createWithId(const String& fieldId, const String& fieldName, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum) {
        return NumberFieldInfo::fromValues(fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum);
    }
    static NumberFieldInfo createWithName(const String& fieldName, const String& fieldId, const String& labelText, int valueNum, int minNum, int maxNum, int stepNum) {
        return NumberFieldInfo::fromValues(fieldId, fieldName, labelText, valueNum, minNum, maxNum, stepNum);
    }
};

struct ConfigSectionInfo
{
    String title;
    String content;
    String helpText;
private:
    ConfigSectionInfo(const String& title, const String& content, const String& helpText)
        : title(title), content(content), helpText(helpText) {}
public:
    static ConfigSectionInfo fromValues(const String& title, const String& content, const String& helpText) {
        return ConfigSectionInfo(title, content, helpText);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        String title;
        String content;
        String helpText;
        Builder& setTitle(const String& titleText) { title = titleText; return *this; }
        Builder& setContent(const String& contentText) { content = contentText; return *this; }
        Builder& setHelpText(const String& helpTextValue) { helpText = helpTextValue; return *this; }
        ConfigSectionInfo build() const {
            return ConfigSectionInfo(title, content, helpText);
        }
    };
    static Builder builder() { return {}; }
    static ConfigSectionInfo createWithTitle(const String& title, const String& content, const String& helpText) {
        return ConfigSectionInfo(title, content, helpText);
    }
    static ConfigSectionInfo createWithContent(const String& content, const String& title, const String& helpText) {
        return ConfigSectionInfo(title, content, helpText);
    }
};

// Все функции и переменные, которые не используются вне этого файла, объявить static
// Например:
// static void myInternalFunction() { ... }
// static int myInternalVar = 0;

namespace {
// Внутренние функции — только для этой единицы трансляции
// (функции, объявленные в заголовочном файле, реализованы вне namespace)
String generatePageHeaderImpl(const PageInfo& page)
{
    const String iconStr = page.icon.length() > 0 ? page.icon + " " : "";
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" + iconStr + page.title + "</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style>";
    html += "</head><body><div class='container'>";
    return html;
}

String generateBasePageImpl(const PageInfo& page, const String& content)
{
    String html = generatePageHeaderImpl(page);
    html += navHtml();
    html += content;
    html += generatePageFooter();
    return html;
}
} // namespace

// Реализации функций, объявленных в web_routes.h, должны быть в глобальном пространстве имён
// Глобальные функции
String generatePageHeader(const String& titleText, const String& iconText)
{
    return generatePageHeaderImpl(PageInfo::builder().setTitle(titleText).setIcon(iconText).build());
}

String generatePageFooter()
{
    return "</div>" + String(getToastHTML()) + "</body></html>";
}

String generateBasePage(const String& titleText, const String& contentText, const String& iconText)
{
    return generateBasePageImpl(PageInfo::builder().setTitle(titleText).setIcon(iconText).build(), contentText);
}

String generateErrorPage(int errorCode, const String& errorMessage)
{
    String content = "<h1>" UI_ICON_ERROR " Ошибка " + String(errorCode) + "</h1>";
    content += "<div class='msg msg-error'>" UI_ICON_ERROR " " + errorMessage + "</div>";
    content += "<p><a href='/' style='color: #4CAF50; text-decoration: none;'>← Вернуться на главную</a></p>";

    return generateBasePage("Ошибка " + String(errorCode), content, UI_ICON_ERROR);
}

String generateSuccessPage(const String& titleText, const String& messageText, const String& redirectUrlText,
                           int redirectDelaySeconds)
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

String generateApModeUnavailablePage(const String& titleText, const String& iconText)
{
    String content = "<h1>" + iconText + " " + titleText + "</h1>";
    content += "<div class='msg msg-warning'>" UI_ICON_WARNING " Эта страница недоступна в режиме точки доступа</div>";
    content += "<p><a href='/' style='color: #4CAF50; text-decoration: none;'>← Вернуться на главную</a></p>";

    return generateBasePage(titleText, content, iconText);
}
