#include <SoftwareSerial.h>
#include <CRC16.h>

// Настройки подключения
#define RS485_RX_PIN 2
#define RS485_TX_PIN 3
#define RS485_DE_PIN 4  // Контроль направления передачи
#define SENSOR_BAUDRATE 4800
#define SERIAL_BAUDRATE 9600

// Настройки датчика]
#define SENSOR_ADDRESS 0x01
#define READ_HOLDING_REGISTERS 0x03

class NPKSensor {
private:
  SoftwareSerial* serialPort;
  CRC16 crc;
  uint8_t sensorAddress;

  // Вычисление CRC16 (ModBus RTU)
  uint16_t calculateCRC(uint8_t* data, uint8_t length) {
    uint16_t calculatedCRC = crc.computeCRC(data, length);
    return calculatedCRC;
  }

  // Проверка корректности CRC ответа
  bool validateCRC(uint8_t* response, uint8_t length) {
    uint16_t receivedCRC = (response[length - 1] << 8) | response[length - 2];
    uint16_t calculatedCRC = calculateCRC(response, length - 2);
    return receivedCRC == calculatedCRC;
  }

public:
  NPKSensor(uint8_t rxPin, uint8_t txPin, uint8_t dePin, uint8_t address = 0x01) 
    : serialPort(new SoftwareSerial(rxPin, txPin)), sensorAddress(address) {
    pinMode(dePin, OUTPUT);
  }

  void begin() {
    serialPort->begin(SENSOR_BAUDRATE);
    Serial.begin(SERIAL_BAUDRATE);
  }

  bool readSensorData(uint8_t* data, uint8_t dataLength) {
    // Формирование запроса
    uint8_t query[8];
    query[0] = sensorAddress;
    query[1] = READ_HOLDING_REGISTERS;
    query[2] = 0x00;  // Начальный регистр (старший байт)
    query[3] = 0x00;  // Начальный регистр (младший байт)
    query[4] = 0x00;  // Количество регистров (старший байт)
    query[5] = 0x07;  // Количество регистров (младший байт)

    // Вычисление CRC
    uint16_t queryCRC = calculateCRC(query, 6);
    query[6] = queryCRC & 0xFF;
    query[7] = queryCRC >> 8;

    // Очистка буфера
    while (serialPort->available()) serialPort->read();

    // Отправка запроса
    digitalWrite(RS485_DE_PIN, HIGH);  // Режим передачи
    serialPort->write(query, 8);
    serialPort->flush();
    digitalWrite(RS485_DE_PIN, LOW);   // Режим приема

    // Ожидание ответа с таймаутом
    unsigned long startTime = millis();
    while (serialPort->available() < dataLength) {
      if (millis() - startTime > 1000) {
        Serial.println("Ошибка: Тайм-аут ответа");
        return false;
      }
    }

    // Чтение ответа
    serialPort->readBytes(data, dataLength);

    // Проверка CRC и адреса
    if (data[0] != sensorAddress || !validateCRC(data, dataLength)) {
      Serial.println("Ошибка: Некорректный ответ датчика");
      return false;
    }

    return true;
  }

  bool parseSensorData(uint8_t* data) {
    // Извлечение значений с проверкой диапазонов
    unsigned int soilHumidity = (data[3] << 8) | data[4];
    unsigned int soilTemperature = (data[5] << 8) | data[6];
    unsigned int soilConductivity = (data[7] << 8) | data[8];
    unsigned int soilPH = (data[9] << 8) | data[10];
    unsigned int nitrogen = (data[11] << 8) | data[12];
    unsigned int phosphorus = (data[13] << 8) | data[14];
    unsigned int potassium = (data[15] << 8) | data[16];

    // Проверка диапазонов значений
    if (soilHumidity > 1000 || soilTemperature > 500 || 
        soilConductivity > 10000 || soilPH > 140 ||
        nitrogen > 1000 || phosphorus > 1000 || potassium > 1000) {
      Serial.println("Ошибка: Значения вне допустимого диапазона");
      return false;
    }

    // Вывод данных
    Serial.println("--- Данные NPK-датчика ---");
    Serial.print("Влажность почвы: ");
    Serial.print((float)soilHumidity / 10.0);
    Serial.println(" %");

    Serial.print("Температура почвы: ");
    Serial.print((float)soilTemperature / 10.0);
    Serial.println(" °C");

    Serial.print("Электропроводность: ");
    Serial.print(soilConductivity);
    Serial.println(" мкСм/см");

    Serial.print("pH почвы: ");
    Serial.print((float)soilPH / 10.0);
    Serial.println("");

    Serial.print("Азот: ");
    Serial.println(nitrogen);

    Serial.print("Фосфор: ");
    Serial.println(phosphorus);

    Serial.print("Калий: ");
    Serial.println(potassium);

    return true;
  }
};

NPKSensor npkSensor(RS485_RX_PIN, RS485_TX_PIN, RS485_DE_PIN);

void setup() {
  npkSensor.begin();
  Serial.println("NPK-датчик инициализирован");
}

void loop() {
  uint8_t sensorData[19];
  
  // Попытка чтения данных
  if (npkSensor.readSensorData(sensorData, sizeof(sensorData))) {
    npkSensor.parseSensorData(sensorData);
  }
  
  delay(5000);  // Опрос каждые 5 секунд
} 