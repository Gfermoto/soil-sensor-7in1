# 🗒️ Tech-Debt Report — June 2025 (rev-6, 26-Jun)

| Area | Status | Evidence / Metric |
|------|:------:|-------------------|
| Code cleanliness | 🔴 | 0 clang-tidy warnings (**устранены**); IWYU: 0 циклов, 5 неиспользуемых include; дублирование: 18 уникальных дубликатов (score=18, 10 файлов). |
| Architecture | 🟡 | Бизнес-логика рекомендаций всё ещё в `routes_data.cpp`; ISensor покрывает Real/Fake; циклов include нет. |
| Testing | 🟠 | 42 Google-Test + 8 Unity (legacy). Coverage **31 %** (gcov). Нет e2e-тестов OTA/UI. |
| CI / Static analysis | 🟢 | Build + unit + clang-tidy high/medium→error (gate). cppcheck MISRA-subset запускается. |
| Security | 🟡 | Rate-limit IP-bucket (20 req/min) внедрён; CSRF-токенов нет; OTA манифест без подписи. |
| Performance | ✅ | Flash 59 %, RAM 24 % (v3.4.9-prod, ESP32-WROOM-32). |
| Documentation | 🟢 | README, API.md, ARCH_OVERALL.md, QA_REFACTORING_PLAN актуальны. |

Легенда статусов: 🟢 — OK, 🟡 — warning, 🟠 — problem, 🔴 — critical.

---

## 1. Key pain points (обновлено)
1. **Критический уровень технического долга** — общий score: 295 (🔴 Critical, см. ниже).
2. **Дублирование кода** — 18 уникальных дубликатов, требуется рефакторинг.
3. **Тесты** — покрытие 31 %, нет e2e для OTA/Web.
4. **Смешение слоёв** — расчёт рекомендаций в роуте данных.
5. **Security gaps** — нет CSRF/HTTPS, OTA подпись TODO.

## 2. Quick-win tasks (Sprint 0)
| ID | Task | Owner | Effort | Done-When |
|----|------|-------|--------|-----------|
| T0-1 | clang-tidy *medium*→error для нового кода | DevOps | 3 h | **Выполнено: 0 warning** |
| T0-2 | CSRF-token на POST /save & /api/* | Lead Dev | 6 h | Token header проверен |
| T0-3 | 20 e2e tests (Playwright) Web/UI | QA | 3 d | Coverage Web routes ≥ 70 % |
| T0-4 | Unit tests `CalibrationManager` (load/interpolate) | QA | 1 d | +4 % coverage |
| T0-5 | Security audit OWASP IoT Top 10 | Security | 2 d | 0 high-severity issues |

## 3. Long-term goals (ссылки → QA_REFACTORING_PLAN_2025H2.md)
* strict layered architecture (RF-4).
* OTA SHA-256 + rollback metrics (RF-5).
* Unit coverage ≥ 85 %, Integration ≥ 60 % (QA-plan).
* Security hardening (CSRF, input validation, rate limiting).

---

## 4. Архитектурные решения (обновлено)
- **Отменена миграция статических страниц** — риски превышают выгоды
- **Фокус на стабильности** — приоритет качеству кода над архитектурными экспериментами
- **Постепенное улучшение** — эволюционное развитие без революционных изменений

---

**Обновление 26.06.2025:**
- Критический уровень технического долга: score=295 (🔴 Critical, см. technical-debt-ci.json).
- Все предупреждения clang-tidy устранены (0 high/medium/low).
- Оптимизированы include, IWYU: 0 циклов, 5 неиспользуемых include.
- Дублирование: 18 уникальных дубликатов (10 файлов).
- Все unit-тесты проходят, но покрытие остаётся низким.
- Требуется рефакторинг дублирующихся функций и усиление тестирования.
- Рекомендация: внедрить e2e-тесты, повысить покрытие, устранить дублирование, усилить безопасность (CSRF, OTA подпись).

Prepared by: **AI Assistant** — 2025-06-26 