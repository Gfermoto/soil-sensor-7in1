#pragma once
// Заглушка Arduino.h для тестирования и статического анализа

// Убираем windows.h для clang-tidy
// #include <windows.h>

#include <cstdint>
#include <cstring>
#include <string>

// Основные типы Arduino
typedef std::string String;
typedef uint8_t byte;

// Константы
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

// Макросы
#define PROGMEM
#define F(str) (str)

// Функции
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis();
unsigned long micros();
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

// Serial заглушка
class SerialClass {
public:
    void begin(unsigned long baud);
    void print(const char* str);
    void println(const char* str);
    void printf(const char* format, ...);
};

extern SerialClass Serial;
