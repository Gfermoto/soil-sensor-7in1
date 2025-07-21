#include "ota_verification.h"
#include <sstream>
#include <iomanip>
#include <strings.h>

namespace
{
bool isSha256HexEqual(const uint8_t* calcDigest, const char* expectedHex)
{
    std::ostringstream oss;
    for (int i = 0; i < 32; ++i)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(calcDigest[i]);
    }
    std::string hexString = oss.str();
    return strcasecmp(hexString.c_str(), expectedHex) == 0;
}
}  // namespace

bool verifySha256Digest(const uint8_t* calcDigest, const char* expectedHex)
{
    return isSha256HexEqual(calcDigest, expectedHex);
} 