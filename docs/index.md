# 🌱 JXCT 7-в-1 Датчик Почвы

<div style="text-align: center; margin: 30px 0;">
  <h1 style="color: #2e7d32; font-size: 2.5em; margin-bottom: 10px;">🌱 JXCT 7-в-1 Датчик Почвы</h1>
  <p style="font-size: 1.2em; color: #666; margin-bottom: 20px;">Умный датчик почвы с веб-интерфейсом для точного мониторинга параметров почвы</p>
  <div style="display: flex; justify-content: center; gap: 15px; flex-wrap: wrap; margin: 20px 0;">
    <span style="background: #e8f5e8; padding: 8px 16px; border-radius: 20px; font-size: 0.9em; color: #2e7d32;">📊 7 параметров</span>
    <span style="background: #e3f2fd; padding: 8px 16px; border-radius: 20px; font-size: 0.9em; color: #1976d2;">🔬 Научная компенсация</span>
    <span style="background: #fff3e0; padding: 8px 16px; border-radius: 20px; font-size: 0.9em; color: #f57c00;">🌐 Веб-интерфейс</span>
    <span style="background: #f3e5f5; padding: 8px 16px; border-radius: 20px; font-size: 0.9em; color: #7b1fa2;">🔧 ESP32 + PlatformIO</span>
  </div>
</div>

---

## 🚀 Быстрый старт

<div style="background: #f8f9fa; padding: 20px; border-radius: 10px; margin: 20px 0; border-left: 4px solid #28a745;">
  <h3 style="margin-top: 0; color: #28a745;">⚡ Установка за 5 минут</h3>
  <ol style="margin: 10px 0; padding-left: 20px;">
    <li><strong>Клонируйте репозиторий:</strong><br>
      <code>git clone https://github.com/Gfermoto/soil-sensor-7in1.git</code></li>
    <li><strong>Откройте в PlatformIO:</strong><br>
      <code>cd soil-sensor-7in1 && pio run</code></li>
    <li><strong>Загрузите на ESP32:</strong><br>
      <code>pio run --target upload</code></li>
    <li><strong>Подключитесь к WiFi:</strong><br>
      Сеть: <code>JXCT_Setup</code> → <code>http://192.168.4.1</code></li>
  </ol>
</div>

---

## 📚 Полная документация

<div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin: 30px 0;">

### 🔧 **Техническая документация**
<div style="background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);">
  <h4 style="margin-top: 0; color: #1976d2;">📖 Руководства пользователя</h4>
  <ul style="margin: 10px 0; padding-left: 20px;">
    <li><a href="manuals/USER_GUIDE.md" style="color: #1976d2; text-decoration: none;">📋 Руководство пользователя</a> - Полное руководство по использованию</li>
    <li><a href="manuals/TECHNICAL_DOCS.md" style="color: #1976d2; text-decoration: none;">🔧 Техническая документация</a> - Архитектура и API</li>
    <li><a href="manuals/API.md" style="color: #1976d2; text-decoration: none;">📊 API документация</a> - REST API и интеграции</li>
  </ul>
  
  <h4 style="color: #1976d2;">🌱 Агрономические рекомендации</h4>
  <ul style="margin: 10px 0; padding-left: 20px;">
    <li><a href="manuals/AGRO_RECOMMENDATIONS.md" style="color: #1976d2; text-decoration: none;">🌿 Агрономические рекомендации</a> - Рекомендации по культурам</li>
    <li><a href="manuals/COMPENSATION_GUIDE.md" style="color: #1976d2; text-decoration: none;">🔬 Руководство по компенсации</a> - Научные модели и калибровка</li>
  </ul>
</div>

