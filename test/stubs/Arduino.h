#pragma once
#include <cstdint>
#include <cstddef>

inline unsigned long millis() { return 0; }
inline void delay(unsigned long) {}
// Здесь могут быть другие stub-функции, но без определения String 