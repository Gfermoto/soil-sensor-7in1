#pragma once
class Preferences
{
   public:
    void begin(const char* ns = nullptr, bool readOnly = false) {}
    void putString(const char*, const char*) {}
    void end() {}
};
