# 📆 Refactoring Epics — H2 2025

| Epic ID | Goal | Owner | KPI / DoD | ETA |
|---------|------|-------|-----------|-----|
| EP-1 | CI Hardening (lint + clang-tidy blocking) | DevOps | Pipeline red on new high issues; clang-format diff = 0 | 2025-07-05 |
| EP-2 | Security & Rate-Limit | Lead Dev | All critical POST routes behind auth + 429 throttle | 2025-07-12 |
| EP-3 | HAL / ISensor finish + FakeSensor parity | Lead Dev | 100 % of sensor ops via interface; native tests green | 2025-07-19 |
| EP-4 | RecommendationEngine extraction | Lead Dev | No business logic in `routes_*` ; ≥80 % unit coverage | 2025-07-30 |
| EP-5 | Test Coverage > 60 % | QA | gcov (native) ≥60 % Core lines | 2025-08-15 |
| EP-6 | OTA Manager (dual-bank + SHA-256) | Lead Dev | Successful OTA/rollback on staging 5/5 runs | 2025-09-10 |

---
## Dependencies
* EP-1 блокирует все → выполнится первым.  
* EP-3 prerequisite для EP-4.  
* EP-6 зависит от EP-1 (CI) + EP-2 (security hooks).

## Milestones (GitHub Projects)
1. **Sprint-0 (quick-wins)** → T0-1…T0-3 из Tech-Debt report.  
2. **Sprint-1** → EP-1 + EP-2.  
3. **Sprint-2** → EP-3 + EP-4.  
4. **Sprint-3** → EP-5 + start EP-6.  
5. **Sprint-4** → Finish EP-6, Release Candidate v2.8.0.

---
Maintainers:  
• **Architect** – roadmap, dependency graph  
• **Lead Developer** – technical execution  
• **QA Specialist** – acceptance criteria, coverage 