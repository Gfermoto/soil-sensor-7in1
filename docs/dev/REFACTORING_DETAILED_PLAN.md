# 🔧 Детальный план рефакторинга крупных файлов JXCT v3.10.0

**Дата создания:** 22.01.2025
**Автор:** Stanley Wilson - Senior Technical Architect
**Статус:** 🚀 **ГОТОВ К ВЫПОЛНЕНИЮ**
**Приоритет:** Критический

---

## 📊 Анализ текущего состояния

### 🔍 **Крупные файлы для рефакторинга:**

1. **`src/web/routes_data.cpp`** - 937 строк
   - **Ответственности:** HTTP роуты + вычисления рекомендаций + калибровка + сезонные корректировки
   - **Проблемы:** Смешанная ответственность, дублирование if-else для культур

2. **`src/mqtt_client.cpp`** - 822 строки
   - **Ответственности:** MQTT клиент + Home Assistant + кэширование + OTA + DNS кэш
   - **Проблемы:** Множественные кэши, сложная логика интеграции

3. **`src/modbus_sensor.cpp`** - 726 строк
   - **Ответственности:** Modbus + калибровка + компенсация + валидация + фильтрация
   - **Проблемы:** Научные алгоритмы смешаны с протоколом

---

## 🎯 Стратегия рефакторинга

