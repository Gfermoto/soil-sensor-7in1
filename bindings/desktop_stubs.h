/**
 * @file desktop_stubs.h
 * @brief Заглушки Arduino функций для desktop-сборки
 */

#ifndef DESKTOP_STUBS_H
#define DESKTOP_STUBS_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// Заглушки для Arduino типов
typedef uint8_t byte;
typedef uint16_t word;

// Заглушки для Arduino функций
inline unsigned long millis() { return 0; }
inline void delay(unsigned long ms) { (void)ms; }
inline void digitalWrite(int pin, int value) { (void)pin; (void)value; }
inline int digitalRead(int pin) { (void)pin; return 0; }
inline void analogWrite(int pin, int value) { (void)pin; (void)value; }
inline int analogRead(int pin) { (void)pin; return 0; }
inline void pinMode(int pin, int mode) { (void)pin; (void)mode; }

// Заглушки для Serial
class SerialClass {
public:
    void begin(int baud) { (void)baud; }
    void print(const char* str) { (void)str; }
    void print(int value) { (void)value; }
    void print(float value) { (void)value; }
    void println(const char* str) { (void)str; }
    void println(int value) { (void)value; }
    void println(float value) { (void)value; }
    bool available() { return false; }
    int read() { return -1; }
};

extern SerialClass Serial;

// Заглушки для WiFi
class WiFiClass {
public:
    bool begin(const char* ssid, const char* password) { (void)ssid; (void)password; return true; }
    bool isConnected() { return true; }
    String localIP() { return "192.168.1.100"; }
};

extern WiFiClass WiFi;

// Заглушки для String
class String {
private:
    std::string str;
public:
    String() : str("") {}
    String(const char* s) : str(s) {}
    String(const std::string& s) : str(s) {}
    
    const char* c_str() const { return str.c_str(); }
    int length() const { return str.length(); }
    bool isEmpty() const { return str.empty(); }
    
    String& operator+=(const String& other) { str += other.str; return *this; }
    String& operator+=(const char* s) { str += s; return *this; }
    
    operator const char*() const { return str.c_str(); }
};

// Заглушки для других Arduino функций
inline void yield() {}
inline void esp_task_wdt_init(int timeout, bool panic) { (void)timeout; (void)panic; }
inline void esp_task_wdt_add(void* task) { (void)task; }
inline void esp_task_wdt_delete(void* task) { (void)task; }
inline void esp_task_wdt_reset() {}

#endif // DESKTOP_STUBS_H 