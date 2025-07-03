# 🌱 JXCT 7-в-1 - Документация

**Профессиональная IoT система мониторинга почвы на базе ESP32**

[![Version](https://img.shields.io/github/v/tag/Gfermoto/soil-sensor-7in1?color=blue&label=version&style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg?style=for-the-badge)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/github/license/Gfermoto/soil-sensor-7in1?color=yellow&label=license&style=for-the-badge)](../LICENSE)
[![CI](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/ci.yml/badge.svg?branch=main&style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/ci.yml)
[![Code Quality](https://img.shields.io/badge/code%20quality-A%2B-brightgreen.svg?style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1)
[![Documentation](https://img.shields.io/badge/docs-100%25%20complete-brightgreen.svg?style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1)
[![Tests](https://img.shields.io/badge/tests-100%25%20passing-brightgreen.svg?style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1)
[![IoT](https://img.shields.io/badge/IoT-Smart%20Agriculture-blue.svg?style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1)

## 🚀 Быстрый старт

**Для пользователей:**

[![📋 Руководство пользователя](https://img.shields.io/badge/📋-Руководство%20пользователя-orange?style=for-the-badge&logo=book)](manuals/USER_GUIDE.md)
[![🌱 Агрономические рекомендации](https://img.shields.io/badge/🌱-Агрономические%20рекомендации-brightgreen?style=for-the-badge&logo=leaf)](manuals/AGRO_RECOMMENDATIONS.md)

**Для инженеров:**

[![🛠️ Техническая документация](https://img.shields.io/badge/🛠️-Техническая%20документация-blue?style=for-the-badge&logo=read-the-docs)](manuals/TECHNICAL_DOCS.md)
[![🔌 Схема подключения](https://img.shields.io/badge/🔌-Схема%20подключения-blueviolet?style=for-the-badge&logo=plug)](manuals/WIRING_DIAGRAM.md)

**Для разработчиков:**

[![👨‍💻 API Overview](https://img.shields.io/badge/👨‍💻-API%20Overview-green?style=for-the-badge&logo=swagger)](api-overview.md)
[![🧑‍💻 Dev Guide](https://img.shields.io/badge/🧑‍💻-Dev%20Guide-lightgrey?style=for-the-badge&logo=github)](dev/)

## 📚 Полная документация

### 👥 Пользовательская документация
| Документ | Описание | Назначение |
|----------|----------|------------|
| [📋 Руководство пользователя](manuals/USER_GUIDE.md) | Основное руководство по использованию | Пошаговая инструкция для конечных пользователей |
| [🔧 Техническая документация](manuals/TECHNICAL_DOCS.md) | Технические детали и спецификации | Подробное описание компонентов и принципов работы |
| [🌱 Агрономические рекомендации](manuals/AGRO_RECOMMENDATIONS.md) | Рекомендации для сельского хозяйства | Научные обоснования и практические советы для агрономов |
| [📊 API документация](manuals/API.md) | Интерфейсы программирования | REST API для интеграции с внешними системами |
| [🔬 Руководство по компенсации](manuals/COMPENSATION_GUIDE.md) | Алгоритмы компенсации измерений | Настройка точности измерений в различных условиях |

### 👨‍💻 Документация разработчика
| Документ | Описание | Назначение |
|----------|----------|------------|
| [🧪 Руководство по тестированию](TESTING_GUIDE.md) | Полная система тестирования | Фреймворк, методология и автоматизация тестирования |
| [📊 Текущие результаты тестирования](CURRENT_TEST_RESULTS.md) | Актуальные метрики качества | Статус тестов, покрытие кода, технический долг |
| [🔄 Интеграция отчётов](REPORTS_INTEGRATION.md) | Веб-интеграция и API | Система отчетности и интеграция с внешними сервисами |
| [📋 Отчёт о завершении Этапа 2](STAGE_2_COMPLETION_REPORT.md) | Итоговый отчёт | Результаты завершенного этапа разработки |
| [🏗️ Архитектура проекта](dev/ARCH_OVERALL.md) | Общая архитектура | Принципы проектирования и структура системы |
| [🔧 План рефакторинга](dev/QA_REFACTORING_PLAN_2025H2.md) | Планы развития | Дорожная карта улучшений на второе полугодие 2025 |
| [⚠️ Отчёт о техническом долге](dev/TECH_DEBT_REPORT_2025-06.md) | Анализ качества кода | Детальный анализ проблем и планы их устранения |

### 📖 Дополнительные ресурсы
| Ресурс | Описание | Назначение |
|--------|----------|------------|
| [📄 Обзор API](api-overview.md) | Краткий обзор API | Быстрое знакомство с возможностями API |
| [🔧 Руководство по контрибьюции](CONTRIBUTING_DOCS.md) | Как участвовать в разработке | Правила и процедуры для участников проекта |
| [🌐 HTML документация](api/html/) | Doxygen документация | Автогенерированная документация кода |
| [📖 Навигация по docs](NAVIGATION.md) | Полная навигация | Структура и организация документации |

## 📊 Текущий статус проекта

### ✅ Завершённые этапы
- **Этап 1**: ✅ Реализация тестов с отчётами
- **Этап 2**: ✅ Интеграция отчётов с сайтом

### 🎯 Текущие метрики качества
| Метрика | Значение | Цель |
|---------|----------|------|
| **Тесты** | 15/15 (100%) | ✅ Включая CSRF тесты |
| **Уязвимости безопасности** | 105 (-22%) | 🟡 Прогресс: CSRF защита |
| **Code Smells** | 75 | 🟡 Средний приоритет |
| **Покрытие кода** | 75.2% (+4.4%) | 🟡 Цель: 90%+ |
| **Рейтинг качества** | C+ (улучшение) | 🟡 Прогресс к A |

### 🚀 Следующие шаги
- **Этап 3**: Продолжение снижения технического долга
- **Приоритет 1**: Устранение оставшихся 105 уязвимостей безопасности
- **Приоритет 2**: Рефакторинг 6 функций высокой сложности
- **Достигнуто**: ✅ CSRF защита, ✅ Unit-тесты калибровки

## 🔧 Основные возможности

### 🌱 Измерения
- **Температура почвы** (-40°C до +80°C)
- **Влажность почвы** (0-100% VWC)
- **Электропроводность** (0-23 mS/cm)
- **pH почвы** (3-9 pH)
- **NPK удобрения** (0-1999 mg/kg)

### 🧠 Алгоритмы компенсации
- **Модель Арчи** - компенсация температуры для EC
- **Уравнение Нернста** - температурная компенсация pH
- **FAO 56** - расчёт ET0 для орошения

### 🌐 IoT интеграция
- **MQTT** - публикация данных
- **ThingSpeak** - облачное хранение
- **Modbus RTU** - промышленный протокол
- **REST API** - веб-интерфейс

### 🔧 Разработка
- **PlatformIO** - среда разработки
- **Docker** - контейнеризация
- **GitHub Actions** - CI/CD
- **Doxygen** - документация кода

## 🤝 Поддержка

- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **Issues:** [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

## 📄 Лицензия

Этот проект распространяется под лицензией **[GNU Affero General Public License v3.6.9 (AGPL-3.0)](../LICENSE)**.

Для коммерческого использования без раскрытия исходников доступна платная лицензия:
- Email: eyera.team@gmail.com
- Telegram: [@Gfermoto](https://t.me/Gfermoto)

---

**EYERA Development Team** | Версия 3.6.7 | Июнь 2025
