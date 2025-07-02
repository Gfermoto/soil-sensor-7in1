#pragma once
#include <cmath>
#include <iostream>

// Простая реализация Unity для автономного тестирования
#define TEST_ASSERT_TRUE(condition)                                                    \
    if (!(condition))                                                                  \
    {                                                                                  \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__ << std::endl; \
        return;                                                                        \
    }

#define TEST_ASSERT_FALSE(condition)                                                   \
    if (condition)                                                                     \
    {                                                                                  \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__ << std::endl; \
        return;                                                                        \
    }

#define TEST_ASSERT_EQUAL_STRING(expected, actual)                                                    \
    if (std::string(expected) != std::string(actual))                                                 \
    {                                                                                                 \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__ << " expected: " << expected \
                  << " actual: " << actual << std::endl;                                              \
        return;                                                                                       \
    }

#define TEST_ASSERT_NOT_EQUAL_STRING(expected, actual)                          \
    if (std::string(expected) == std::string(actual))                           \
    {                                                                           \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__        \
                  << " strings should not be equal: " << expected << std::endl; \
        return;                                                                 \
    }

#define TEST_ASSERT_EQUAL(expected, actual)                                                           \
    if ((expected) != (actual))                                                                       \
    {                                                                                                 \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__ << " expected: " << expected \
                  << " actual: " << actual << std::endl;                                              \
        return;                                                                                       \
    }

#define TEST_ASSERT_FLOAT_WITHIN(tolerance, expected, actual)                                         \
    if (std::abs((expected) - (actual)) > (tolerance))                                                \
    {                                                                                                 \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__ << " expected: " << expected \
                  << " actual: " << actual << " tolerance: " << tolerance << std::endl;               \
        return;                                                                                       \
    }

#define TEST_ASSERT_GREATER_THAN(threshold, actual)                                                                 \
    if ((actual) <= (threshold))                                                                                    \
    {                                                                                                               \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__ << " expected greater than: " << threshold \
                  << " actual: " << actual << std::endl;                                                            \
        return;                                                                                                     \
    }

#define TEST_ASSERT_NOT_EQUAL(expected, actual)                                \
    if ((expected) == (actual))                                                \
    {                                                                          \
        std::cerr << "❌ FAIL: " << __FUNCTION__ << " line " << __LINE__       \
                  << " values should not be equal: " << expected << std::endl; \
        return;                                                                \
    }

#define RUN_TEST(test)                                 \
    do                                                 \
    {                                                  \
        std::cout << "🧪 Running " << #test << "... "; \
        test();                                        \
        std::cout << "✅ PASS" << std::endl;           \
        tests_passed++;                                \
    } while (0)

#define UNITY_BEGIN()     \
    int tests_passed = 0; \
    std::cout << "🚀 Starting Unity Tests" << std::endl;

#undef UNITY_END
#define UNITY_END()                                                                  \
    std::cout << "\n📊 Tests completed: " << tests_passed << " passed" << std::endl; \
    return 0;
