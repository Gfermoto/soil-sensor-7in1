#pragma once

/**
 * @file test_framework_config.hpp
 * @brief Конфигурация системы тестирования JXCT
 * @version 1.0.0
 * @date 2025-01-22
 * @author EYERA Development Team
 */

#include <chrono>
#include <string>
#include <vector>

namespace jxct::testing
{

/**
 * @brief Конфигурация системы отчётов
 */
struct ReportConfig
{
    std::string outputDir = "test_reports";
    std::string projectName = "JXCT Soil Sensor";
    std::string version = "3.6.0";
    bool generateXML = true;
    bool generateHTML = true;
    bool generateJSON = true;
    bool includeCodeCoverage = true;
    bool includePerformanceMetrics = true;
    bool includeTechnicalDebt = true;
};

/**
 * @brief Метрики качества кода
 */
struct QualityMetrics
{
    double codeCoverage = 0.0;
    int totalTests = 0;
    int passedTests = 0;
    int failedTests = 0;
    int skippedTests = 0;
    std::chrono::milliseconds executionTime{0};
    size_t memoryUsage = 0;

    // Технический долг
    int codeSmells = 0;
    int duplicatedLines = 0;
    int complexityIssues = 0;
    int securityHotspots = 0;
};

/**
 * @brief Категории тестов
 */
enum class TestCategory
{
    UNIT,         // Юнит-тесты
    INTEGRATION,  // Интеграционные тесты
    SYSTEM,       // Системные тесты
    PERFORMANCE,  // Тесты производительности
    SECURITY,     // Тесты безопасности
    API,          // API тесты
    HARDWARE      // Тесты аппаратной части
};

/**
 * @brief Приоритет теста
 */
enum class TestPriority
{
    CRITICAL,  // Критичные тесты (блокируют релиз)
    HIGH,      // Высокий приоритет
    MEDIUM,    // Средний приоритет
    LOW        // Низкий приоритет
};

/**
 * @brief Информация о тесте
 */
struct TestInfo
{
    std::string name;
    std::string description;
    TestCategory category;
    TestPriority priority;
    std::vector<std::string> tags;
    std::string author;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point lastModified;
};

/**
 * @brief Результат выполнения теста
 */
struct TestResult
{
    TestInfo info;
    bool passed = false;
    std::chrono::milliseconds duration{0};
    std::string errorMessage;
    std::string stackTrace;
    size_t memoryUsed = 0;
    std::vector<std::string> warnings;

    // Метрики производительности
    double cpuUsage = 0.0;
    size_t peakMemoryUsage = 0;
    int networkCalls = 0;
    int fileOperations = 0;
};

/**
 * @brief Отчёт о выполнении тестов
 */
struct TestReport
{
    std::chrono::system_clock::time_point timestamp;
    std::string environment;
    std::string gitCommit;
    std::string gitBranch;
    QualityMetrics metrics;
    std::vector<TestResult> results;

    // Сравнение с предыдущим запуском
    bool hasRegression = false;
    std::vector<std::string> regressionDetails;
};

}  // namespace jxct::testing