### **Принципы:**
- **Single Responsibility Principle** - один файл = одна ответственность
- **Dependency Inversion** - зависимости от абстракций
- **Open/Closed Principle** - открыт для расширения, закрыт для изменения
- **DRY (Don't Repeat Yourself)** - устранение дублирования

### **Цели:**
- **Размер файлов:** <300 строк каждый
- **Модульность:** Четкое разделение ответственности
- **Тестируемость:** Легкое создание unit тестов
- **Расширяемость:** Простое добавление новых функций

---

## 📋 Детальный план рефакторинга

### **Этап 1: Подготовка (1 неделя)**

#### **1.1 Создание базовой структуры**
```
src/
├── business/
│   ├── crop_recommendation_engine.h/cpp
│   ├── sensor_calibration_service.h/cpp
│   └── sensor_compensation_service.h/cpp
├── infrastructure/
│   ├── mqtt_connection_manager.h/cpp
│   ├── mqtt_cache_manager.h/cpp
│   └── home_assistant_integration.h/cpp
├── web/
│   ├── routes_data.h/cpp (упрощенный)
│   ├── calibration_upload_handler.h/cpp
│   └── sensor_data_presenter.h/cpp
└── sensors/
    ├── modbus_sensor.h/cpp (упрощенный)
    ├── sensor_data_validator.h/cpp
    └── sensor_data_filter.h/cpp
```

#### **1.2 Создание интерфейсов**
```cpp
// include/business/ICropRecommendationEngine.h
class ICropRecommendationEngine {
public:
    virtual ~ICropRecommendationEngine() = default;
    virtual RecValues computeRecommendations(const String& cropId,
                                           const SoilProfile& soilProfile,
                                           const EnvironmentType& envType) = 0;
};

// include/infrastructure/IMqttConnectionManager.h
class IMqttConnectionManager {
public:
    virtual ~IMqttConnectionManager() = default;
    virtual bool connect() = 0;
    virtual bool publish(const String& topic, const String& payload) = 0;
    virtual bool subscribe(const String& topic) = 0;
};
```

---

### **Этап 2: Рефакторинг routes_data.cpp (2 недели)**

#### **2.1 Выделение CropRecommendationEngine**
```cpp
// src/business/crop_recommendation_engine.cpp
class CropRecommendationEngine : public ICropRecommendationEngine {
private:
    // Конфигурации культур
    struct CropConfig {
        float temp, hum, ec, ph, n, p, k;
    };

    std::map<String, CropConfig> cropConfigs;

    // Сезонные корректировки
    void applySeasonalCorrection(RecValues& rec, Season season, bool isGreenhouse);

public:
    RecValues computeRecommendations(const String& cropId,
                                   const SoilProfile& soilProfile,
                                   const EnvironmentType& envType) override;
};
```

#### **2.2 Выделение CalibrationUploadHandler**
```cpp
// src/web/calibration_upload_handler.cpp
class CalibrationUploadHandler {
private:
    CalibrationManager& calibrationManager;

public:
    bool handleUpload(WebServer& server);
    bool validateCSV(const String& csvData);
    bool saveCalibrationData(const String& csvData, SoilProfile profile);
};
```

#### **2.3 Выделение SensorDataPresenter**
```cpp
// src/web/sensor_data_presenter.cpp
class SensorDataPresenter {
public:
    String formatSensorData(const SensorData& data);
    String formatRecommendations(const RecValues& rec);
    String generateHTML(const SensorData& data, const RecValues& rec);
};
```

#### **2.4 Упрощенный routes_data.cpp**
```cpp
// src/web/routes_data.cpp (новый, ~150 строк)
class DataRoutes {
private:
    ICropRecommendationEngine& recommendationEngine;
    CalibrationUploadHandler& calibrationHandler;
    SensorDataPresenter& dataPresenter;

public:
    void setupRoutes(WebServer& server);

private:
    void handleReadings(WebServer& server);
    void handleApiData(WebServer& server);
    void handleCalibrationUpload(WebServer& server);
};
```

---

### **Этап 3: Рефакторинг mqtt_client.cpp (2 недели)**

#### **3.1 Выделение MqttConnectionManager**
```cpp
// src/infrastructure/mqtt_connection_manager.cpp
class MqttConnectionManager : public IMqttConnectionManager {
private:
    WiFiClient& espClient;
    PubSubClient& mqttClient;
    String server, username, password;
    int port;

    // DNS кэширование
    struct DNSCache {
        String hostname;
        IPAddress ip;
        unsigned long timestamp;
    };
    DNSCache dnsCache;

public:
    bool connect() override;
    bool publish(const String& topic, const String& payload) override;
    bool subscribe(const String& topic) override;

private:
    IPAddress resolveHostname(const String& hostname);
};
```

#### **3.2 Выделение MqttCacheManager**
```cpp
// src/infrastructure/mqtt_cache_manager.cpp
class MqttCacheManager {
private:
    // Кэш JSON данных
    struct SensorDataCache {
        String json;
        unsigned long timestamp;
        bool isValid;
    };

    // Кэш топиков
    struct TopicCache {
        String statusTopic, commandTopic, stateTopic;
        bool isValid;
    };

    SensorDataCache sensorCache;
    TopicCache topicCache;

public:
    String getCachedSensorJson(const SensorData& data);
    String getStatusTopic();
    String getCommandTopic();
    void invalidateCache();
};
```

#### **3.3 Выделение HomeAssistantIntegration**
```cpp
// src/infrastructure/home_assistant_integration.cpp
class HomeAssistantIntegration {
private:
    IMqttConnectionManager& mqttManager;

    // Кэш конфигураций HA
    struct HAConfigCache {
        String tempConfig, humConfig, ecConfig, phConfig;
        String nConfig, pConfig, kConfig;
        bool isValid;
    };
    HAConfigCache configCache;

public:
    void publishConfig();
    void removeConfig();
    void handleDiscovery();

private:
    String generateSensorConfig(const String& sensorType, const String& unit);
};
```

#### **3.4 Упрощенный mqtt_client.cpp**
```cpp
// src/mqtt_client.cpp (новый, ~200 строк)
class MqttClient {
private:
    IMqttConnectionManager& connectionManager;
    MqttCacheManager& cacheManager;
    HomeAssistantIntegration& haIntegration;

public:
    void setup();
    void handle();
    void publishSensorData();

private:
    void handleCommands();
    void publishAvailability(bool online);
};
```

---

### **Этап 4: Рефакторинг modbus_sensor.cpp (2 недели)**

#### **4.1 Выделение SensorCalibrationService**
```cpp
// src/business/sensor_calibration_service.cpp
class SensorCalibrationService {
private:
    CalibrationManager& calibrationManager;

public:
    void applyCalibration(SensorData& data, SoilProfile profile);
    bool validateCalibrationData(const SensorData& data);

private:
    float applySingleCalibration(float value, SoilProfile profile);
};
```

#### **4.2 Выделение SensorCompensationService**
```cpp
// src/business/sensor_compensation_service.cpp
class SensorCompensationService {
public:
    void applyCompensation(SensorData& data, SoilType soilType);

private:
    // Модель Арчи для EC
    float correctEC(float ec25, float temperature, float humidity, SoilType soilType);

    // Уравнение Нернста для pH
    float correctPH(float phRaw, float temperature);

    // FAO 56 для NPK
    void correctNPK(float temperature, float humidity, NPKReferences& npk, SoilType soilType);
};
```

#### **4.3 Выделение SensorDataValidator**
```cpp
// src/sensors/sensor_data_validator.cpp
class SensorDataValidator {
public:
    bool validateSensorData(const SensorData& data);
    bool isInRange(float value, float min, float max);
    bool checkConsistency(const SensorData& data);

private:
    struct ValidationRanges {
        float tempMin, tempMax;
        float humMin, humMax;
        float ecMin, ecMax;
        float phMin, phMax;
        float npkMin, npkMax;
    };
    ValidationRanges ranges;
};
```

#### **4.4 Выделение SensorDataFilter**
```cpp
// src/sensors/sensor_data_filter.cpp
class SensorDataFilter {
private:
    // Moving average фильтры
    struct MovingAverageFilter {
        std::array<float, 10> buffer;
        uint8_t index, count;
    };

    MovingAverageFilter tempFilter, humFilter, ecFilter, phFilter;
    MovingAverageFilter nFilter, pFilter, kFilter;

public:
    void initFilters();
    void applyFilters(SensorData& data);
    void detectIrrigation(SensorData& data);

private:
    float calculateMovingAverage(MovingAverageFilter& filter, float newValue);
};
```

#### **4.5 Упрощенный modbus_sensor.cpp**
```cpp
// src/modbus_sensor.cpp (новый, ~250 строк)
class ModbusSensor {
private:
    ModbusMaster& modbus;
    SensorCalibrationService& calibrationService;
    SensorCompensationService& compensationService;
    SensorDataValidator& validator;
    SensorDataFilter& filter;

public:
    void setup();
    bool readSensorData(SensorData& data);
    bool testConnection();

private:
    int readBasicParameters(SensorData& data);
    int readNPKParameters(SensorData& data);
    bool readSingleRegister(uint16_t reg_addr, float multiplier, float& result);
    void preTransmission();
    void postTransmission();
};
```

---

### **Этап 5: Создание фабрик и устранение дублирования (1 неделя)**

#### **5.1 CropConfigFactory**
```cpp
// src/business/crop_config_factory.cpp
class CropConfigFactory {
public:
    static CropConfig getConfig(const String& cropId);
    static void applySeasonalAdjustment(RecValues& rec, Season season, bool isGreenhouse);

private:
    static const std::map<String, CropConfig> cropConfigs;
    static const std::map<Season, SeasonalAdjustment> seasonalAdjustments;
};
```

#### **5.2 SensorFactory**
```cpp
// src/sensors/sensor_factory.cpp
class SensorFactory {
public:
    static std::unique_ptr<ISensor> createSensor(SensorType type);
    static std::unique_ptr<ISensor> createModbusSensor(uint8_t address);
    static std::unique_ptr<ISensor> createFakeSensor();
};
```

---

### **Этап 6: Интеграция и тестирование (1 неделя)**

#### **6.1 Dependency Injection Container**
```cpp
// src/di/container.h
class DIContainer {
private:
    std::map<String, std::shared_ptr<void>> services;

public:
    template<typename T>
    void registerService(std::shared_ptr<T> service);

    template<typename T>
    std::shared_ptr<T> getService();
};
```

#### **6.2 Обновление main.cpp**
```cpp
// src/main.cpp (обновленный)
void setup() {
    // Инициализация DI контейнера
    auto container = std::make_unique<DIContainer>();

    // Регистрация сервисов
    container->registerService<ICropRecommendationEngine>(
        std::make_shared<CropRecommendationEngine>());
    container->registerService<IMqttConnectionManager>(
        std::make_shared<MqttConnectionManager>(espClient, mqttClient));

    // Инициализация компонентов
    auto dataRoutes = std::make_unique<DataRoutes>(*container);
    auto mqttClient = std::make_unique<MqttClient>(*container);
    auto modbusSensor = std::make_unique<ModbusSensor>(*container);

    // Настройка
    dataRoutes->setupRoutes(server);
    mqttClient->setup();
    modbusSensor->setup();
}
```

---

## 📈 Ожидаемые результаты

### **Метрики качества:**
- **Размер файлов:** 937 → 150 строк (routes_data.cpp)
- **Размер файлов:** 822 → 200 строк (mqtt_client.cpp)
- **Размер файлов:** 726 → 250 строк (modbus_sensor.cpp)
- **Технический долг:** 18 → 8 (-55%)
- **Покрытие тестами:** 70.8% → 85% (+20%)

### **Архитектурные улучшения:**
- ✅ **Single Responsibility** - каждый класс имеет одну ответственность
- ✅ **Dependency Inversion** - зависимости от интерфейсов
- ✅ **Testability** - легко создавать unit тесты
- ✅ **Maintainability** - простое изменение и расширение
- ✅ **Reusability** - компоненты можно переиспользовать

---

## 🚀 План выполнения

### **Неделя 1: Подготовка**
- [ ] Создание структуры папок
- [ ] Определение интерфейсов
- [ ] Создание базовых классов

### **Неделя 2-3: routes_data.cpp**
- [ ] Выделение CropRecommendationEngine
- [ ] Выделение CalibrationUploadHandler
- [ ] Выделение SensorDataPresenter
- [ ] Упрощение routes_data.cpp

### **Неделя 4-5: mqtt_client.cpp**
- [ ] Выделение MqttConnectionManager
- [ ] Выделение MqttCacheManager
- [ ] Выделение HomeAssistantIntegration
- [ ] Упрощение mqtt_client.cpp

### **Неделя 6-7: modbus_sensor.cpp**
- [ ] Выделение SensorCalibrationService
- [ ] Выделение SensorCompensationService
- [ ] Выделение SensorDataValidator
- [ ] Выделение SensorDataFilter
- [ ] Упрощение modbus_sensor.cpp

### **Неделя 8: Фабрики и DI**
- [ ] Создание CropConfigFactory
- [ ] Создание SensorFactory
- [ ] Внедрение DI контейнера
- [ ] Обновление main.cpp

### **Неделя 9: Тестирование**
- [ ] Unit тесты для новых классов
- [ ] Integration тесты
- [ ] Performance тесты
- [ ] Финальная проверка

---

## 🔍 Контроль качества

### **Критерии успеха:**
- [ ] Все файлы <300 строк
- [ ] 100% тесты проходят
- [ ] Покрытие кода >80%
- [ ] Технический долг <10
- [ ] Время сборки не увеличилось

### **Метрики отслеживания:**
- **Еженедельные checkpoints** - прогресс по файлам
- **Code review** - качество каждого выделенного класса
- **Performance monitoring** - не допустить деградации
- **Test coverage** - покрытие новых классов

---

**Заключение:** Данный план обеспечивает системный и профессиональный подход к рефакторингу крупных файлов. Каждый этап имеет четкие цели, критерии успеха и план тестирования. Результатом будет модульная, тестируемая и расширяемая архитектура enterprise-уровня.
