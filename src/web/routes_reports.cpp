#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структуры для отчётов
struct TestSummary
{
    int total;
    int passed;
    int failed;
    float success_rate;
    String timestamp;
};

struct TechnicalDebtMetrics
{
    int code_smells;
    int duplicated_lines;
    int complexity_issues;
    int security_hotspots;
    String maintainability_rating;
    float debt_ratio;
    float coverage;
};

namespace
{
TestSummary lastTestSummary;
TechnicalDebtMetrics lastTechDebt;
unsigned long lastReportUpdate = 0;
const unsigned long REPORT_CACHE_TTL = REPORT_CACHE_TTL_MS;

bool loadTestReport(const String& filename, TestSummary& summary)
{
    if (!SPIFFS.exists(filename))
    {
        // Заглушка с демо-данными
        summary.total = TESTS_TOTAL_COUNT;
        summary.passed = TESTS_PASSED_COUNT;
        summary.failed = 0;
        summary.success_rate = TEST_SUCCESS_RATE_MAX;
        summary.timestamp = TEST_TIMESTAMP_EXAMPLE;
        return true;
    }

    File file = SPIFFS.open(filename, "r");
    if (!file)
    {
        return false;
    }

    StaticJsonDocument<REPORTS_JSON_DOC_SIZE> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        return false;
    }

    summary.total = doc["summary"]["total"] | 0;
    summary.passed = doc["summary"]["passed"] | 0;
    summary.failed = doc["summary"]["failed"] | 0;
    summary.success_rate = doc["summary"]["success_rate"] | 0.0;
    summary.timestamp = doc["timestamp"] | "Unknown";

    return true;
}

bool loadTechDebtReport(const String& filename, TechnicalDebtMetrics& debt)
{
    if (!SPIFFS.exists(filename))
    {
        // Заглушка с демо-данными
        debt.code_smells = TECH_DEBT_CODE_SMELLS;
        debt.duplicated_lines = TECH_DEBT_DUPLICATED_LINES;
        debt.complexity_issues = 6;
        debt.security_hotspots = TECH_DEBT_SECURITY_HOTSPOTS;
        debt.maintainability_rating = "D";
        debt.debt_ratio = TECH_DEBT_DEBT_RATIO;
        debt.coverage = TECH_DEBT_COVERAGE;
        return true;
    }

    File file = SPIFFS.open(filename, "r");
    if (!file)
    {
        return false;
    }

    StaticJsonDocument<REPORTS_JSON_DOC_LARGE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        return false;
    }

    debt.code_smells = doc["metrics"]["code_smells"] | 0;
    debt.duplicated_lines = doc["metrics"]["duplicated_lines"] | 0;
    debt.complexity_issues = doc["metrics"]["complexity_issues"] | 0;
    debt.security_hotspots = doc["metrics"]["security_hotspots"] | 0;
    debt.maintainability_rating = doc["metrics"]["maintainability_rating"] | "Unknown";
    debt.debt_ratio = doc["metrics"]["debt_ratio"] | 0.0;
    debt.coverage = doc["metrics"]["coverage"] | 0.0;

    return true;
}

void updateReportsCache()
{
    const unsigned long now = millis();
    if (now - lastReportUpdate < REPORT_CACHE_TTL)
    {
        return;  // Кэш ещё актуален
    }

    loadTestReport("/reports/test-summary.json", lastTestSummary);
    loadTechDebtReport("/reports/technical-debt.json", lastTechDebt);
    lastReportUpdate = now;
}

