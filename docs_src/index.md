# JXCT Soil Sensor Documentation

Welcome to the complete documentation portal of the ESP32-based **JXCT Soil Sensor 7-in-1** firmware.

## Sections

<div class="cards">

<div class="card">
  <h3>📖 Getting started</h3>
  <p>Пошаговый старт и сборка прошивки.</p>
  <a class="md-button md-button--primary" href="getting-started/">Читать</a>
</div>

<div class="card">
  <h3>🔧 Firmware</h3>
  <p>Документация по прошивке:</p>
  <ul>
    <li><a href="firmware/ota/">OTA</a></li>
    <li><a href="firmware/config/">Configuration</a></li>
    <li><a href="firmware/compensation/">Compensation</a></li>
    <li><a href="firmware/modbus/">Modbus</a></li>
  </ul>
  <a class="md-button" href="firmware/config/">Подробнее</a>
</div>

<div class="card">
  <h3>💡 Hardware</h3>
  <p>Аппаратная часть:</p>
  <ul>
    <li><a href="hardware/schematic/">Схема</a></li>
    <li><a href="hardware/bill-of-materials/">BOM</a></li>
  </ul>
  <a class="md-button" href="hardware/schematic/">Открыть</a>
</div>

<div class="card">
  <h3>🌐 API</h3>
  <p>Интерфейсы:</p>
  <ul>
    <li><a href="api/">OpenAPI</a></li>
    <li><a href="api/overview/">Overview</a></li>
  </ul>
  <a class="md-button" href="api/">Документация</a>
</div>

<div class="card">
  <h3>📂 Examples</h3>
  <p>Примеры файлов конфигурации и калибровок.</p>
  <a class="md-button" href="examples/">Каталог</a>
</div>

<div class="card">
  <h3>🌱 Agro</h3>
  <p>Агрономические рекомендации.</p>
  <a class="md-button" href="agro/recommendations/">Читать</a>
</div>

<div class="card">
  <h3>🛠️ Development</h3>
  <p>Разработка:</p>
  <ul>
    <li><a href="development/ARCH_OVERALL/">Architecture</a></li>
    <li><a href="development/QA_REFACTORING_PLAN_2025H2/">QA Plan</a></li>
    <li><a href="development/TECH_DEBT_REPORT_2025-06/">Tech-Debt</a></li>
    <li><a href="development/STATIC_PAGES_MIGRATION_PLAN/">Static pages</a></li>
    <li><a href="development/tech_debt_auto/">Tech-Debt Auto</a></li>
  </ul>
  <a class="md-button" href="development/ARCH_OVERALL/">Подробнее</a>
</div>

<div class="card">
  <h3>📚 C++ API</h3>
  <p>Doxygen HTML документация прошивки.</p>
  <a class="md-button" href="_doxygen/html/index.html">Открыть</a>
</div>

</div>

<details>
<summary>Основные возможности прошивки</summary>

* 🌡️ Real-time показания pH, EC, NPK, влажности и температуры
* 🔄 OTA 2.x с дифф-загрузками и проверкой SHA-256
* 📡 MQTT / ThingSpeak / HTTP API
* 🛠️ Полностью open-source (C++17, PlatformIO)

</details>

> use the left navigation panel to explore the topics. 