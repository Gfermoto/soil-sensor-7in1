/**
 * @file test_report_generator.cpp
 * @brief Генератор отчётов о тестировании
 * @version 1.0.0
 * @date 2025-01-22
 * @author EYERA Development Team
 */

#include "test_report_generator.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <ctime>

#ifdef __linux__
#include <sys/resource.h>
#include <unistd.h>
#endif

namespace jxct::testing {

TestReportGenerator::TestReportGenerator(const ReportConfig& config)
    : config_(config) {
    // Создаём директорию для отчётов
    std::filesystem::create_directories(config_.outputDir);
    
    // Инициализируем метрики
    metrics_ = QualityMetrics{};
}

void TestReportGenerator::addTestResult(const TestResult& result) {
    results_.push_back(result);
    updateMetrics(result);
}

void TestReportGenerator::setTechnicalDebtMetrics(int codeSmells, int duplicatedLines, 
                                                 int complexityIssues, int securityHotspots) {
    metrics_.codeSmells = codeSmells;
    metrics_.duplicatedLines = duplicatedLines;
    metrics_.complexityIssues = complexityIssues;
    metrics_.securityHotspots = securityHotspots;
}

void TestReportGenerator::setCodeCoverage(double coverage) {
    metrics_.codeCoverage = coverage;
}

void TestReportGenerator::generateReports() {
    auto report = createReport();
    
    if (config_.generateXML) {
        generateXMLReport(report);
    }
    
    if (config_.generateHTML) {
        generateHTMLReport(report);
    }
    
    if (config_.generateJSON) {
        generateJSONReport(report);
    }
    
    // Сохраняем исторические данные
    saveHistoricalData(report);
    
    // Анализируем регрессии
    analyzeRegressions(report);
}

TestReport TestReportGenerator::createReport() {
    TestReport report;
    report.timestamp = std::chrono::system_clock::now();
    report.environment = getEnvironmentInfo();
    report.gitCommit = getGitCommit();
    report.gitBranch = getGitBranch();
    report.metrics = metrics_;
    report.results = results_;
    
    return report;
}

void TestReportGenerator::generateXMLReport(const TestReport& report) {
    std::string filename = config_.outputDir + "/test-results.xml";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create XML report file: " + filename);
    }
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<testsuites name=\"" << config_.projectName << "\" ";
    file << "tests=\"" << metrics_.totalTests << "\" ";
    file << "failures=\"" << metrics_.failedTests << "\" ";
    file << "skipped=\"" << metrics_.skippedTests << "\" ";
    file << "time=\"" << std::fixed << std::setprecision(3) 
         << metrics_.executionTime.count() / 1000.0 << "\">\n";
    
    // Группируем тесты по категориям
    std::map<TestCategory, std::vector<TestResult>> categorized;
    for (const auto& result : report.results) {
        categorized[result.info.category].push_back(result);
    }
    
    for (const auto& [category, tests] : categorized) {
        file << "  <testsuite name=\"" << categoryToString(category) << "\" ";
        file << "tests=\"" << tests.size() << "\">\n";
        
        for (const auto& test : tests) {
            file << "    <testcase name=\"" << escapeXML(test.info.name) << "\" ";
            file << "classname=\"" << escapeXML(test.info.description) << "\" ";
            file << "time=\"" << std::fixed << std::setprecision(3) 
                 << test.duration.count() / 1000.0 << "\"";
            
            if (!test.passed) {
                file << ">\n";
                file << "      <failure message=\"" << escapeXML(test.errorMessage) << "\">";
                file << escapeXML(test.stackTrace);
                file << "</failure>\n";
                file << "    </testcase>\n";
            } else {
                file << "/>\n";
            }
        }
        
        file << "  </testsuite>\n";
    }
    
    file << "</testsuites>\n";
    file.close();
}

void TestReportGenerator::generateHTMLReport(const TestReport& report) {
    std::string filename = config_.outputDir + "/test-report.html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create HTML report file: " + filename);
    }
    
    file << generateHTMLHeader();
    file << generateHTMLSummary(report);
    file << generateHTMLMetrics(report);
    file << generateHTMLTestResults(report);
    file << generateHTMLTechnicalDebt(report);
    file << generateHTMLFooter();
    
    file.close();
}

