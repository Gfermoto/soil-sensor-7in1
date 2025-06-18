# 🗒️ Tech-Debt Report — June 2025

| Area | Status | Evidence / Metric |
|------|:------:|-------------------|
| Code cleanliness | 🟡 | 2 641 clang-tidy warnings → 112 high / 368 medium / rest low. Duplicate `#include <Arduino.h>` in 17 headers. |
| Architecture | 🟡 | Web routes still contain business logic (recommendations, calibration). `ISensor` extracted but RecommendationEngine WIP. |
| Testing | 🔴 | 8 Unity tests (only Core). Coverage 12 % (gcov native). No integration tests. |
| CI / Static analysis | 🟡 | Build + unit tests ✅; cppcheck & clang-tidy only report, not gating. |
| Security | 🟡 | POST routes require password, but no rate-limit / CSRF / encryption. |
| Performance | ✅ | Flash 56 %, RAM 22 % (v2.7.0-prod, ESP32-WROOM-32). |
| Documentation | 🟢 | README, API.md, ARCH_OVERALL.md up-to-date. |

---
## 1. Key pain points
1. **Low test coverage** – high risk of regression during refactor. 
2. **Mixed responsibilities** – `routes_data.cpp` contains compensation & recommendations.
3. **Static analysis debt** – >100 high clang-tidy findings (raw pointers, old-style casts).
4. **Security gaps** – No rate-limit, no HTTPS, no JWT, OTA unsigned.

## 2. Quick-win tasks (sprint 0)
| ID | Task | Owner | Effort | Done-When |
|----|------|-------|--------|-----------|
| T0-1 | Make clang-tidy step «red» for new *high* findings | DevOps | 2 h | CI fails on ≥1 new high warn |
| T0-2 | Add `X-RateLimit` middleware (simple IP bucket) | Lead Dev | 4 h | 429 for >20 req/min |
| T0-3 | Unit tests for `validateMQTT*()` + `isValidHostname/IP` | QA | 1 d | Coverage +8 % |

## 3. Long-term refactor goals (link → ../REFRACTORING_EPICS_2025H2.md)
* Layered architecture strictness (no Core ↘ UI deps).  
* Replace `String` with `std::string_view` in Core.  
* Finish OTA manager (dual-bank, SHA-256).

---
Prepared by: **AI Assistant (acting Architect)** — 2025-06-20 