### 💻 **Разработка и код**
<div style="background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);">
  <h4 style="margin-top: 0; color: #f57c00;">📚 Документация кода</h4>
  <ul style="margin: 10px 0; padding-left: 20px;">
    <li><a href="html/index.html" style="color: #f57c00; text-decoration: none; font-weight: bold;">📖 Doxygen документация</a> - Полная документация C++ кода</li>
    <li><a href="html/classes.html" style="color: #f57c00; text-decoration: none;">🏗️ Классы и интерфейсы</a> - Архитектура классов</li>
    <li><a href="html/files.html" style="color: #f57c00; text-decoration: none;">📁 Исходные файлы</a> - Структура проекта</li>
  </ul>
  
  <h4 style="color: #f57c00;">🔧 Разработка</h4>
  <ul style="margin: 10px 0; padding-left: 20px;">
    <li><a href="dev/ARCH_OVERALL.md" style="color: #f57c00; text-decoration: none;">🏗️ Архитектура системы</a> - Общая архитектура</li>
    <li><a href="dev/QA_REFACTORING_PLAN_2025H2.md" style="color: #f57c00; text-decoration: none;">📋 План рефакторинга</a> - Дорожная карта развития</li>
    <li><a href="dev/TECH_DEBT_REPORT_2025-06.md" style="color: #f57c00; text-decoration: none;">📊 Технический долг</a> - Отчет о качестве кода</li>
  </ul>
</div>

### 📋 **Примеры и конфигурация**
<div style="background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);">
  <h4 style="margin-top: 0; color: #7b1fa2;">📄 Примеры файлов</h4>
  <ul style="margin: 10px 0; padding-left: 20px;">
    <li><a href="examples/calibration_example.csv" style="color: #7b1fa2; text-decoration: none;">📊 Пример калибровки</a> - CSV файл калибровочной таблицы</li>
    <li><a href="examples/test_safe_config.json" style="color: #7b1fa2; text-decoration: none;">⚙️ Тестовая конфигурация</a> - Безопасные настройки для тестов</li>
  </ul>
  
  <h4 style="color: #7b1fa2;">🤝 Участие в разработке</h4>
  <ul style="margin: 10px 0; padding-left: 20px;">
    <li><a href="CONTRIBUTING_DOCS.md" style="color: #7b1fa2; text-decoration: none;">📝 Руководство по участию</a> - Как внести свой вклад</li>
  </ul>
</div>

</div>

---

## 🌟 Основные возможности

<div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 15px; margin: 30px 0;">

### 🌡️ **7 параметров в одном датчике**
<div style="background: #e8f5e8; padding: 15px; border-radius: 8px;">
  <ul style="margin: 5px 0; padding-left: 20px;">
    <li><strong>Температура:</strong> 0-50°C, ±0.5°C</li>
    <li><strong>Влажность:</strong> 0-100%, ±3%</li>
    <li><strong>EC:</strong> 0-5000 µS/cm, ±5%</li>
    <li><strong>pH:</strong> 3-9 pH, ±0.3 pH</li>
    <li><strong>Азот (N):</strong> 0-2000 мг/кг, ±10%</li>
    <li><strong>Фосфор (P):</strong> 0-1000 мг/кг, ±10%</li>
    <li><strong>Калий (K):</strong> 0-2000 мг/кг, ±10%</li>
  </ul>
</div>

### 🔬 **Научная компенсация**
<div style="background: #e3f2fd; padding: 15px; border-radius: 8px;">
  <ul style="margin: 5px 0; padding-left: 20px;">
    <li><strong>Модель Арчи (1942):</strong> Компенсация EC</li>
    <li><strong>Уравнение Нернста:</strong> Температурная поправка pH</li>
    <li><strong>FAO 56 + Eur. J. Soil Sci.:</strong> NPK компенсация</li>
    <li><strong>CSV калибровка:</strong> Лабораторная поверка</li>
  </ul>
</div>

### 🌐 **Веб-интерфейс**
<div style="background: #fff3e0; padding: 15px; border-radius: 8px;">
  <ul style="margin: 5px 0; padding-left: 20px;">
    <li><strong>Адаптивный дизайн:</strong> Для всех устройств</li>
    <li><strong>Цветовая индикация:</strong> Статус измерений</li>
    <li><strong>OTA обновления:</strong> По воздуху</li>
    <li><strong>Экспорт CSV:</strong> Данные для анализа</li>
  </ul>
