#ifndef ESP32_STUBS_H
#define ESP32_STUBS_H

#include <cstdint>
#include <cstring>
#include <string>

// Заглушки для ESP32 типов (только если не определены системой)
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

// Не определяем uint32_t и int32_t, так как они уже есть в stdint.h

// Заглушка для FreeRTOS типов
typedef unsigned int UBaseType_t;

// Заглушка для HTTP методов
enum HTTPMethod
{
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE
};

// --- String ---
class String
{
   private:
    std::string data;

   public:
    String() : data("") {}
    String(const char* str) : data(str ? str : "") {}
    String(const std::string& str) : data(str) {}
    String(int value) : data(std::to_string(value)) {}
    String(float value) : data(std::to_string(value)) {}
    const char* c_str() const
    {
        return data.c_str();
    }
    int length() const
    {
        return data.length();
    }
    bool equals(const String& other) const
    {
        return data == other.data;
    }
    bool equals(const char* str) const
    {
        return data == (str ? str : "");
    }
    bool startsWith(const char* prefix) const
    {
        if (!prefix) return false;
        return data.substr(0, strlen(prefix)) == prefix;
    }
    String substring(int beginIndex) const
    {
        if (beginIndex >= data.length()) return String("");
        return String(data.substr(beginIndex));
    }
    String substring(int beginIndex, int endIndex) const
    {
        if (beginIndex >= data.length() || beginIndex >= endIndex) return String("");
        return String(data.substr(beginIndex, endIndex - beginIndex));
    }
    int indexOf(char ch) const
    {
        size_t pos = data.find(ch);
        return pos == std::string::npos ? -1 : pos;
    }
    int indexOf(const char* str) const
    {
        size_t pos = data.find(str);
        return pos == std::string::npos ? -1 : pos;
    }
    String operator+(const String& other) const
    {
        return String(data + other.data);
    }
    String operator+(const char* str) const
    {
        return String(data + (str ? str : ""));
    }
    bool operator==(const String& other) const
    {
        return data == other.data;
    }
    bool operator==(const char* str) const
    {
        return data == (str ? str : "");
    }
    operator const char*() const
    {
        return data.c_str();
    }
};

// --- JsonObject ---
class JsonObject
{
   public:
    JsonObject() {}
    bool containsKey(const char* key) const
    {
        return false;
    }
    String get(const char* key) const
    {
        return String("");
    }
    void set(const char* key, const String& value) {}
    void set(const char* key, int value) {}
    void set(const char* key, float value) {}
    void set(const char* key, bool value) {}
    JsonObject createNestedObject(const char* key)
    {
        return JsonObject();
    }
    // JsonArray объявлен ниже
};

// --- Stream ---
class Stream
{
   public:
    virtual int available()
    {
        return 0;
    }
    virtual int read()
    {
        return -1;
    }
    virtual size_t write(uint8_t)
    {
        return 0;
    }
    virtual size_t write(const uint8_t*, size_t)
    {
        return 0;
    }
};

// --- HardwareSerial ---
class HardwareSerial : public Stream
{
   private:
    int uart_num;

   public:
    HardwareSerial(int uart) : uart_num(uart) {}
    void begin(unsigned long baud) {}
    void end() {}
    int available() override
    {
        return 0;
    }
    int read() override
    {
        return -1;
    }
    size_t write(uint8_t) override
    {
        return 0;
    }
    size_t write(const uint8_t*, size_t) override
    {
        return 0;
    }
    void print(const String& str) {}
    void print(const char* str) {}
    void print(int value) {}
    void print(float value) {}
    void println(const String& str) {}
    void println(const char* str) {}
    void println(int value) {}
    void println(float value) {}
    void println() {}
};

// --- SerialClass ---
class SerialClass : public HardwareSerial
{
   public:
    SerialClass() : HardwareSerial(0) {}
};

// --- WiFiClass ---
class WiFiClass
{
   public:
    bool begin(const char* ssid, const char* password)
    {
        return true;
    }
    bool disconnect()
    {
        return true;
    }
    bool isConnected()
    {
        return true;
    }
    String localIP()
    {
        return String("192.168.1.100");
    }
    String macAddress()
    {
        return String("AA:BB:CC:DD:EE:FF");
    }
    int RSSI()
    {
        return -50;
    }
};

// --- WebServer ---
class WebServer
{
   private:
    int port;

   public:
    WebServer(int p) : port(p) {}
    void begin() {}
    void handleClient() {}
    void on(const char* path, void (*handler)()) {}
    void on(const char* path, HTTPMethod method, void (*handler)()) {}
    void send(int code, const char* contentType, const char* content) {}
    void send(int code, const char* contentType, const String& content) {}
    void sendHeader(const char* name, const char* value) {}
    String arg(const char* name)
    {
        return String("");
    }
    String arg(int i)
    {
        return String("");
    }
    int args()
    {
        return 0;
    }
    bool hasArg(const char* name)
    {
        return false;
    }
    String uri()
    {
        return String("");
    }
    HTTPMethod method()
    {
        return HTTP_GET;
    }
    String hostHeader()
    {
        return String("");
    }
    String header(const char* name)
    {
        return String("");
    }
    bool hasHeader(const char* name)
    {
        return false;
    }
    String headerName(int i)
    {
        return String("");
    }
    int headers()
    {
        return 0;
    }
    void enableCORS(bool enable) {}
    void enableCrossOrigin(bool enable) {}
    void close() {}
};

// --- File ---
class File
{
   private:
    std::string filename;
    size_t position_;
    size_t size_;
    bool isDirectory_;

