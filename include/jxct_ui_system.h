#pragma once
#include <Arduino.h>

// ========================================
// JXCT UI DESIGN SYSTEM v2.3.1
// Единая система дизайна для веб-интерфейса
// ========================================

// 🎨 ЦВЕТОВАЯ ПАЛИТРА
#define UI_COLOR_PRIMARY "#4CAF50"          // Основной зеленый
#define UI_COLOR_PRIMARY_HOVER "#45a049"    // Зеленый при наведении
#define UI_COLOR_SECONDARY "#2196F3"        // Синий для второстепенных кнопок
#define UI_COLOR_SECONDARY_HOVER "#0b7dda"  // Синий при наведении
#define UI_COLOR_DANGER "#F44336"           // Красный для опасных действий
#define UI_COLOR_DANGER_HOVER "#d32f2f"     // Красный при наведении
#define UI_COLOR_WARNING "#FFC107"          // Желтый для предупреждений
#define UI_COLOR_SUCCESS "#4CAF50"          // Зеленый для успеха
#define UI_COLOR_TEXT "#333"                // Основной текст
#define UI_COLOR_TEXT_LIGHT "#666"          // Светлый текст
#define UI_COLOR_BG "#f5f5f5"               // Фон страницы
#define UI_COLOR_BORDER "#ddd"              // Границы элементов

// 📱 РАЗМЕРЫ И ОТСТУПЫ
#define UI_CONTAINER_MAX_WIDTH "1000px"
#define UI_SECTION_PADDING "15px"
#define UI_BUTTON_PADDING "8px 16px"
#define UI_INPUT_PADDING "10px"
#define UI_BORDER_RADIUS "6px"

// 🔤 ТИПОГРАФИКА
#define UI_FONT_FAMILY "Arial, -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif"
#define UI_FONT_SIZE_BASE "16px"
#define UI_FONT_SIZE_SMALL "14px"
#define UI_FONT_SIZE_H1 "22px"
#define UI_FONT_SIZE_H2 "18px"

// 🖱️ АНИМАЦИИ
#define UI_TRANSITION_FAST "0.2s ease"
#define UI_TRANSITION_NORMAL "0.3s ease"

// 📦 ИКОНКИ (Unicode Emoji)
#define UI_ICON_SAVE "💾"
#define UI_ICON_RESET "🔄"
#define UI_ICON_DOWNLOAD "📥"
#define UI_ICON_UPLOAD "📤"
#define UI_ICON_CONFIG "⚙️"
#define UI_ICON_INTERVALS "⏱️"
#define UI_ICON_DATA "📊"
#define UI_ICON_SERVICE "🔧"
#define UI_ICON_WIFI "📶"
#define UI_ICON_MQTT "📡"
#define UI_ICON_SUCCESS "✅"
#define UI_ICON_ERROR "❌"
#define UI_ICON_WARNING "⚠️"
#define UI_ICON_INFO "ℹ️"
#define UI_ICON_LOCK "🔒"
#define UI_ICON_FOLDER "📁"
#define UI_ICON_STATUS "📋"
#define UI_ICON_REFRESH "🔄"

// 📋 ТИПЫ КНОПОК
enum class ButtonType
{
    PRIMARY,    // Основная зеленая кнопка
    SECONDARY,  // Синяя кнопка
    DANGER,     // Красная кнопка
    SUCCESS,    // Зеленая кнопка успеха
    OUTLINE     // Кнопка с контуром
};

// 🎨 ТИПЫ СООБЩЕНИЙ
enum class MessageType
{
    SUCCESS,
    ERROR,
    WARNING,
    INFO
};

// 🎯 ФУНКЦИИ ДЛЯ ГЕНЕРАЦИИ CSS И HTML
const char* getUnifiedCSS();
const char* getToastHTML();
const char* getLoaderHTML();
String generateButton(ButtonType type, const char* icon, const char* text, const char* action = "");