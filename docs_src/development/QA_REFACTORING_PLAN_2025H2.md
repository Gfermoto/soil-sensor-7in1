---
title: QA & Refactoring Plan H2-2025
---

# 🧪 План QA-аудита и рефакторинга (JXCT H2-2025)

> Версия 1.0   |   Дата: 2025-06-21   |   Ответственные: **QA-Lead**, **Lead Dev**, **Arch**

---

## 1. Введение
Предыдущие дорожные карты устарели после внедрения OTA 2.0, модульного UI и
универсальной CSV-калибровки. Настоящий план фиксирует новый вектор развития: фокус на
удаление «code smells» и подготовку базы к отделению статики UI
(`spiffs_web`) от ядра прошивки.

---

## 2. Цели (H2-2025)
| # | Цель | KPI / метрика |
|---|------|---------------|
| 1 | Уменьшить технический долг | **Tech-Debt SLO ≤ 45 дней**, <br/>Sonar rating ≥ A |
| 2 | Повысить покрытие тестами | **Unit coverage ≥ 85 %**, <br/>**Integration ≥ 60 %** |
| 3 | Исключить циклические зависимости | **0** циклов в include-графе (include-what-you-use) |
| 4 | Снизить дублирование кода | Duplication < 3 % (cloned lines) |
| 5 | Повысить стабильность CI | Build-success = 100 % <br/>(main + PR) |

---

## 3. Методика QA-аудита
1. **Инструменты статического анализа**  
   * clang-tidy (modernize-*, performance-*, readability-*)  
   * Cppcheck (MISRA subset)  
   * Sonar C++ (community edition)  
2. **Поиск дублирования** — `cpp-dependencies`, `cloc --by-file --duplicates`.  
3. **Граф зависимостей** — `include-what-you-use` + `Graphviz`.  
4. **Сложность** — Lizard: CCN ≥ 15 → hot-spot.  
5. **Runtime-аудит** — AddressSanitizer + LeakSanitizer (native build).

Результаты фиксируются в отчёте `tech_debt_report_2025-07.md` (шаблон — см. `docs/dev/TECH_DEBT_REPORT_2025-06.md`).

---

## 4. Этапы и сроки
| Неделя | Deliverable | Owner |
|--------|-------------|-------|
| 26 (24-28 июн) | Setup clang-tidy + IWYU в CI <br/>Снимок базовых метрик | DevOps |
| 27 (01-05 июл) | Полный статический отчёт + список критичных smells | QA |
| 28-29 | Refactor Sprint-1: **low-hanging** (duplication, naming) | DevTeam |
| 30 (22-26 июл) | Регресс-тесты + code-coverage отчёт | QA |
| 31-33 | Refactor Sprint-2: **architecture** (циклы, включает routing split) | DevTeam |
| 34 (19-23 авг) | Beta v2.8.0 (core) + web-split PoC | FW-Lead / UI-Lead |

---

## 5. Критические эпики рефакторинга
| Epic ID | Описание | DoD |
|---------|----------|-----|
| **RF-1** | **CI Hardening 2.0** — clang-tidy «error» уровень, блокирующий merge | main зелёный 30 дней подряд |
| **RF-2** | **Dependency Flattening** — удалить все include-циклы, внедрить `forward decl` | IWYU отчёт = 0 циклов |
| **RF-3** | **Sensor Abstraction v2** — финализировать `ISensor` + прослойку Adapter | Реальные и Fake датчики используют общий API |
| **RF-4** | **RecommendationEngine extraction** — вынести бизнес-логику из `routes_*` в модуль | ≥ 80 % unit coverage модуля |
| **RF-5** | **UI Static Split** — реализовать план `STATIC_PAGES_MIGRATION_PLAN.md` v1.1 | `web_spiffs.bin` < 1.2 MB, OTA web-only работает |

---

## 6. Риски и митигaция
| Риск | Влияние | Митигaция |
|------|---------|-----------|
| Недостаток времени на рефакторинг → замедление фич | Среднее | Выделить 20 % спринта под tech-debt (policy) |
| Ломаем API во время разделения зависимостей | Высокое | Контрактные тесты (integration) + SemVer-tag |
| Рост размера Flash после IWYU | Низкое | `linker-gc-sections`, `-Os`, убрать dead-code |

---

## 7. Обновление связанной документации
* `STATIC_PAGES_MIGRATION_PLAN.md` → v1.1 (отражает Epic RF-5).

---

## 8. Контроль прогресса
Статус-борд GitHub Projects **"Refactoring & QA 2025-H2"** c автоматическими метками:
* `tech-debt 🐞`  – issue из отчёта статического анализа;
* `architecture 🔧` – задачи эпиков RF-2 / RF-3 ;
* `ui-split 🌐` – Epic RF-5.

Каждая задача имеет: описание smell / метрики, ссылку на отчёт, чек-лист DoD.

---

© JXCT Team, 2025 — документ поддерживается автоматически (ссылка в CI: nightly‐gen-tech-debt). 