   public:
    File() : filename(""), position_(0), size_(0), isDirectory_(false) {}
    File(const char* name) : filename(name ? name : ""), position_(0), size_(0), isDirectory_(false) {}
    bool isValid() const
    {
        return !filename.empty();
    }
    const char* name() const
    {
        return filename.c_str();
    }
    bool isDirectory() const
    {
        return isDirectory_;
    }
    size_t size() const
    {
        return size_;
    }
    size_t position() const
    {
        return position_;
    }
    bool seek(size_t pos)
    {
        if (pos <= size_)
        {
            position_ = pos;
            return true;
        }
        return false;
    }
    size_t read(uint8_t* buffer, size_t size)
    {
        return 0;
    }
    size_t write(const uint8_t* buffer, size_t size)
    {
        return 0;
    }
    void close() {}
    File openNextFile()
    {
        return File();
    }
    void rewindDirectory() {}
};

// --- FS ---
class FS
{
   public:
    bool begin(bool formatOnFail = false)
    {
        return true;
    }
    void end() {}
    bool format()
    {
        return true;
    }
    size_t totalBytes()
    {
        return 1024 * 1024;
    }
    size_t usedBytes()
    {
        return 512 * 1024;
    }
    size_t freeBytes()
    {
        return 512 * 1024;
    }
    File open(const char* path, const char* mode = "r")
    {
        return File(path);
    }
    File open(const String& path, const char* mode = "r")
    {
        return File(path.c_str());
    }
    bool exists(const char* path)
    {
        return false;
    }
    bool exists(const String& path)
    {
        return false;
    }
    bool remove(const char* path)
    {
        return true;
    }
    bool remove(const String& path)
    {
        return true;
    }
    bool rename(const char* pathFrom, const char* pathTo)
    {
        return true;
    }
    bool rename(const String& pathFrom, const String& pathTo)
    {
        return true;
    }
    bool mkdir(const char* path)
    {
        return true;
    }
    bool mkdir(const String& path)
    {
        return true;
    }
    bool rmdir(const char* path)
    {
        return true;
    }
    bool rmdir(const String& path)
    {
        return true;
    }
};

// --- LittleFSClass ---
class LittleFSClass : public FS
{
   public:
    LittleFSClass() {}
};

// --- ModbusMaster ---
class ModbusMaster
{
   public:
    bool begin(int baud)
    {
        return true;
    }
    bool beginTransmission(uint8_t slave)
    {
        return true;
    }
    bool endTransmission()
    {
        return true;
    }
    uint8_t requestFrom(uint8_t slave, uint8_t count)
    {
        return count;
    }
    uint8_t available()
    {
        return 0;
    }
    uint8_t read()
    {
        return 0;
    }
    uint8_t write(uint8_t data)
    {
        return 1;
    }
    uint8_t write(uint8_t* data, uint8_t length)
    {
        return length;
    }
    uint8_t readInputRegisters(uint8_t slave, uint16_t address, uint8_t count)
    {
        return count;
    }
    uint8_t readHoldingRegisters(uint8_t slave, uint16_t address, uint8_t count)
    {
        return count;
    }
    uint8_t writeSingleRegister(uint8_t slave, uint16_t address, uint16_t value)
    {
        return 1;
    }
    uint8_t writeMultipleRegisters(uint8_t slave, uint16_t address, uint8_t count)
    {
        return count;
    }
    uint16_t getResponseBuffer(uint8_t index)
    {
        return 0;
    }
    void setTransmitBuffer(uint8_t index, uint16_t value) {}
    void clearTransmitBuffer() {}
    void clearResponseBuffer() {}
};

// --- CSRFProtection ---
class CSRFProtection
{
   public:
    static String generateToken()
    {
        return String("test_token");
    }
    static bool validateToken(const String& token)
    {
        return true;
    }
    static String getTokenFromRequest(const WebServer&)
    {
        return "test_token";
    }
};

// --- PubSubClient ---
class PubSubClient
{
   public:
    bool connect(const char* id)
    {
        return true;
    }
    bool publish(const char* topic, const char* payload)
    {
        return true;
    }
    bool subscribe(const char* topic)
    {
        return true;
    }
    bool loop()
    {
        return true;
    }
    bool connected()
    {
        return true;
    }
    void disconnect() {}
};

// --- HTTPClient ---
class HTTPClient
{
   public:
    bool begin(const char* url)
    {
        return true;
    }
    int GET()
    {
        return 200;
    }
    int POST(const char* payload)
    {
        return 200;
    }
    String getString()
    {
        return String("");
    }
    void end() {}
};

// --- ArduinoJson ---
class JsonDocument
{
   public:
    JsonDocument() {}
    ~JsonDocument() {}
    bool isNull() const
    {
        return true;
    }
    void clear() {}
};

class JsonArray
{
   public:
    JsonArray() {}
    int size() const
    {
        return 0;
    }
    String get(int index) const
    {
        return String("");
    }
    void add(const String& value) {}
    void add(int value) {}
    void add(float value) {}
    void add(bool value) {}
    JsonObject createNestedObject()
    {
        return JsonObject();
    }
    JsonArray createNestedArray()
    {
        return JsonArray();
    }
};

// --- EEPROMClass ---
class EEPROMClass
{
   public:
    uint8_t read(int address)
    {
        return 0;
    }
    void write(int address, uint8_t value) {}
    bool commit()
    {
        return true;
    }
    void begin(size_t size) {}
    void end() {}
};

// Глобальные экземпляры
extern SerialClass Serial;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;
extern WiFiClass WiFi;
extern WebServer server;
extern LittleFSClass LittleFS;
extern ModbusMaster modbus;
extern PubSubClient mqtt;
extern HTTPClient http;
extern EEPROMClass EEPROM;

// Arduino функции
unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void delayMicroseconds(unsigned long us);
void digitalWrite(uint8_t pin, uint8_t state);
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int value);
void yield();
void esp_restart();

#endif  // ESP32_STUBS_H
