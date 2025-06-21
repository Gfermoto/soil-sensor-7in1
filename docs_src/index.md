# JXCT Soil Sensor Documentation

Welcome to the complete documentation portal of the ESP32-based **JXCT Soil Sensor 7-in-1** firmware.

## Sections

{ .landing-table }

| Раздел | Описание |
|--------|----------|
| 📖 **Getting started** | [Перейти →](getting-started.md) — пошаговый старт |
| 🔧 **Firmware** | Конфигурация, OTA, компенсации, Modbus:<br/>• [OTA](firmware/ota.md) · [Config](firmware/config.md) · [Compensation](firmware/compensation.md) · [Modbus](firmware/modbus.md) |
| 💡 **Hardware** | [Схема](hardware/schematic.md) · [BOM](hardware/bill-of-materials.md) |
| 🌐 **API** | [OpenAPI](api/index.md) · [Overview](api/overview.md) |
| 🌱 **Agro** | [Рекомендации](agro/recommendations.md) |
| 🛠️ **Development** | Архитектура, планы, отчёты:<br/>• [Architecture](development/ARCH_OVERALL.md) · [QA Plan](development/QA_REFACTORING_PLAN_2025H2.md) · [Tech-Debt](development/TECH_DEBT_REPORT_2025-06.md) · [Static pages](development/STATIC_PAGES_MIGRATION_PLAN.md) |
| 📚 **C++ API** | <a class="md-button" href="_doxygen/html/index.html">Doxygen&nbsp;HTML</a> |

<details>
<summary>Основные возможности прошивки</summary>

* 🌡️ Real-time показания pH, EC, NPK, влажности и температуры
* 🔄 OTA 2.x с дифф-загрузками и проверкой SHA-256
* 📡 MQTT / ThingSpeak / HTTP API
* 🛠️ Полностью open-source (C++17, PlatformIO)

</details>

> use the left navigation panel to explore the topics. 