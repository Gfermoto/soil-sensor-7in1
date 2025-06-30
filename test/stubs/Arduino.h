#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdint.h>
#include <cctype>
#include <string>

class String : public std::string
{
   public:
    using std::string::string;  // inherit constructors
    String(const std::string& s) : std::string(s) {}
    String(const char* s) : std::string(s) {}
    String(long value) : std::string(std::to_string(value)) {}
    String(unsigned long value) : std::string(std::to_string(value)) {}
    String(int value) : std::string(std::to_string(value)) {}
    int indexOf(char c) const
    {
        auto pos = this->find(c);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }
    // For compatibility with Arduino int indexOf(const String& substr)
    int indexOf(const String& substr) const
    {
        auto pos = this->find(substr);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }
    char charAt(size_t index) const
    {
        return this->at(index);
    }
    String substring(size_t start, size_t end) const
    {
        return this->substr(start, end - start);
    }
    int toInt() const
    {
        return std::stoi(*this);
    }
    bool isEmpty() const { return this->empty(); }
};

using UBaseType_t = unsigned int;

using millis_t = unsigned long;

inline bool isAlphaNumeric(char c)
{
    return std::isalnum(static_cast<unsigned char>(c));
}

class SerialStub
{
   public:
    void begin(unsigned long) {}
    template <typename T>
    void print(const T&)
    {
    }
    template <typename T>
    void println(const T&)
    {
    }
};

static SerialStub Serial;

#include <chrono>
#include <thread>
inline void delay(millis_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline unsigned long millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    return (unsigned long)std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}