#include "ota_verification.h"
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <mbedtls/sha256.h>
#include <cstring>
#include <string>
#include <WString.h> // Добавляю для String

namespace
{
bool isSha256HexEqual(const uint8_t* calcDigest, const char* expectedHex)
{
    if (calcDigest == nullptr || expectedHex == nullptr) {
        return false;
    }
    
    // Конвертируем вычисленный digest в hex строку
    String hexString = "";
    for (int i = 0; i < 32; i++) {
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", calcDigest[i]);
        hexString += hex;
    }
    
    // Сравниваем без учета регистра
    return strcasecmp(hexString.c_str(), expectedHex) == 0;
}

bool verifySha256Digest(const uint8_t* data, size_t dataLen, const uint8_t* expectedDigest) {
    if (data == nullptr || expectedDigest == nullptr || dataLen == 0) {
        return false;
    }
    
    // Вычисляем SHA256 от данных
    uint8_t calculatedDigest[32];
    mbedtls_sha256_context shaCtx;
    mbedtls_sha256_init(&shaCtx);
    
    // mbedtls функции возвращают void, поэтому просто вызываем их
    mbedtls_sha256_starts(&shaCtx, 0);
    mbedtls_sha256_update(&shaCtx, data, dataLen);
    mbedtls_sha256_finish(&shaCtx, calculatedDigest);
    
    mbedtls_sha256_free(&shaCtx);
    
    // Сравниваем с ожидаемым digest
    return memcmp(calculatedDigest, expectedDigest, 32) == 0;
}

}  // namespace

// Глобальная функция для совместимости с ota_manager.cpp
bool verifySha256Digest(const uint8_t* calcDigest, const char* expectedHex) {
    return isSha256HexEqual(calcDigest, expectedHex);
} 