String generateReportsHTML()
{
    updateReportsCache();
    const String html = R"(
<!DOCTYPE html>
<html lang=\"en\">
<head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>[TEST] Reports - JXCT</title>
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; line-height: 1.6; margin: 0; padding: 20px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; }
        .container { max-width: 1200px; margin: 0 auto; background: white; border-radius: 10px; box-shadow: 0 0 20px rgba(0,0,0,0.1); overflow: hidden; }
        .header { background: linear-gradient(135deg, #2e7d32 0%, #388e3c 100%); color: white; padding: 2rem; text-align: center; }
        .header h1 { margin: 0; font-size: 2.5em; font-weight: 300; }
        .nav { background: #f8f9fa; padding: 1rem 2rem; border-bottom: 1px solid #e9ecef; }
        .nav a { color: #495057; text-decoration: none; margin-right: 20px; font-weight: 500; }
        .nav a:hover { color: #2e7d32; }
        .content { padding: 2rem; }
        .metrics-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin-bottom: 30px; }
        .metric-card { background: #f8f9fa; border-left: 4px solid #2e7d32; padding: 20px; border-radius: 5px; }
        .metric-card h3 { margin: 0 0 10px 0; color: #2e7d32; }
        .metric-value { font-size: 2em; font-weight: bold; color: #495057; }
        .metric-label { color: #6c757d; font-size: 0.9em; }
        .status-indicator { display: inline-block; width: 12px; height: 12px; border-radius: 50%; margin-right: 8px; }
        .status-success { background-color: #28a745; }
        .status-warning { background-color: #ffc107; }
        .status-danger { background-color: #dc3545; }
        .progress-bar { width: 100%; height: 20px; background-color: #e9ecef; border-radius: 10px; overflow: hidden; margin: 10px 0; }
        .progress-fill { height: 100%; background: linear-gradient(90deg, #28a745 0%, #20c997 100%); transition: width 0.3s ease; }
        .tech-debt-section { background: #fff3cd; border: 1px solid #ffeaa7; border-radius: 5px; padding: 20px; margin-top: 20px; }
        .tech-debt-section h3 { color: #856404; margin-top: 0; }
        .refresh-btn { background: #2e7d32; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; font-size: 14px; margin-bottom: 20px; }
        .refresh-btn:hover { background: #1b5e20; }
        .footer { background: #f8f9fa; text-align: center; padding: 1rem; border-top: 1px solid #e9ecef; color: #6c757d; }
    </style>
</head>
<body>
    <div class=\"container\">
        <div class=\"header\">
            <h1>[TEST] Reports</h1>
            <p>JXCT Code Quality Monitoring System</p>
        </div>
        <div class=\"nav\">
            <a href=\"/\">[HOME] Main</a>
            <a href=\"/status\">[STATUS] Status</a>
            <a href=\"/reports/dashboard\">[CHART] Dashboard</a>
            <a href=\"/api/reports/full\">[FILE] JSON API</a>
        </div>
        <div class=\"content\">
            <button class=\"refresh-btn\" onclick=\"location.reload()\">[REFRESH] Refresh Reports</button>
            <h2>[CHART] Test Summary</h2>
            <div class=\"metrics-grid\">
                <div class=\"metric-card\">
                    <h3><span class=\"status-indicator status-success\"></span>Total Tests</h3>
                    <div class=\"metric-value\">" + String(lastTestSummary.total) + R"(</div>
                    <div class=\"metric-label\">Tests Executed</div>
                </div>
                <div class=\"metric-card\">
                    <h3><span class=\"status-indicator status-success\"></span>Passed</h3>
                    <div class=\"metric-value\">" + String(lastTestSummary.passed) + R"(</div>
                    <div class=\"metric-label\">Tests Passed</div>
                </div>
                <div class=\"metric-card\">
                    <h3><span class=\"status-indicator " + (lastTestSummary.failed == 0 ? "status-success" : "status-danger") + R"(\"></span>Failed</h3>
                    <div class=\"metric-value\">" + String(lastTestSummary.failed) + R"(</div>
                    <div class=\"metric-label\">Tests Failed</div>
                </div>
                <div class=\"metric-card\">
                    <h3><span class=\"status-indicator " + (lastTestSummary.success_rate >= 90 ? "status-success" : "status-warning") + R"(\"></span>Success Rate</h3>
                    <div class=\"metric-value\">" + String(lastTestSummary.success_rate, 1) + R"(%</div>
                    <div class=\"metric-label\">Success Percentage</div>
                    <div class=\"progress-bar\">
                        <div class=\"progress-fill\" style=\"width: " + String(lastTestSummary.success_rate) + R"(%;\"></div>
                    </div>
                </div>
            </div>
            <div class=\"tech-debt-section\">
                <h3>[WARNING] Technical Debt</h3>
                <div class=\"metrics-grid\">
                    <div class=\"metric-card\">
                        <h3>[SEARCH] Code Smells</h3>
                        <div class=\"metric-value\">" + String(lastTechDebt.code_smells) + R"(</div>
                        <div class=\"metric-label\">Code Quality Issues</div>
                    </div>
                    <div class=\"metric-card\">
                        <h3>[COPY] Duplicated Lines</h3>
                        <div class=\"metric-value\">" + String(lastTechDebt.duplicated_lines) + R"(</div>
                        <div class=\"metric-label\">Duplicated Code Lines</div>
                    </div>
                    <div class=\"metric-card\">
                        <h3>[LOOP] Complexity</h3>
                        <div class=\"metric-value\">" + String(lastTechDebt.complexity_issues) + R"(</div>
                        <div class=\"metric-label\">High Complexity Functions</div>
                    </div>
                    <div class=\"metric-card\">
                        <h3>[LOCK] Security</h3>
                        <div class=\"metric-value\">" + String(lastTechDebt.security_hotspots) + R"(</div>
                        <div class=\"metric-label\">Security Vulnerabilities</div>
                    </div>
                </div>
                <div style=\"margin-top: 20px;\">
                    <strong>Maintainability Rating:</strong> <span style=\"color: #856404; font-weight: bold;\">" + lastTechDebt.maintainability_rating + R"(</span><br>
                    <strong>Debt Ratio:</strong> <span style=\"color: #856404; font-weight: bold;\">" + String(lastTechDebt.debt_ratio, 2) + R"(%</span><br>
                    <strong>Test Coverage:</strong> <span style=\"color: #856404; font-weight: bold;\">" + String(lastTechDebt.coverage, 1) + R"(%</span>
                </div>
            </div>
            <div style=\"margin-top: 30px; padding: 20px; background: #e8f5e8; border-radius: 5px;\">
                <h3>[CALENDAR] Last Update</h3>
                <p><strong>Report from:</strong> " + lastTestSummary.timestamp + R"(</p>
                <p><em>Reports are updated automatically with each CI/CD pipeline run</em></p>
            </div>
        </div>
        <div class=\"footer\">
            <p>© 2025 JXCT Development Team | Code Quality Monitoring System vяю3 . 6 . 8 
 
 </p>
        </div>
    </div>
    <script>
        setInterval(() => { location.reload(); }, 300000);
    </script>
</body>
</html>
)";
    return html;
}

String generateReportsDashboardHTML()
{
    updateReportsCache();

    const String statusIcon = lastTestSummary.success_rate >= 90 ? "[OK]" : "[WARNING]";
    const String statusColor = lastTestSummary.success_rate >= 90 ? "#28a745" : "#ffc107";

    const String html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>[CHART] Quality Dashboard - JXCT</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 20px;
            background: #f8f9fa;
        }
        .dashboard {
            max-width: 800px;
            margin: 0 auto;
        }
        .summary-card {
            background: white;
            border-radius: 10px;
            padding: 30px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            text-align: center;
            margin-bottom: 20px;
        }
        .status-icon {
            font-size: 4em;
            margin-bottom: 20px;
        }
        .status-text {
            font-size: 1.5em;
            font-weight: bold;
            color: )" + statusColor +
                        R"(;
            margin-bottom: 10px;
        }
        .quick-stats {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 15px;
            margin-top: 20px;
        }
        .stat-item {
            text-align: center;
        }
        .stat-value {
            font-size: 2em;
            font-weight: bold;
            color: #495057;
        }
        .stat-label {
            color: #6c757d;
            font-size: 0.9em;
        }
        .actions {
            text-align: center;
            margin-top: 20px;
        }
        .btn {
            display: inline-block;
            padding: 10px 20px;
            margin: 0 10px;
            background: #2e7d32;
            color: white;
            text-decoration: none;
            border-radius: 5px;
            font-weight: 500;
        }
        .btn:hover {
            background: #1b5e20;
        }
        .btn-secondary {
            background: #6c757d;
        }
        .btn-secondary:hover {
            background: #495057;
        }
    </style>
</head>
<body>
    <div class="dashboard">
        <div class="summary-card">
            <div class="status-icon">)" +
                        statusIcon + R"(</div>
            <div class="status-text">
                )" + (lastTestSummary.success_rate >= 90 ? "System is stable" : "Attention required") +
                        R"(
            </div>
            <div style="color: #6c757d;">
                Last testing: )" +
                        lastTestSummary.timestamp + R"(
            </div>

            <div class="quick-stats">
                <div class="stat-item">
                    <div class="stat-value">)" +
                        String(lastTestSummary.success_rate, 0) + R"(%</div>
                    <div class="stat-label">Success Rate</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">)" +
                        String(lastTestSummary.total) + R"(</div>
                    <div class="stat-label">Tests</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">)" +
                        String(lastTechDebt.code_smells) + R"(</div>
                    <div class="stat-label">Code Smells</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">)" +
                        String(lastTechDebt.coverage, 0) + R"(%</div>
                    <div class="stat-label">Coverage</div>
                </div>
            </div>

            <div class="actions">
                <a href="/reports" class="btn">[CHART] Detailed Reports</a>
                <a href="/api/reports/full" class="btn btn-secondary">[FILE] JSON API</a>
                <a href="/" class="btn btn-secondary">[HOME] Main</a>
            </div>
        </div>
    </div>
</body>
</html>
)";

    return html;
}
}  // namespace

void setupReportsRoutes()
{
    logInfo("🧪 Настройка маршрутов отчётов тестирования...");

    // API для получения отчётов в JSON формате
    webServer.on("/api/reports/test-summary", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/api/reports/test-summary", webServer.client().remoteIP().toString());

                     updateReportsCache();

                     StaticJsonDocument<JSON_DOC_SMALL> doc;
                     doc["timestamp"] = lastTestSummary.timestamp;
                     doc["total"] = lastTestSummary.total;
                     doc["passed"] = lastTestSummary.passed;
                     doc["failed"] = lastTestSummary.failed;
                     doc["success_rate"] = lastTestSummary.success_rate;

                     String json;
                     serializeJson(doc, json);
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
                 });

    webServer.on("/api/reports/technical-debt", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/api/reports/technical-debt", webServer.client().remoteIP().toString());

                     updateReportsCache();

                     StaticJsonDocument<JSON_DOC_MEDIUM> doc;
                     doc["code_smells"] = lastTechDebt.code_smells;
                     doc["duplicated_lines"] = lastTechDebt.duplicated_lines;
                     doc["complexity_issues"] = lastTechDebt.complexity_issues;
                     doc["security_hotspots"] = lastTechDebt.security_hotspots;
                     doc["maintainability_rating"] = lastTechDebt.maintainability_rating;
                     doc["debt_ratio"] = lastTechDebt.debt_ratio;
                     doc["coverage"] = lastTechDebt.coverage;

                     String json;
                     serializeJson(doc, json);
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
                 });

    // API для получения полных отчётов
    webServer.on("/api/reports/full", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/api/reports/full", webServer.client().remoteIP().toString());

                     updateReportsCache();

                     StaticJsonDocument<JSON_DOC_LARGE> doc;

                     // Тестовые метрики
                     doc["test_summary"]["total"] = lastTestSummary.total;
                     doc["test_summary"]["passed"] = lastTestSummary.passed;
                     doc["test_summary"]["failed"] = lastTestSummary.failed;
                     doc["test_summary"]["success_rate"] = lastTestSummary.success_rate;
                     doc["test_summary"]["timestamp"] = lastTestSummary.timestamp;

                     // Технический долг
                     doc["technical_debt"]["code_smells"] = lastTechDebt.code_smells;
                     doc["technical_debt"]["duplicated_lines"] = lastTechDebt.duplicated_lines;
                     doc["technical_debt"]["complexity_issues"] = lastTechDebt.complexity_issues;
                     doc["technical_debt"]["security_hotspots"] = lastTechDebt.security_hotspots;
                     doc["technical_debt"]["maintainability_rating"] = lastTechDebt.maintainability_rating;
                     doc["technical_debt"]["debt_ratio"] = lastTechDebt.debt_ratio;
                     doc["technical_debt"]["coverage"] = lastTechDebt.coverage;

                     String json;
                     serializeJson(doc, json);
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
                 });

    // Веб-интерфейс для просмотра отчётов
    webServer.on("/reports", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/reports", webServer.client().remoteIP().toString());

                     const String html = generateReportsHTML();
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                 });

    // Дашборд отчётов (краткий обзор)
    webServer.on("/reports/dashboard", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/reports/dashboard", webServer.client().remoteIP().toString());

                     const String html = generateReportsDashboardHTML();
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                 });

    logSuccess("🧪 Маршруты отчётов тестирования настроены");
}
