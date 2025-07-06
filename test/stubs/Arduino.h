/**
 * @file Arduino.h
 * @brief Простая заглушка Arduino для тестов
 * @details Минимальная заглушка без конфликтов с ESP32
 */

#ifndef ARDUINO_H_STUB
#define ARDUINO_H_STUB

// Базовые типы данных
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

// Заглушки для Serial
class SerialStub {
public:
    void begin(unsigned long baud) {}
    void print(const char* str) {}
    void println(const char* str) {}
    void printf(const char* format, ...) {}
};

extern SerialStub Serial;

// Заглушки для функций Arduino
void delay(unsigned long ms) {}
unsigned long millis() { return 0; }
void pinMode(int pin, int mode) {}
void digitalWrite(int pin, int value) {}
int digitalRead(int pin) { return 0; }

// Константы
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1

#endif // ARDUINO_H_STUB