</div>

### 🔧 **Технические особенности**
<div style="background: #f3e5f5; padding: 15px; border-radius: 8px;">
  <ul style="margin: 5px 0; padding-left: 20px;">
    <li><strong>ESP32:</strong> Мощный микроконтроллер</li>
    <li><strong>Modbus RTU:</strong> Надежная связь</li>
    <li><strong>MQTT:</strong> IoT интеграция</li>
    <li><strong>ThingSpeak:</strong> Облачное хранение</li>
  </ul>
</div>

</div>

---

## 📊 Текущий статус

<div style="background: #f8f9fa; padding: 20px; border-radius: 10px; margin: 20px 0;">
  <div style="display: flex; justify-content: space-between; align-items: center; flex-wrap: wrap; gap: 15px;">
    <div>
      <strong>Версия:</strong> <span style="background: #28a745; color: white; padding: 4px 8px; border-radius: 4px;">3.4.9</span>
    </div>
    <div>
      <strong>Платформа:</strong> <span style="background: #17a2b8; color: white; padding: 4px 8px; border-radius: 4px;">ESP32 + PlatformIO</span>
    </div>
    <div>
      <strong>Статус:</strong> <span style="background: #28a745; color: white; padding: 4px 8px; border-radius: 4px;">✅ Стабильный</span>
    </div>
    <div>
      <strong>Лицензия:</strong> <span style="background: #ffc107; color: #212529; padding: 4px 8px; border-radius: 4px;">MIT</span>
    </div>
  </div>
</div>

### ✅ Последние улучшения
- 🔧 Исправлена логика применения калибровочной таблицы
- 🔬 Двухэтапная компенсация: CSV + математическая
- 🌿 Объединены агрономические рекомендации
- 📚 **Полная консистентность документации** - все документы имеют единообразную структуру
- 🚫 Отменена рискованная миграция статических страниц

### 📖 **Консистентность документации**
<div style="background: #e8f5e8; padding: 15px; border-radius: 8px; margin: 15px 0;">
  <p style="margin: 5px 0;"><strong>✅ Все документы теперь имеют:</strong></p>
  <ul style="margin: 5px 0; padding-left: 20px;">
    <li>Блоки содержания в начале каждого документа</li>
    <li>Единообразные блоки ссылок в конце</li>
    <li>Консистентные заголовки и форматирование</li>
    <li>Профессиональную структуру навигации</li>
  </ul>
</div>

---

## 🔗 Полезные ссылки

<div style="display: flex; justify-content: center; gap: 15px; flex-wrap: wrap; margin: 30px 0;">
  <a href="https://github.com/Gfermoto/soil-sensor-7in1" style="background: #24292e; color: white; padding: 12px 20px; border-radius: 6px; text-decoration: none; display: flex; align-items: center; gap: 8px;">
    📦 GitHub Repository
  </a>
  <a href="https://github.com/Gfermoto/soil-sensor-7in1/releases/latest" style="background: #28a745; color: white; padding: 12px 20px; border-radius: 6px; text-decoration: none; display: flex; align-items: center; gap: 8px;">
    🚀 Latest Release
  </a>
  <a href="https://github.com/Gfermoto/soil-sensor-7in1/issues" style="background: #dc3545; color: white; padding: 12px 20px; border-radius: 6px; text-decoration: none; display: flex; align-items: center; gap: 8px;">
    🐛 Issues
  </a>
  <a href="https://t.me/Gfermoto" style="background: #0088cc; color: white; padding: 12px 20px; border-radius: 6px; text-decoration: none; display: flex; align-items: center; gap: 8px;">
    💬 Telegram Support
  </a>
</div>

---

<div style="text-align: center; margin: 40px 0; padding: 20px; background: #f8f9fa; border-radius: 10px;">
  <p style="margin: 5px 0; color: #666;">© 2025 JXCT Development Team</p>
  <p style="margin: 5px 0; color: #666;">Made with ❤️ for the agricultural community</p>
</div> 