void TestReportGenerator::generateJSONReport(const TestReport& report) {
    std::string filename = config_.outputDir + "/test-report.json";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create JSON report file: " + filename);
    }
    
    file << "{\n";
    file << "  \"project\": \"" << config_.projectName << "\",\n";
    file << "  \"version\": \"" << config_.version << "\",\n";
    file << "  \"timestamp\": \"" << formatTimestamp(report.timestamp) << "\",\n";
    file << "  \"environment\": \"" << report.environment << "\",\n";
    file << "  \"git\": {\n";
    file << "    \"commit\": \"" << report.gitCommit << "\",\n";
    file << "    \"branch\": \"" << report.gitBranch << "\"\n";
    file << "  },\n";
    
    // Метрики
    file << "  \"metrics\": {\n";
    file << "    \"codeCoverage\": " << report.metrics.codeCoverage << ",\n";
    file << "    \"totalTests\": " << report.metrics.totalTests << ",\n";
    file << "    \"passedTests\": " << report.metrics.passedTests << ",\n";
    file << "    \"failedTests\": " << report.metrics.failedTests << ",\n";
    file << "    \"skippedTests\": " << report.metrics.skippedTests << ",\n";
    file << "    \"executionTime\": " << report.metrics.executionTime.count() << ",\n";
    file << "    \"memoryUsage\": " << report.metrics.memoryUsage << ",\n";
    file << "    \"technicalDebt\": {\n";
    file << "      \"codeSmells\": " << report.metrics.codeSmells << ",\n";
    file << "      \"duplicatedLines\": " << report.metrics.duplicatedLines << ",\n";
    file << "      \"complexityIssues\": " << report.metrics.complexityIssues << ",\n";
    file << "      \"securityHotspots\": " << report.metrics.securityHotspots << "\n";
    file << "    }\n";
    file << "  },\n";
    
    // Результаты тестов
    file << "  \"results\": [\n";
    for (size_t i = 0; i < report.results.size(); ++i) {
        const auto& result = report.results[i];
        file << "    {\n";
        file << "      \"name\": \"" << result.info.name << "\",\n";
        file << "      \"category\": \"" << categoryToString(result.info.category) << "\",\n";
        file << "      \"priority\": \"" << priorityToString(result.info.priority) << "\",\n";
        file << "      \"passed\": " << (result.passed ? "true" : "false") << ",\n";
        file << "      \"duration\": " << result.duration.count() << ",\n";
        file << "      \"memoryUsed\": " << result.memoryUsed << "\n";
        file << "    }";
        if (i < report.results.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";
    
    file.close();
}

std::string TestReportGenerator::generateHTMLHeader() {
    return R"(<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JXCT Test Report</title>
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 20px; background: #f5f5f5; }
        .container { max-width: 1200px; margin: 0 auto; background: white; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 8px 8px 0 0; }
        .header h1 { margin: 0; font-size: 2.5em; }
        .header p { margin: 10px 0 0 0; opacity: 0.9; }
        .content { padding: 30px; }
        .metrics { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin: 20px 0; }
        .metric-card { background: #f8f9fa; padding: 20px; border-radius: 6px; border-left: 4px solid #007bff; }
        .metric-value { font-size: 2em; font-weight: bold; color: #007bff; }
        .metric-label { color: #6c757d; margin-top: 5px; }
        .test-results { margin: 30px 0; }
        .test-item { border: 1px solid #dee2e6; border-radius: 6px; margin: 10px 0; overflow: hidden; }
        .test-header { padding: 15px; background: #f8f9fa; border-bottom: 1px solid #dee2e6; display: flex; justify-content: space-between; align-items: center; cursor: pointer; }
        .test-name { font-weight: bold; }
        .test-status { padding: 4px 12px; border-radius: 20px; font-size: 0.8em; font-weight: bold; }
        .status-pass { background: #d4edda; color: #155724; }
        .status-fail { background: #f8d7da; color: #721c24; }
        .test-details { padding: 15px; display: none; background: #fff; }
        .technical-debt { background: #fff3cd; border: 1px solid #ffeaa7; border-radius: 6px; padding: 20px; margin: 20px 0; }
        .debt-warning { color: #856404; }
        .progress-bar { width: 100%; height: 20px; background: #e9ecef; border-radius: 10px; overflow: hidden; margin: 10px 0; }
        .progress-fill { height: 100%; background: linear-gradient(90deg, #28a745, #20c997); transition: width 0.3s ease; }
        .category-badge { display: inline-block; padding: 2px 8px; border-radius: 12px; font-size: 0.7em; margin-right: 5px; }
        .badge-unit { background: #e3f2fd; color: #1976d2; }
        .badge-integration { background: #f3e5f5; color: #7b1fa2; }
        .badge-system { background: #e8f5e8; color: #388e3c; }
        .badge-performance { background: #fff3e0; color: #f57c00; }
        .badge-security { background: #ffebee; color: #d32f2f; }
    </style>
</head>
<body>
)";
}

std::string TestReportGenerator::generateHTMLSummary(const TestReport& report) {
    std::ostringstream html;
    
    html << "<div class=\"container\">\n";
    html << "  <div class=\"header\">\n";
    html << "    <h1>🧪 Отчёт о тестировании</h1>\n";
    html << "    <p>" << config_.projectName << " v" << config_.version << "</p>\n";
    html << "    <p>Сгенерирован: " << formatTimestamp(report.timestamp) << "</p>\n";
    html << "    <p>Окружение: " << report.environment << " | Коммит: " << report.gitCommit.substr(0, 8) << "</p>\n";
    
    // Прогресс-бар общего успеха
    double successRate = report.metrics.totalTests > 0 ? 
        (double)report.metrics.passedTests / report.metrics.totalTests * 100.0 : 0.0;
    
    html << "    <div class=\"progress-bar\">\n";
    html << "      <div class=\"progress-fill\" style=\"width: " << successRate << "%\"></div>\n";
    html << "    </div>\n";
    html << "    <p>Успешность: " << std::fixed << std::setprecision(1) << successRate << "%</p>\n";
    html << "  </div>\n";
    html << "  <div class=\"content\">\n";
    
    return html.str();
}

std::string TestReportGenerator::generateHTMLMetrics(const TestReport& report) {
    std::ostringstream html;
    
    html << "    <div class=\"metrics\">\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-value\">" << report.metrics.totalTests << "</div>\n";
    html << "        <div class=\"metric-label\">Всего тестов</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-value\" style=\"color: #28a745;\">" << report.metrics.passedTests << "</div>\n";
    html << "        <div class=\"metric-label\">✅ Прошли</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-value\" style=\"color: #dc3545;\">" << report.metrics.failedTests << "</div>\n";
    html << "        <div class=\"metric-label\">❌ Провалились</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-value\">" << std::fixed << std::setprecision(1) << report.metrics.codeCoverage << "%</div>\n";
    html << "        <div class=\"metric-label\">📊 Покрытие кода</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-value\">" << std::fixed << std::setprecision(2) << report.metrics.executionTime.count() / 1000.0 << "s</div>\n";
    html << "        <div class=\"metric-label\">⏱️ Время выполнения</div>\n";
    html << "      </div>\n";
    html << "      <div class=\"metric-card\">\n";
    html << "        <div class=\"metric-value\">" << report.metrics.memoryUsage / 1024 << "KB</div>\n";
    html << "        <div class=\"metric-label\">💾 Использование памяти</div>\n";
    html << "      </div>\n";
    html << "    </div>\n";
    
    return html.str();
}

std::string TestReportGenerator::generateHTMLTestResults(const TestReport& report) {
    std::ostringstream html;
    
    html << "    <div class=\"test-results\">\n";
    html << "      <h2>📋 Результаты тестов</h2>\n";
    
    for (const auto& result : report.results) {
        std::string categoryClass = "badge-unit";
        switch (result.info.category) {
            case TestCategory::INTEGRATION: categoryClass = "badge-integration"; break;
            case TestCategory::SYSTEM: categoryClass = "badge-system"; break;
            case TestCategory::PERFORMANCE: categoryClass = "badge-performance"; break;
            case TestCategory::SECURITY: categoryClass = "badge-security"; break;
            default: break;
        }
        
        html << "      <div class=\"test-item\">\n";
        html << "        <div class=\"test-header\">\n";
        html << "          <div>\n";
        html << "            <div class=\"test-name\">" << result.info.name << "</div>\n";
        html << "            <div>\n";
        html << "              <span class=\"category-badge " << categoryClass << "\">" << categoryToString(result.info.category) << "</span>\n";
        html << "              <small>" << priorityToString(result.info.priority) << " | " << result.duration.count() << "ms</small>\n";
        html << "            </div>\n";
        html << "          </div>\n";
        html << "          <div class=\"test-status " << (result.passed ? "status-pass" : "status-fail") << "\">\n";
        html << "            " << (result.passed ? "✅ PASS" : "❌ FAIL") << "\n";
        html << "          </div>\n";
        html << "        </div>\n";
        
        html << "        <div class=\"test-details\">\n";
        html << "          <p><strong>Описание:</strong> " << result.info.description << "</p>\n";
        if (!result.passed) {
            html << "          <p><strong>❌ Ошибка:</strong> " << result.errorMessage << "</p>\n";
        }
        html << "          <p><strong>⏱️ Время выполнения:</strong> " << result.duration.count() << "ms</p>\n";
        html << "          <p><strong>💾 Память:</strong> " << result.memoryUsed << " bytes</p>\n";
        html << "          <p><strong>👤 Автор:</strong> " << result.info.author << "</p>\n";
        html << "        </div>\n";
        
        html << "      </div>\n";
    }
    
    html << "    </div>\n";
    
    return html.str();
}

std::string TestReportGenerator::generateHTMLTechnicalDebt(const TestReport& report) {
    std::ostringstream html;
    
    if (config_.includeTechnicalDebt) {
        html << "    <div class=\"technical-debt\">\n";
        html << "      <h3 class=\"debt-warning\">⚠️ Технический долг</h3>\n";
        html << "      <p>Анализ качества кода и технического долга проекта</p>\n";
        html << "      <div class=\"metrics\">\n";
        html << "        <div class=\"metric-card\">\n";
        html << "          <div class=\"metric-value\" style=\"color: #856404;\">" << report.metrics.codeSmells << "</div>\n";
        html << "          <div class=\"metric-label\">🔍 Code Smells</div>\n";
        html << "        </div>\n";
        html << "        <div class=\"metric-card\">\n";
        html << "          <div class=\"metric-value\" style=\"color: #856404;\">" << report.metrics.duplicatedLines << "</div>\n";
        html << "          <div class=\"metric-label\">📋 Дублированные строки</div>\n";
        html << "        </div>\n";
        html << "        <div class=\"metric-card\">\n";
        html << "          <div class=\"metric-value\" style=\"color: #856404;\">" << report.metrics.complexityIssues << "</div>\n";
        html << "          <div class=\"metric-label\">🔄 Проблемы сложности</div>\n";
        html << "        </div>\n";
        html << "        <div class=\"metric-card\">\n";
        html << "          <div class=\"metric-value\" style=\"color: #dc3545;\">" << report.metrics.securityHotspots << "</div>\n";
        html << "          <div class=\"metric-label\">🔒 Уязвимости</div>\n";
        html << "        </div>\n";
        html << "      </div>\n";
        
        // Рекомендации по улучшению
        html << "      <h4>💡 Рекомендации</h4>\n";
        html << "      <ul>\n";
        if (report.metrics.codeSmells > 10) {
            html << "        <li>Высокое количество code smells - рекомендуется рефакторинг</li>\n";
        }
        if (report.metrics.duplicatedLines > 50) {
            html << "        <li>Много дублированного кода - выделить общие функции</li>\n";
        }
        if (report.metrics.complexityIssues > 0) {
            html << "        <li>Есть сложные функции - упростить логику</li>\n";
        }
        if (report.metrics.securityHotspots > 0) {
            html << "        <li>Обнаружены потенциальные уязвимости - провести аудит безопасности</li>\n";
        }
        if (report.metrics.codeCoverage < 80.0) {
            html << "        <li>Низкое покрытие тестами - добавить больше тестов</li>\n";
        }
        html << "      </ul>\n";
        html << "    </div>\n";
    }
    
    return html.str();
}

std::string TestReportGenerator::generateHTMLFooter() {
    return R"(  </div>
</div>

<script>
// Добавляем интерактивность
document.querySelectorAll('.test-header').forEach(header => {
    header.addEventListener('click', () => {
        const details = header.nextElementSibling;
        if (details && details.classList.contains('test-details')) {
            const isVisible = details.style.display === 'block';
            details.style.display = isVisible ? 'none' : 'block';
            
            // Анимация для стрелки
            header.style.transform = isVisible ? 'none' : 'scale(1.02)';
        }
    });
});

// Подсветка критичных тестов
document.querySelectorAll('.test-item').forEach(item => {
    const status = item.querySelector('.test-status');
    if (status && status.textContent.includes('FAIL')) {
        item.style.borderLeft = '4px solid #dc3545';
    }
});
</script>

</body>
</html>
)";
}

void TestReportGenerator::updateMetrics(const TestResult& result) {
    metrics_.totalTests++;
    
    if (result.passed) {
        metrics_.passedTests++;
    } else {
        metrics_.failedTests++;
    }
    
    metrics_.executionTime += result.duration;
    metrics_.memoryUsage += result.memoryUsed;
}

std::string TestReportGenerator::getEnvironmentInfo() {
    std::ostringstream info;
    
#ifdef __linux__
    info << "Linux";
#elif defined(_WIN32)
    info << "Windows";
#elif defined(__APPLE__)
    info << "macOS";
#else
    info << "Unknown";
#endif

#ifdef ARDUINO
    info << " (Arduino/ESP32)";
#else
    info << " (Native)";
#endif
    
    return info.str();
}

std::string TestReportGenerator::getGitCommit() {
    // В реальной реализации здесь будет вызов git команды
    const char* githubSha = std::getenv("GITHUB_SHA");
    if (githubSha) {
        return std::string(githubSha);
    }
    
    // Попытка получить через git команду (упрощённо)
    return "unknown-commit";
}

std::string TestReportGenerator::getGitBranch() {
    // В реальной реализации здесь будет вызов git команды
    const char* githubRef = std::getenv("GITHUB_REF_NAME");
    if (githubRef) {
        return std::string(githubRef);
    }
    
    return "unknown-branch";
}

std::string TestReportGenerator::formatTimestamp(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string TestReportGenerator::categoryToString(TestCategory category) {
    switch (category) {
        case TestCategory::UNIT: return "Unit";
        case TestCategory::INTEGRATION: return "Integration";
        case TestCategory::SYSTEM: return "System";
        case TestCategory::PERFORMANCE: return "Performance";
        case TestCategory::SECURITY: return "Security";
        case TestCategory::API: return "API";
        case TestCategory::HARDWARE: return "Hardware";
        default: return "Unknown";
    }
}

std::string TestReportGenerator::priorityToString(TestPriority priority) {
    switch (priority) {
        case TestPriority::CRITICAL: return "Critical";
        case TestPriority::HIGH: return "High";
        case TestPriority::MEDIUM: return "Medium";
        case TestPriority::LOW: return "Low";
        default: return "Unknown";
    }
}

std::string TestReportGenerator::escapeXML(const std::string& input) {
    std::string result = input;
    size_t pos = 0;
    
    while ((pos = result.find("&", pos)) != std::string::npos) {
        result.replace(pos, 1, "&amp;");
        pos += 5;
    }
    
    pos = 0;
    while ((pos = result.find("<", pos)) != std::string::npos) {
        result.replace(pos, 1, "&lt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = result.find(">", pos)) != std::string::npos) {
        result.replace(pos, 1, "&gt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = result.find("\"", pos)) != std::string::npos) {
        result.replace(pos, 1, "&quot;");
        pos += 6;
    }
    
    return result;
}

void TestReportGenerator::saveHistoricalData(const TestReport& report) {
    // Сохраняем исторические данные для анализа трендов
    std::string historyFile = config_.outputDir + "/test-history.json";
    
    // Здесь будет реализация сохранения истории для анализа трендов
    // Пока что создаём заглушку
    std::ofstream file(historyFile, std::ios::app);
    if (file.is_open()) {
        file << "// История тестов для " << formatTimestamp(report.timestamp) << "\n";
        file.close();
    }
}

void TestReportGenerator::analyzeRegressions(TestReport& report) {
    // Анализируем регрессии по сравнению с предыдущими запусками
    // Пока что простая проверка
    if (report.metrics.failedTests > 0) {
        report.hasRegression = true;
        report.regressionDetails.push_back("Обнаружены провалившиеся тесты");
    }
    
    if (report.metrics.codeCoverage < 70.0) {
        report.hasRegression = true;
        report.regressionDetails.push_back("Низкое покрытие кода");
    }
}

} // namespace jxct::testing 