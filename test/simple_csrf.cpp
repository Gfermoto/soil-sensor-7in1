#include "simple_csrf.h"
#include <chrono>
#include <mutex>
#include <random>

namespace
{
std::string currentToken;
std::chrono::steady_clock::time_point tokenTime;
const int TOKEN_LENGTH = 32;
const int TOKEN_LIFETIME_SEC = 1800;  // 30 минут
std::mutex mtx;
}  // namespace

std::string generateCSRFToken()
{
    std::lock_guard<std::mutex> lock(mtx);
    static const char hex[] = "0123456789abcdef";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::string token;
    for (int i = 0; i < TOKEN_LENGTH; ++i)
    {
        token += hex[dis(gen)];
    }
    currentToken = token;
    tokenTime = std::chrono::steady_clock::now();
    return token;
}

bool validateCSRFToken(const std::string& token)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (token.empty() || currentToken.empty()) return false;
    auto now = std::chrono::steady_clock::now();
    auto age = std::chrono::duration_cast<std::chrono::seconds>(now - tokenTime).count();
    if (age > TOKEN_LIFETIME_SEC)
    {
        currentToken.clear();
        return false;
    }
    return token == currentToken;
}
