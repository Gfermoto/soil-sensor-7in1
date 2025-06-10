/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "JXCT Soil Sensor v2.0", "index.html", [
    [ "JXCT Soil Sensor v2.0 (ESP32, PlatformIO)", "index.html", "index" ],
    [ "QA ОТЧЁТ: JXCT Soil Sensor v2.0 (ESP32/PlatformIO)", "md_docs_2_q_a___r_e_p_o_r_t__2025.html", [
      [ "📋 КРАТКОЕ РЕЗЮМЕ", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md35", null ],
      [ "🔒 БЕЗОПАСНОСТЬ [КРИТИЧНО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md37", [
        [ "1.1 Веб-интерфейс", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md38", null ],
        [ "1.2 Хранение данных", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md39", null ],
        [ "1.3 Сетевая безопасность", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md40", null ]
      ] ],
      [ "🏗️ АРХИТЕКТУРА И КОД [ХОРОШО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md41", [
        [ "2.1 Структура проекта", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md42", null ],
        [ "2.2 Качество кода", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md43", null ],
        [ "2.3 Memory Management", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md44", null ]
      ] ],
      [ "🌐 СЕТЕВЫЕ ФУНКЦИИ [ХОРОШО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md45", [
        [ "3.1 WiFi Management", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md46", null ],
        [ "3.2 MQTT Integration", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md47", null ],
        [ "3.3 ThingSpeak Integration", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md48", null ]
      ] ],
      [ "📊 ДАННЫЕ И ДАТЧИКИ [ОТЛИЧНО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md49", [
        [ "4.1 Modbus Communication", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md50", null ],
        [ "4.2 Data Validation", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md51", null ],
        [ "4.3 Data Formatting", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md52", null ]
      ] ],
      [ "🎨 ПОЛЬЗОВАТЕЛЬСКИЙ ИНТЕРФЕЙС [ХОРОШО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md53", [
        [ "5.1 Веб-интерфейс", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md54", null ],
        [ "5.2 API", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md55", null ]
      ] ],
      [ "⚡ ПРОИЗВОДИТЕЛЬНОСТЬ [ХОРОШО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md56", [
        [ "6.1 Memory Usage", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md57", null ],
        [ "6.2 CPU Performance", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md58", null ]
      ] ],
      [ "🔧 НАСТРОЙКА И КОНФИГУРАЦИЯ [ОТЛИЧНО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md59", [
        [ "7.1 Configuration Management", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md60", null ],
        [ "7.2 Build Configuration", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md61", null ]
      ] ],
      [ "📚 ДОКУМЕНТАЦИЯ [ХОРОШО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md62", [
        [ "8.1 Code Documentation", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md63", null ],
        [ "8.2 API Documentation", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md64", null ]
      ] ],
      [ "🧪 ТЕСТИРОВАНИЕ [ПЛОХО]", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md65", null ],
      [ "🎯 ИТОГОВАЯ ОЦЕНКА", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md67", null ],
      [ "📈 РЕКОМЕНДАЦИИ ПО УЛУЧШЕНИЮ", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md69", [
        [ "🔥 КРИТИЧНО (требует немедленного внимания)", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md70", null ],
        [ "⚡ ВЫСОКИЙ ПРИОРИТЕТ", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md71", null ],
        [ "📌 СРЕДНИЙ ПРИОРИТЕТ", "md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md72", null ]
      ] ]
    ] ],
    [ "🚀 ПЛАН РАЗВИТИЯ JXCT Soil Sensor v2.x", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html", [
      [ "🎯 ВИДЕНИЕ И ЦЕЛИ", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md77", null ],
      [ "📅 ROADMAP ПО ВЕРСИЯМ", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md79", [
        [ "🔥 v2.1 - SECURITY FIRST (Июль 2025)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md80", [
          [ "Критические исправления безопасности", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md81", null ]
        ] ],
        [ "🛡️ v2.2 - ENTERPRISE SECURITY (Август 2025)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md82", [
          [ "HTTPS & Certificates", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md83", null ]
        ] ],
        [ "🧪 v2.3 - TESTING FOUNDATION (Сентябрь 2025)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md84", [
          [ "Unit Testing Framework", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md85", null ]
        ] ],
        [ "📊 v2.4 - MONITORING & OBSERVABILITY (Октябрь 2025)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md86", [
          [ "Advanced Logging", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md87", null ]
        ] ],
        [ "🚀 v2.5 - ADVANCED FEATURES (Ноябрь 2025)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md88", [
          [ "OTA & Management", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md89", null ]
        ] ],
        [ "🌟 v3.0 - NEXT GENERATION (Q1 2026)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md90", [
          [ "Architecture Refactoring", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md91", null ]
        ] ]
      ] ],
      [ "📋 ДЕТАЛЬНЫЙ ПЛАН ЗАДАЧ", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md93", [
        [ "🏃‍♂️ SPRINT 1: Authentication System (v2.1)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md94", [
          [ "Week 1: Basic Authentication", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md95", null ],
          [ "Week 2: Access Control & Security Hardening", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md96", null ]
        ] ],
        [ "🛡️ SPRINT 2: Password Encryption (v2.1)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md97", [
          [ "Implementation Details:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md98", null ]
        ] ],
        [ "🔐 SPRINT 3: HTTPS Implementation (v2.2)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md99", [
          [ "Technical Approach:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md100", null ]
        ] ]
      ] ],
      [ "🛠️ ТЕХНИЧЕСКИЕ ТРЕБОВАНИЯ", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md102", [
        [ "Минимальные системные требования для v2.5:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md103", null ],
        [ "Новые зависимости:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md104", null ]
      ] ],
      [ "📊 МЕТРИКИ УСПЕХА", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md106", [
        [ "Безопасность:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md107", null ],
        [ "Качество:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md108", null ],
        [ "Функциональность:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md109", null ]
      ] ],
      [ "🤝 РЕСУРСЫ И КОМАНДА", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md111", [
        [ "Роли:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md112", null ],
        [ "Внешние ресурсы:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md113", null ]
      ] ],
      [ "🚨 РИСКИ И МИТИГАЦИЯ", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md115", [
        [ "Технические риски:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md116", null ],
        [ "Бизнес риски:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md117", null ]
      ] ],
      [ "📈 LONG-TERM VISION (2026+)", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md119", [
        [ "v4.0 - IoT Platform:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md120", null ],
        [ "Market positioning:", "md_docs_2_d_e_v_e_l_o_p_m_e_n_t___r_o_a_d_m_a_p__2025.html#autotoc_md121", null ]
      ] ]
    ] ],
    [ "Классы", "annotated.html", [
      [ "Классы", "annotated.html", "annotated_dup" ],
      [ "Алфавитный указатель классов", "classes.html", null ],
      [ "Члены классов", "functions.html", [
        [ "Указатель", "functions.html", null ],
        [ "Переменные", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Файлы", "files.html", [
      [ "Файлы", "files.html", "files_dup" ],
      [ "Список членов всех файлов", "globals.html", [
        [ "Указатель", "globals.html", "globals_dup" ],
        [ "Функции", "globals_func.html", null ],
        [ "Переменные", "globals_vars.html", null ],
        [ "Перечисления", "globals_enum.html", null ],
        [ "Элементы перечислений", "globals_eval.html", null ],
        [ "Макросы", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"md_docs_2_q_a___r_e_p_o_r_t__2025.html#autotoc_md44"
];

var SYNCONMSG = 'нажмите на выключить для синхронизации панелей';
var SYNCOFFMSG = 'нажмите на включить для синхронизации панелей';