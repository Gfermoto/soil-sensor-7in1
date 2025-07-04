# 🧪 План QA-аудита и рефакторинга (JXCT H2-2025)

> Версия 1.1   |   Дата: 2025-06-24   |   Ответственные: **QA-Lead**, **Lead Dev**, **Arch**

---

## 1. Введение
Предыдущие дорожные карты устарели после внедрения OTA 2.0, модульного UI и
универсальной CSV-калибровки. Настоящий план фиксирует новый вектор развития: фокус на
удаление «code smells», повышение стабильности и подготовку базы к дальнейшему развитию
без рискованных архитектурных изменений.

---

## 2. Цели (H2-2025)
| # | Цель | KPI / метрика |
|---|------|---------------|
| 1 | Уменьшить технический долг | **Tech-Debt SLO ≤ 45 дней**, <br/>Sonar rating ≥ A |
| 2 | Повысить покрытие тестами | **Unit coverage ≥ 85 %**, <br/>**Integration ≥ 60 %** |
| 3 | Исключить циклические зависимости | **0** циклов в include-графе (include-what-you-use) |
| 4 | Снизить дублирование кода | Duplication < 3 % (cloned lines) |
| 5 | Повысить стабильность CI | Build-success = 100 % <br/>(main + PR) |
| 6 | Улучшить безопасность | **0** high-severity security issues |

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
6. **Security audit** — OWASP IoT Top 10, dependency scanning.

Результаты фиксируются в отчёте `TECH_DEBT_REPORT.md` (шаблон — см. `docs/dev/TECH_DEBT_REPORT.md`).

---

## 4. Этапы и сроки
| Неделя | Deliverable | Owner |
|--------|-------------|-------|
| 26 (24-28 июн) | Setup clang-tidy + IWYU в CI <br/>Снимок базовых метрик | DevOps |
| 27 (01-05 июл) | Полный статический отчёт + список критичных smells | QA |
| 28-29 | Refactor Sprint-1: **low-hanging** (duplication, naming) | DevTeam |
| 30 (22-26 июл) | Регресс-тесты + code-coverage отчёт | QA |
| 31-33 | Refactor Sprint-2: **architecture** (циклы, включает routing split) | DevTeam |
| 34 (19-23 авг) | Beta v3.6.0 (core stability focus) | FW-Lead |

---

## 5. Критические эпики рефакторинга
| Epic ID | Описание | DoD |
|---------|----------|-----|
| **RF-1** | **CI Hardening 2.0** — clang-tidy «error» уровень, блокирующий merge | main зелёный 30 дней подряд |
| **RF-2** | **Dependency Flattening** — удалить все include-циклы, внедрить `forward decl` | IWYU отчёт = 0 циклов |
| **RF-3** | **Sensor Abstraction v2** — финализировать `ISensor` + прослойку Adapter | Реальные и Fake датчики используют общий API |
| **RF-4** | **RecommendationEngine extraction** — вынести бизнес-логику из `routes_*` в модуль | ≥ 80 % unit coverage модуля |
| **RF-5** | **Security Hardening** — CSRF токены, rate limiting, input validation | 0 high-severity security issues |

---

## 6. Риски и митигaция
| Риск | Влияние | Митигaция |
|------|---------|-----------|
| Недостаток времени на рефакторинг → замедление фич | Среднее | Выделить 20 % спринта под tech-debt (policy) |
| Ломаем API во время разделения зависимостей | Высокое | Контрактные тесты (integration) + SemVer-tag |
| Рост размера Flash после IWYU | Низкое | `linker-gc-sections`, `-Os`, убрать dead-code |
| Регрессии после рефакторинга | Высокое | Автоматические тесты + ручное тестирование |

---

## 7. Обновление связанной документации
* Удалён `STATIC_PAGES_MIGRATION_PLAN.md` (отменён из-за рисков)
* Обновлён `TECH_DEBT_REPORT.md` с актуальными метриками
* Фокус на стабильности и качестве кода

---

## 8. Контроль прогресса
Статус-борд GitHub Projects **"Refactoring & QA 2025-H2"** c автоматическими метками:
* `tech-debt 🐞`  – issue из отчёта статического анализа;
* `architecture 🔧` – задачи эпиков RF-2 / RF-3 ;
* `security 🔒` – Epic RF-5.

Каждая задача имеет: описание smell / метрики, ссылку на отчёт, чек-лист DoD.

---

© JXCT Team, 2025 — документ поддерживается автоматически (ссылка в CI: nightly‐gen-tech-debt).

- **Покрытие тестов:** 75.2% (было 31%)

### ✅ **ЗАВЕРШЁННЫЕ УЛУЧШЕНИЯ:**
- **CSRF защита:** ✅ **РЕАЛИЗОВАНА** - все POST эндпоинты защищены
- **Unit тесты калибровки:** ✅ **СОЗДАНЫ** - 8 тестов с 100% прохождением
