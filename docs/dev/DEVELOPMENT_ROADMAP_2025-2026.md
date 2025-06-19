# 🚧 Дорожная карта развития JXCT (H2-2025 → 2026)

## 0. Видение
*Сделать JXCT эталонной open-source платформой для управления почвенными данными, обеспечивающей лабораторную точность, расширяемость и удобство интеграции.*

---

## 1. Этапы и основные релизы
| Квартал | Версия | Ключевые фичи | Риски |
|---------|---------|---------------|-------|
| 2025 Q3 | **v2.7.0** | OTA 2.0, seasonal NPK, refactored UI | Flash-size |
| 2025 Q4 | **v2.8.0** | Plug-in sensors (I2C), Prometheus exporter | API-стабильность |
| 2026 Q1 | **v3.0.0** | Dual-core pipeline, BLE Companion App | Power-consumption |
| 2026 Q2 | **v3.1.0** | AI-predictive irrigation (TinyML) | ML-accuracy |
| 2026 H2 | — TBD | (уточняется) | Roadmap-review |

---

## 2. Перекрёстные направления
1. **Надёжность** — code-coverage ≥ 85 %, CI: build + lint + unit + integration.
2. **Документация** — 100 % public API с примерами, live-docs в Github Pages.
3. **Масштабируемость** — модульный UI, plug-in протоколы (MODBUS, I2C, BLE).
4. **Размер прошивки** — flash ≤ 1 MB, RAM ≤ 320 KB (production).
5. **Сопровождаемость** — clang-tidy, clang-format, Tech Debt SLO ≤ 60 дней.
6. **Сообщество** — monthly release-notes, Telegram AMA, Hacktoberfest tasks.

---

## 3. Милистоуны H2-2025
| Месяц | Epic | Issue-id | Owner |
|-------|------|----------|-------|
| Июль  | OTA 2.0 stabilization | #112 #118 | @fw-lead |
| Авг   | UI dynamic theming | #121 | @ui-lead |
| Сен   | Prometheus exporter | #135 | @fw-eng |
| Окт   | Sensor plug-in API | #144 | @arch |
| Нояб  | Dual-core migration PoC | #150 | @fw-lead |
| Дек   | Beta v2.8.0 | #160 | @pm |

---

## 4. KPI & OKR
| OKR | Целевое значение | Текущий |
|-----|------------------|---------|
| Build-success CI | 100 % main | 96 % |
| Unit coverage | ≥ 80 % | 74 % |
| Issue lead-time | ≤ 7 дней | 11 дней |
| Community PR merged | ≥ 5 / квартал | 2 |

---

## 5. Обновление документа
Обновляется каждое 1-е число месяца архитектором проекта. Изменения подтверждаются в PR с label `roadmap`.

---

© JXCT Team, 2025 