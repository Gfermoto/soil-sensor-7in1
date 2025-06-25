#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Скрипт синхронизации отчётов тестирования с сайтом
Автоматически обновляет статические HTML страницы с последними отчётами
"""

import json
import os
import shutil
import datetime
from pathlib import Path
from typing import Dict, Any, Optional

class ReportSynchronizer:
    """Класс для синхронизации отчётов с сайтом"""
    
    def __init__(self, project_root: str):
        self.project_root = Path(project_root)
        self.reports_dir = self.project_root / "test_reports"
        self.site_dir = self.project_root / "site"
        self.site_reports_dir = self.site_dir / "reports"
        
        # Создаём директории если не существуют
        self.site_reports_dir.mkdir(exist_ok=True)
        
    def load_test_summary(self) -> Optional[Dict[str, Any]]:
        """Загружает сводку тестирования"""
        try:
            summary_file = self.reports_dir / "simple-test-report.json"
            if summary_file.exists():
                with open(summary_file, 'r', encoding='utf-8') as f:
                    return json.load(f)
        except Exception as e:
            print(f"⚠️ Ошибка загрузки отчёта тестирования: {e}")
        return None
    
    def load_technical_debt(self) -> Optional[Dict[str, Any]]:
        """Загружает отчёт технического долга"""
        try:
            debt_file = self.reports_dir / "technical-debt.json"
            if debt_file.exists():
                with open(debt_file, 'r', encoding='utf-8') as f:
                    return json.load(f)
        except Exception as e:
            print(f"⚠️ Ошибка загрузки отчёта технического долга: {e}")
        return None
    
    def generate_reports_index_html(self, test_summary: Dict[str, Any], tech_debt: Dict[str, Any]) -> str:
        """Генерирует главную страницу отчётов"""
        
        # Статус системы
        success_rate = test_summary.get('summary', {}).get('success_rate', 0)
        status_icon = "✅" if success_rate >= 90 else "⚠️" if success_rate >= 70 else "❌"
        status_text = "Отлично" if success_rate >= 90 else "Хорошо" if success_rate >= 70 else "Требует внимания"
        status_color = "#28a745" if success_rate >= 90 else "#ffc107" if success_rate >= 70 else "#dc3545"
        
        # Метрики технического долга
        metrics = tech_debt.get('metrics', {})
        code_smells = metrics.get('code_smells', 0)
        duplicated_lines = metrics.get('duplicated_lines', 0)
        complexity_issues = metrics.get('complexity_issues', 0)
        security_hotspots = metrics.get('security_hotspots', 0)
        maintainability_rating = metrics.get('maintainability_rating', 'Unknown')
        debt_ratio = metrics.get('debt_ratio', 0.0)
        coverage = metrics.get('coverage', 0.0)
        
        html = f"""<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🧪 Отчёты качества - JXCT Soil Sensor</title>
    <meta name="description" content="Автоматические отчёты качества кода и тестирования проекта JXCT">
    <link rel="icon" href="../assets/images/favicon.png">
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            line-height: 1.6;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }}
        .container {{
            max-width: 1200px;
            margin: 0 auto;
            background: white;
            box-shadow: 0 0 20px rgba(0,0,0,0.1);
            min-height: 100vh;
        }}
        .header {{
            background: linear-gradient(135deg, #2e7d32 0%, #388e3c 100%);
            color: white;
            padding: 2rem;
            text-align: center;
        }}
        .header h1 {{
            margin: 0;
            font-size: 2.5em;
            font-weight: 300;
        }}
        .nav {{
            background: #f8f9fa;
            padding: 1rem 2rem;
            border-bottom: 1px solid #e9ecef;
        }}
        .nav a {{
            color: #495057;
            text-decoration: none;
            margin-right: 20px;
            font-weight: 500;
        }}
        .nav a:hover {{
            color: #2e7d32;
        }}
        .status-banner {{
            background: {status_color};
            color: white;
            padding: 1rem 2rem;
            text-align: center;
            font-size: 1.2em;
            font-weight: bold;
        }}
        .content {{
            padding: 2rem;
        }}
        .metrics-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }}
        .metric-card {{
            background: #f8f9fa;
            border-left: 4px solid #2e7d32;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }}
        .metric-card h3 {{
            margin: 0 0 10px 0;
            color: #2e7d32;
            font-size: 1.1em;
        }}
        .metric-value {{
            font-size: 2.2em;
            font-weight: bold;
            color: #495057;
            margin-bottom: 5px;
        }}
        .metric-label {{
            color: #6c757d;
            font-size: 0.9em;
        }}
        .progress-bar {{
            width: 100%;
            height: 20px;
            background-color: #e9ecef;
            border-radius: 10px;
            overflow: hidden;
            margin: 10px 0;
        }}
        .progress-fill {{
            height: 100%;
            background: linear-gradient(90deg, #28a745 0%, #20c997 100%);
            transition: width 0.3s ease;
        }}
        .tech-debt-section {{
            background: #fff3cd;
            border: 1px solid #ffeaa7;
            border-radius: 8px;
            padding: 20px;
            margin-top: 30px;
        }}
        .tech-debt-section h3 {{
            color: #856404;
            margin-top: 0;
        }}
        .update-info {{
            background: #e8f5e8;
            border-radius: 8px;
            padding: 20px;
            margin-top: 30px;
        }}
        .footer {{
            background: #f8f9fa;
            text-align: center;
            padding: 2rem;
            border-top: 1px solid #e9ecef;
            color: #6c757d;
        }}
        .btn {{
            display: inline-block;
            padding: 10px 20px;
            margin: 5px;
            background: #2e7d32;
            color: white;
            text-decoration: none;
            border-radius: 5px;
            font-weight: 500;
            transition: background 0.3s;
        }}
        .btn:hover {{
            background: #1b5e20;
        }}
        .btn-secondary {{
            background: #6c757d;
        }}
        .btn-secondary:hover {{
            background: #495057;
        }}
        @media (max-width: 768px) {{
            .metrics-grid {{
                grid-template-columns: 1fr;
            }}
            .header h1 {{
                font-size: 2em;
            }}
            .content {{
                padding: 1rem;
            }}
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🧪 Отчёты качества кода</h1>
            <p>Автоматический мониторинг качества проекта JXCT Soil Sensor</p>
        </div>
        
        <div class="status-banner">
            {status_icon} Статус системы: {status_text} ({success_rate:.1f}% тестов прошло)
        </div>
        
        <div class="nav">
            <a href="../index.html">🏠 Главная</a>
            <a href="../html/index.html">📚 API Документация</a>
            <a href="dashboard.html">📈 Дашборд</a>
            <a href="test-summary.json">📄 JSON Отчёт</a>
        </div>
        
        <div class="content">
            <h2>📊 Сводка тестирования</h2>
            <div class="metrics-grid">
                <div class="metric-card">
                    <h3>📝 Всего тестов</h3>
                    <div class="metric-value">{test_summary.get('summary', {}).get('total', 0)}</div>
                    <div class="metric-label">Выполнено тестовых сценариев</div>
                </div>
                <div class="metric-card">
                    <h3>✅ Успешных</h3>
                    <div class="metric-value">{test_summary.get('summary', {}).get('passed', 0)}</div>
                    <div class="metric-label">Тестов завершилось успешно</div>
                </div>
                <div class="metric-card">
                    <h3>❌ Неудачных</h3>
                    <div class="metric-value">{test_summary.get('summary', {}).get('failed', 0)}</div>
                    <div class="metric-label">Тестов завершилось с ошибкой</div>
                </div>
                <div class="metric-card">
                    <h3>📈 Успешность</h3>
                    <div class="metric-value">{success_rate:.1f}%</div>
                    <div class="metric-label">Процент успешных тестов</div>
                    <div class="progress-bar">
                        <div class="progress-fill" style="width: {success_rate}%;"></div>
                    </div>
                </div>
            </div>
            
            <div class="tech-debt-section">
                <h3>⚠️ Технический долг</h3>
                <div class="metrics-grid">
                    <div class="metric-card">
                        <h3>🔍 Code Smells</h3>
                        <div class="metric-value">{code_smells}</div>
                        <div class="metric-label">Проблем качества кода</div>
                    </div>
                    <div class="metric-card">
                        <h3>📋 Дублирование</h3>
                        <div class="metric-value">{duplicated_lines}</div>
                        <div class="metric-label">Строк дублированного кода</div>
                    </div>
                    <div class="metric-card">
                        <h3>🔄 Сложность</h3>
                        <div class="metric-value">{complexity_issues}</div>
                        <div class="metric-label">Функций высокой сложности</div>
                    </div>
                    <div class="metric-card">
                        <h3>🔒 Безопасность</h3>
                        <div class="metric-value">{security_hotspots}</div>
                        <div class="metric-label">Потенциальных уязвимостей</div>
                    </div>
                </div>
                
                <div style="margin-top: 20px; padding: 15px; background: rgba(255,255,255,0.5); border-radius: 5px;">
                    <strong>🏆 Рейтинг поддерживаемости:</strong> <span style="color: #856404; font-weight: bold;">{maintainability_rating}</span><br>
                    <strong>💰 Долговой коэффициент:</strong> <span style="color: #856404; font-weight: bold;">{debt_ratio:.2f}%</span><br>
                    <strong>🎯 Покрытие тестами:</strong> <span style="color: #856404; font-weight: bold;">{coverage:.1f}%</span>
                </div>
            </div>
            
            <div class="update-info">
                <h3>📅 Информация об обновлении</h3>
                <p><strong>Последний отчёт:</strong> {test_summary.get('timestamp', 'Неизвестно')}</p>
                <p><strong>Версия проекта:</strong> {tech_debt.get('version', 'Unknown')}</p>
                <p><em>Отчёты обновляются автоматически при каждом запуске CI/CD pipeline</em></p>
                
                <div style="margin-top: 15px;">
                    <a href="test-summary.json" class="btn">📊 Полный отчёт тестов</a>
                    <a href="technical-debt.json" class="btn btn-secondary">⚠️ Технический долг</a>
                    <a href="../html/index.html" class="btn btn-secondary">📚 API Документация</a>
                </div>
            </div>
        </div>
        
        <div class="footer">
            <p>© 2025 JXCT Development Team | Система мониторинга качества кода v3.6.0</p>
            <p>Автоматически сгенерировано {datetime.datetime.now().strftime('%d.%m.%Y в %H:%M')}</p>
        </div>
    </div>
    
    <script>
        // Автообновление каждые 10 минут
        setTimeout(() => {{
            location.reload();
        }}, 600000);
        
        // Добавляем анимацию для прогресс-бара
        document.addEventListener('DOMContentLoaded', function() {{
            const progressBars = document.querySelectorAll('.progress-fill');
            progressBars.forEach(bar => {{
                const width = bar.style.width;
                bar.style.width = '0%';
                setTimeout(() => {{
                    bar.style.width = width;
                }}, 500);
            }});
        }});
    </script>
</body>
</html>"""
        return html
    
    def generate_dashboard_html(self, test_summary: Dict[str, Any], tech_debt: Dict[str, Any]) -> str:
        """Генерирует дашборд отчётов"""
        
        success_rate = test_summary.get('summary', {}).get('success_rate', 0)
        status_icon = "✅" if success_rate >= 90 else "⚠️" if success_rate >= 70 else "❌"
        status_text = "Система работает стабильно" if success_rate >= 90 else "Система требует внимания" if success_rate >= 70 else "Критические проблемы"
        status_color = "#28a745" if success_rate >= 90 else "#ffc107" if success_rate >= 70 else "#dc3545"
        
        metrics = tech_debt.get('metrics', {})
        
        html = f"""<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>📈 Дашборд качества - JXCT</title>
    <meta name="description" content="Краткий обзор качества кода проекта JXCT">
    <link rel="icon" href="../assets/images/favicon.png">
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 20px;
            background: #f8f9fa;
        }}
        .dashboard {{
            max-width: 900px;
            margin: 0 auto;
        }}
        .summary-card {{
            background: white;
            border-radius: 15px;
            padding: 40px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.1);
            text-align: center;
            margin-bottom: 20px;
        }}
        .status-icon {{
            font-size: 5em;
            margin-bottom: 20px;
        }}
        .status-text {{
            font-size: 1.8em;
            font-weight: bold;
            color: {status_color};
            margin-bottom: 15px;
        }}
        .quick-stats {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
            gap: 20px;
            margin-top: 30px;
        }}
        .stat-item {{
            text-align: center;
            padding: 20px;
            background: #f8f9fa;
            border-radius: 10px;
        }}
        .stat-value {{
            font-size: 2.5em;
            font-weight: bold;
            color: #495057;
            margin-bottom: 5px;
        }}
        .stat-label {{
            color: #6c757d;
            font-size: 0.95em;
            font-weight: 500;
        }}
        .actions {{
            text-align: center;
            margin-top: 30px;
        }}
        .btn {{
            display: inline-block;
            padding: 12px 25px;
            margin: 0 10px 10px 0;
            background: #2e7d32;
            color: white;
            text-decoration: none;
            border-radius: 8px;
            font-weight: 500;
            transition: all 0.3s;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }}
        .btn:hover {{
            background: #1b5e20;
            transform: translateY(-2px);
            box-shadow: 0 4px 10px rgba(0,0,0,0.2);
        }}
        .btn-secondary {{
            background: #6c757d;
        }}
        .btn-secondary:hover {{
            background: #495057;
        }}
        .timestamp {{
            color: #6c757d;
            font-size: 0.9em;
            margin-top: 15px;
        }}
        @media (max-width: 768px) {{
            .quick-stats {{
                grid-template-columns: repeat(2, 1fr);
            }}
            .status-text {{
                font-size: 1.4em;
            }}
            .summary-card {{
                padding: 20px;
            }}
        }}
    </style>
</head>
<body>
    <div class="dashboard">
        <div class="summary-card">
            <div class="status-icon">{status_icon}</div>
            <div class="status-text">{status_text}</div>
            <div class="timestamp">
                Последнее обновление: {test_summary.get('timestamp', 'Неизвестно')}
            </div>
            
            <div class="quick-stats">
                <div class="stat-item">
                    <div class="stat-value">{success_rate:.0f}%</div>
                    <div class="stat-label">Успешность тестов</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">{test_summary.get('summary', {}).get('total', 0)}</div>
                    <div class="stat-label">Всего тестов</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">{metrics.get('code_smells', 0)}</div>
                    <div class="stat-label">Code Smells</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">{metrics.get('coverage', 0):.0f}%</div>
                    <div class="stat-label">Покрытие кода</div>
                </div>
            </div>
            
            <div class="actions">
                <a href="index.html" class="btn">📊 Подробные отчёты</a>
                <a href="test-summary.json" class="btn btn-secondary">📄 JSON API</a>
                <a href="../index.html" class="btn btn-secondary">🏠 Главная</a>
            </div>
        </div>
    </div>
    
    <script>
        // Автообновление каждые 5 минут
        setTimeout(() => {{
            location.reload();
        }}, 300000);
    </script>
</body>
</html>"""
        return html
    
    def sync_reports(self) -> bool:
        """Основная функция синхронизации отчётов"""
        try:
            print("🔄 Начинаем синхронизацию отчётов с сайтом...")
            
            # Загружаем отчёты
            test_summary = self.load_test_summary()
            tech_debt = self.load_technical_debt()
            
            if not test_summary or not tech_debt:
                print("❌ Не удалось загрузить отчёты")
                return False
            
            # Копируем JSON файлы
            if (self.reports_dir / "simple-test-report.json").exists():
                shutil.copy2(
                    self.reports_dir / "simple-test-report.json",
                    self.site_reports_dir / "test-summary.json"
                )
                print("✅ Скопирован отчёт тестирования")
            
            if (self.reports_dir / "technical-debt.json").exists():
                shutil.copy2(
                    self.reports_dir / "technical-debt.json",
                    self.site_reports_dir / "technical-debt.json"
                )
                print("✅ Скопирован отчёт технического долга")
            
            # Генерируем HTML страницы
            reports_html = self.generate_reports_index_html(test_summary, tech_debt)
            with open(self.site_reports_dir / "index.html", 'w', encoding='utf-8') as f:
                f.write(reports_html)
            print("✅ Сгенерирована главная страница отчётов")
            
            dashboard_html = self.generate_dashboard_html(test_summary, tech_debt)
            with open(self.site_reports_dir / "dashboard.html", 'w', encoding='utf-8') as f:
                f.write(dashboard_html)
            print("✅ Сгенерирован дашборд отчётов")
            
            print("🎉 Синхронизация отчётов завершена успешно!")
            return True
            
        except Exception as e:
            print(f"❌ Ошибка синхронизации: {e}")
            return False

def main():
    """Главная функция"""
    import sys
    
    # Определяем корневую директорию проекта
    if len(sys.argv) > 1:
        project_root = sys.argv[1]
    else:
        project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    
    print(f"📁 Корневая директория проекта: {project_root}")
    
    # Создаём синхронизатор и запускаем
    synchronizer = ReportSynchronizer(project_root)
    success = synchronizer.sync_reports()
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main() 