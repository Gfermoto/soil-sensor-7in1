# JXCT Soil Sensor 7-in-1 Documentation

Welcome to the complete documentation portal of the ESP32-based **JXCT Soil Sensor 7-in-1** firmware.

## 📖 Quick Navigation

### 🔧 **Firmware Documentation**
- [Architecture Overview](dev/ARCH_OVERALL.md) - Общая архитектура системы
- [QA & Refactoring Plan](dev/QA_REFACTORING_PLAN_2025H2.md) - План рефакторинга
- [Tech Debt Report](dev/TECH_DEBT_REPORT_2025-06.md) - Отчет о техническом долге

### 🌱 **Agro Recommendations**
- [General Recommendations](manuals/AGRO_RECOMMENDATIONS.md) - Агрономические рекомендации
- [Compensation Guide](manuals/COMPENSATION_GUIDE.md) - Руководство по компенсации

### 📚 **API Documentation**
- [C++ API Documentation](html/index.html) - Автоматически сгенерированная документация кода

### 📋 **Examples**
- [Calibration Example](examples/calibration_example.csv) - Пример калибровочной таблицы
- [Test Safe Config](examples/test_safe_config.json) - Безопасная конфигурация для тестов

## 🚀 **Key Features**

### 🌡️ **Real-time Monitoring**
- pH, EC, NPK, влажность и температура
- Двухэтапная компенсация: CSV калибровка + математические модели
- Научно обоснованные алгоритмы (Арчи, Нернст, FAO 56)

### 🔄 **OTA Updates**
- OTA 2.x с дифф-загрузками
- Проверка SHA-256
- Автоматические обновления

### 📡 **Integrations**
- MQTT клиент
- ThingSpeak интеграция
- HTTP REST API
- Веб-интерфейс

### 🛠️ **Development**
- Полностью open-source (C++17, PlatformIO)
- Автоматизированные тесты
- CI/CD pipeline
- Статический анализ кода

## 📊 **Current Status**

**Version:** 3.4.9  
**Last Updated:** 2025-06-24  
**Status:** ✅ Stable with improved calibration system

### ✅ **Recent Improvements**
- Исправлена логика применения калибровочной таблицы
- Двухэтапная компенсация: CSV + математическая
- Объединены агрономические рекомендации
- Обновлена документация с фокусом на стабильность
- Отменена рискованная миграция статических страниц

## 🔗 **Quick Links**

- [GitHub Repository](https://github.com/Gfermoto/soil-sensor-7in1)
- [Latest Release](https://github.com/Gfermoto/soil-sensor-7in1/releases/latest)
- [Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- [Discussions](https://github.com/Gfermoto/soil-sensor-7in1/discussions)

## 📝 **Getting Started**

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Gfermoto/soil-sensor-7in1.git
   cd soil-sensor-7in1
   ```

2. **Install PlatformIO:**
   ```bash
   pip install platformio
   ```

3. **Build the firmware:**
   ```bash
   pio run
   ```

4. **Upload to ESP32:**
   ```bash
   pio run --target upload
   ```

5. **Access web interface:**
   - Connect to WiFi network "JXCT_Setup"
   - Navigate to http://192.168.4.1
   - Configure your settings

## 🤝 **Contributing**

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING_DOCS.md) for details.

---

**© 2025 JXCT Development Team**  
*Made with ❤️ for the agricultural community* 