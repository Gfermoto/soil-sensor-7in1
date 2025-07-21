#ifndef OTA_VERIFICATION_H
#define OTA_VERIFICATION_H

#include <cstdint>

// Безопасная проверка SHA256 digest
bool verifySha256Digest(const uint8_t* calcDigest, const char* expectedHex);

#endif  // OTA_VERIFICATION_H 