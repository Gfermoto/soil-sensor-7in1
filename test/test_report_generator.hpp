#pragma once

/**
 * @file test_report_generator.hpp
 * @brief Генератор отчётов о тестировании
 * @version 1.0.0
 * @date 2025-01-22
 * @author EYERA Development Team
 */

#include "test_framework_config.hpp"
#include <memory>
#include <map>

namespace jxct::testing {

/**
 * @brief Генератор отчётов о тестировании
 * 
 * Класс отвечает за создание различных типов отчётов:
 * - XML (JUnit формат)
 * - HTML (интерактивный отчёт)
 * - JSON (для API интеграции)
 */
class TestReportGenerator {
public:
    /**
     * @brief Конструктор
     * @param config Конфигурация отчётов
     */
    explicit TestReportGenerator(const ReportConfig& config);
    
    /**
     * @brief Добавить результат теста
     * @param result Результат выполнения теста
     */
    void addTestResult(const TestResult& result);
    
    /**
     * @brief Сгенерировать все отчёты
     */
    void generateReports();
    
    /**
     * @brief Получить текущие метрики
     * @return Метрики качества
     */
    const QualityMetrics& getMetrics() const { return metrics_; }
    
    /**
     * @brief Установить метрики технического долга
     * @param codeSmells Количество code smells
     * @param duplicatedLines Количество дублированных строк
     * @param complexityIssues Проблемы сложности
     * @param securityHotspots Уязвимости безопасности
     */
    void setTechnicalDebtMetrics(int codeSmells, int duplicatedLines, 
                                int complexityIssues, int securityHotspots);
    
    /**
     * @brief Установить покрытие кода
     * @param coverage Процент покрытия кода
     */
    void setCodeCoverage(double coverage);

private:
    ReportConfig config_;
    QualityMetrics metrics_;
    std::vector<TestResult> results_;
    
    /**
     * @brief Создать отчёт
     * @return Структура отчёта
     */
    TestReport createReport();
    
    /**
     * @brief Сгенерировать XML отчёт (JUnit формат)
     * @param report Данные отчёта
     */
    void generateXMLReport(const TestReport& report);
    
    /**
     * @brief Сгенерировать HTML отчёт
     * @param report Данные отчёта
     */
    void generateHTMLReport(const TestReport& report);
    
    /**
     * @brief Сгенерировать JSON отчёт
     * @param report Данные отчёта
     */
    void generateJSONReport(const TestReport& report);
    
    // HTML генерация
    std::string generateHTMLHeader();
    std::string generateHTMLSummary(const TestReport& report);
    std::string generateHTMLMetrics(const TestReport& report);
    std::string generateHTMLTestResults(const TestReport& report);
    std::string generateHTMLTechnicalDebt(const TestReport& report);
    std::string generateHTMLFooter();
    
    /**
     * @brief Обновить метрики на основе результата теста
     * @param result Результат теста
     */
    void updateMetrics(const TestResult& result);
    
    /**
     * @brief Получить информацию об окружении
     * @return Строка с информацией об окружении
     */
    std::string getEnvironmentInfo();
    
    /**
     * @brief Получить хеш текущего коммита
     * @return Хеш коммита
     */
    std::string getGitCommit();
    
    /**
     * @brief Получить текущую ветку
     * @return Название ветки
     */
    std::string getGitBranch();
    
    /**
     * @brief Форматировать временную метку
     * @param tp Временная точка
     * @return Отформатированная строка
     */
    std::string formatTimestamp(const std::chrono::system_clock::time_point& tp);
    
    /**
     * @brief Преобразовать категорию теста в строку
     * @param category Категория теста
     * @return Строковое представление
     */
    std::string categoryToString(TestCategory category);
    
    /**
     * @brief Преобразовать приоритет теста в строку
     * @param priority Приоритет теста
     * @return Строковое представление
     */
    std::string priorityToString(TestPriority priority);
    
    /**
     * @brief Экранировать XML символы
     * @param input Входная строка
     * @return Экранированная строка
     */
    std::string escapeXML(const std::string& input);
    
    /**
     * @brief Сохранить исторические данные
     * @param report Отчёт для сохранения
     */
    void saveHistoricalData(const TestReport& report);
    
    /**
     * @brief Проанализировать регрессии
     * @param report Отчёт для анализа
     */
    void analyzeRegressions(TestReport& report);
};

} // namespace jxct::testing 