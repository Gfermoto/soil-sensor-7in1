# 🗒️ Tech-Debt Report — June 2025 (rev-2, 21-Jun)

| Area | Status | Evidence / Metric |
|------|:------:|-------------------|
| Code cleanliness | 🟡 | 1 182 clang-tidy warnings → 38 **high** / 174 **medium** / rest low.  Duplicate `#include <Arduino.h>` устранён; IWYU показывает 2 лишних инклуда. |
| Architecture | 🟡 | Business-логика рекомендаций остаётся в `routes_data.cpp`; абстракция `ISensor` полностью покрывает Real/Fake, циклов include ≈0 (проверено IWYU). |
| Testing | 🟠 | 42 Google-Test кейса + 8 Unity (legacy). Coverage native **31 %** (gcov). Нет e2e-тестов OTA/UI. |
| CI / Static analysis | 🟢 | Build + unit + clang-tidy high-→error (gate). cppcheck MISRA-subset запускается. |
| Security | 🟡 | Rate-limit IP-bucket (20 req/min) внедрён; CSRF-токенов нет; OTA манифест ещё без подписи. |
| Performance | ✅ | Flash 59 %, RAM 24 % (v2.7.1-prod, ESP32-WROOM-32). |
| Documentation | 🟢 | README, API.md, ARCH_OVERALL.md, QA_REFACTORING_PLAN актуальны. |

Легенда статусов: 🟢 — OK, 🟡 — warning, 🟠 — problem, 🔴 — critical.

---
## 1. Key pain points (обновлено)
1. **Тесты** — покрытие 31 %; нет интеграции OTA/Web.
2. **Смешение слоёв** — расчёт рекомендаций всё ещё в роуте данных.
3. **Static analysis debt** — 38 high clang-tidy (raw pointers, C-style cast).
4. **Security gaps** — нет CSRF / HTTPS; OTA подпись TODO.

## 2. Quick-win tasks (Sprint 0)
| ID | Task | Owner | Effort | Done-When |
|----|------|-------|--------|-----------|
| T0-1 | clang-tidy *medium*→error для нового кода | DevOps | 3 h | CI fails on ≥1 new **medium** |
| T0-2 | CSRF-token на POST /save & /api/* | Lead Dev | 6 h | Token header проверен |
| T0-3 | 20 e2e tests (Playwright) Web/UI | QA | 3 d | Coverage Web routes ≥ 70 % |
| T0-4 | Unit tests `CalibrationManager` (load/interpolate) | QA | 1 d | +4 % coverage |

## 3. Long-term goals (ссылки → QA_REFACTORING_PLAN_2025H2.md)
* strict layered architecture (RF-4).
* OTA SHA-256 + rollback metrics (RF-5).
* Unit coverage ≥ 85 %, Integration ≥ 60 % (QA-plan).

---
Prepared by: **AI Assistant** — 2025-06-21 