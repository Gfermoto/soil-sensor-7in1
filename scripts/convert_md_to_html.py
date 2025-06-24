#!/usr/bin/env python3
"""
Скрипт для конвертации Markdown файлов в HTML с красивым дизайном
"""

import os
import re
import sys
from pathlib import Path

def create_html_template(title, content, nav_links):
    """Создает HTML шаблон с красивым дизайном"""
    
    html = f"""<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title}</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            line-height: 1.6;
            color: #333;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }}
        
        .container {{
            max-width: 1200px;
            margin: 0 auto;
            background: white;
            box-shadow: 0 0 20px rgba(0,0,0,0.1);
            border-radius: 10px;
            overflow: hidden;
        }}
        
        .header {{
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 2rem;
            text-align: center;
        }}
        
        .header h1 {{
            font-size: 2.5rem;
            margin-bottom: 0.5rem;
        }}
        
        .header .meta {{
            opacity: 0.9;
            font-size: 1.1rem;
        }}
        
        .nav {{
            background: #f8f9fa;
            padding: 1rem 2rem;
            border-bottom: 1px solid #e9ecef;
        }}
        
        .nav a {{
            color: #667eea;
            text-decoration: none;
            margin-right: 2rem;
            font-weight: 500;
        }}
        
        .nav a:hover {{
            color: #764ba2;
        }}
        
        .content {{
            padding: 2rem;
        }}
        
        .toc {{
            background: #f8f9fa;
            padding: 1.5rem;
            border-radius: 8px;
            margin-bottom: 2rem;
        }}
        
        .toc h2 {{
            color: #667eea;
            margin-bottom: 1rem;
        }}
        
        .toc ol {{
            list-style: none;
            counter-reset: toc-counter;
        }}
        
        .toc li {{
            counter-increment: toc-counter;
            margin-bottom: 0.5rem;
        }}
        
        .toc li::before {{
            content: counter(toc-counter) ". ";
            font-weight: bold;
            color: #667eea;
        }}
        
        .toc a {{
            color: #333;
            text-decoration: none;
        }}
        
        .toc a:hover {{
            color: #667eea;
        }}
        
        h2 {{
            color: #667eea;
            margin: 2rem 0 1rem 0;
            padding-bottom: 0.5rem;
            border-bottom: 2px solid #e9ecef;
        }}
        
        h3 {{
            color: #764ba2;
            margin: 1.5rem 0 1rem 0;
        }}
        
        h4 {{
            color: #764ba2;
            margin: 1rem 0 0.5rem 0;
        }}
        
        p {{
            margin-bottom: 1rem;
        }}
        
        ul, ol {{
            margin-bottom: 1rem;
            padding-left: 2rem;
        }}
        
        li {{
            margin-bottom: 0.5rem;
        }}
        
        code {{
            background: #f8f9fa;
            padding: 0.2rem 0.4rem;
            border-radius: 4px;
            font-family: 'Courier New', monospace;
        }}
        
        pre {{
            background: #f8f9fa;
            padding: 1rem;
            border-radius: 8px;
            overflow-x: auto;
            margin: 1rem 0;
        }}
        
        pre code {{
            background: none;
            padding: 0;
        }}
        
        table {{
            width: 100%;
            border-collapse: collapse;
            margin: 1rem 0;
        }}
        
        th, td {{
            padding: 0.75rem;
            text-align: left;
            border-bottom: 1px solid #e9ecef;
        }}
        
        th {{
            background: #667eea;
            color: white;
        }}
        
        tr:nth-child(even) {{
            background: #f8f9fa;
        }}
        
        .back-link {{
            display: inline-block;
            margin-top: 2rem;
            padding: 0.75rem 1.5rem;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            text-decoration: none;
            border-radius: 25px;
            transition: transform 0.2s;
        }}
        
        .back-link:hover {{
            transform: translateY(-2px);
        }}
        
        .feature-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 1.5rem;
            margin: 1.5rem 0;
        }}
        
        .feature-card {{
            background: #f8f9fa;
            padding: 1.5rem;
            border-radius: 8px;
            border-left: 4px solid #667eea;
        }}
        
        .feature-card h4 {{
            color: #667eea;
            margin-bottom: 1rem;
        }}
        
        blockquote {{
            background: #f8f9fa;
            border-left: 4px solid #667eea;
            padding: 1rem;
            margin: 1rem 0;
            font-style: italic;
        }}
        
        hr {{
            border: none;
            border-top: 2px solid #e9ecef;
            margin: 2rem 0;
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>{title}</h1>
            <div class="meta">
                <strong>Версия:</strong> 3.4.9 | 
                <strong>Дата:</strong> Июнь 2025 | 
                <strong>Автор:</strong> JXCT Development Team
            </div>
        </div>
        
        <div class="nav">
            {nav_links}
        </div>
        
        <div class="content">
            {content}
            <a href="../index.html" class="back-link">← Вернуться на главную</a>
        </div>
    </div>
</body>
</html>"""
    
    return html

def convert_markdown_to_html(md_content):
    """Конвертирует Markdown в HTML"""
    
    # Удаляем заголовок документа (первый h1)
    lines = md_content.split('\n')
    content_lines = []
    skip_until_content = True
    
    for line in lines:
        if skip_until_content and line.startswith('# '):
            skip_until_content = False
            continue
        if not skip_until_content:
            content_lines.append(line)
    
    content = '\n'.join(content_lines)
    
    # Конвертируем Markdown в HTML
    html_content = content
    
    # Заголовки
    html_content = re.sub(r'^### (.*?)$', r'<h3>\1</h3>', html_content, flags=re.MULTILINE)
    html_content = re.sub(r'^## (.*?)$', r'<h2>\1</h2>', html_content, flags=re.MULTILINE)
    html_content = re.sub(r'^#### (.*?)$', r'<h4>\1</h4>', html_content, flags=re.MULTILINE)
    
    # Списки
    html_content = re.sub(r'^- (.*?)$', r'<li>\1</li>', html_content, flags=re.MULTILINE)
    html_content = re.sub(r'^\d+\. (.*?)$', r'<li>\1</li>', html_content, flags=re.MULTILINE)
    
    # Обрамляем списки
    html_content = re.sub(r'(<li>.*?</li>)', r'<ul>\1</ul>', html_content, flags=re.DOTALL)
    html_content = re.sub(r'</ul>\s*<ul>', r'', html_content)
    
    # Код
    html_content = re.sub(r'`([^`]+)`', r'<code>\1</code>', html_content)
    
    # Блоки кода
    html_content = re.sub(r'```(\w+)?\n(.*?)```', r'<pre><code>\2</code></pre>', html_content, flags=re.DOTALL)
    
    # Таблицы (простая поддержка)
    # Сначала найдем таблицы
    table_pattern = r'\|.*\|.*\n\|.*\|.*\n((?:\|.*\|.*\n)*)'
    
    def convert_table(match):
        table_content = match.group(0)
        rows = table_content.strip().split('\n')
        
        html_table = '<table>\n<thead>\n<tr>\n'
        
        # Заголовок
        header_cells = rows[0].split('|')[1:-1]
        for cell in header_cells:
            html_table += f'<th>{cell.strip()}</th>\n'
        html_table += '</tr>\n</thead>\n<tbody>\n'
        
        # Разделитель (пропускаем)
        
        # Данные
        for row in rows[2:]:
            if '|' in row:
                html_table += '<tr>\n'
                cells = row.split('|')[1:-1]
                for cell in cells:
                    html_table += f'<td>{cell.strip()}</td>\n'
                html_table += '</tr>\n'
        
        html_table += '</tbody>\n</table>'
        return html_table
    
    html_content = re.sub(table_pattern, convert_table, html_content, flags=re.MULTILINE)
    
    # Параграфы
    html_content = re.sub(r'\n\n([^<].*?)\n\n', r'\n\n<p>\1</p>\n\n', html_content, flags=re.DOTALL)
    
    # Убираем лишние пустые строки
    html_content = re.sub(r'\n{3,}', r'\n\n', html_content)
    
    return html_content

def get_nav_links(filename):
    """Возвращает навигационные ссылки в зависимости от файла"""
    base_nav = '<a href="../index.html">🏠 Главная</a>'
    
    if 'TECHNICAL_DOCS' in filename:
        return f'{base_nav} <a href="USER_GUIDE.html">👤 Руководство пользователя</a> <a href="API.html">📡 API</a> <a href="CONFIG_MANAGEMENT.html">⚙️ Конфигурация</a> <a href="../html/index.html">📚 API документация</a>'
    elif 'USER_GUIDE' in filename:
        return f'{base_nav} <a href="TECHNICAL_DOCS.html">🔧 Техническая документация</a> <a href="API.html">📡 API</a> <a href="CONFIG_MANAGEMENT.html">⚙️ Конфигурация</a> <a href="../html/index.html">📚 API документация</a>'
    elif 'API' in filename:
        return f'{base_nav} <a href="USER_GUIDE.html">👤 Руководство пользователя</a> <a href="TECHNICAL_DOCS.html">🔧 Техническая документация</a> <a href="CONFIG_MANAGEMENT.html">⚙️ Конфигурация</a> <a href="../html/index.html">📚 API документация</a>'
    elif 'CONFIG_MANAGEMENT' in filename:
        return f'{base_nav} <a href="USER_GUIDE.html">👤 Руководство пользователя</a> <a href="TECHNICAL_DOCS.html">🔧 Техническая документация</a> <a href="API.html">📡 API</a> <a href="../html/index.html">📚 API документация</a>'
    else:
        return f'{base_nav} <a href="USER_GUIDE.html">👤 Руководство пользователя</a> <a href="TECHNICAL_DOCS.html">🔧 Техническая документация</a> <a href="API.html">📡 API</a> <a href="CONFIG_MANAGEMENT.html">⚙️ Конфигурация</a> <a href="../html/index.html">📚 API документация</a>'

def main():
    """Основная функция"""
    manuals_dir = Path('../docs/manuals')
    
    if not manuals_dir.exists():
        print(f"Директория {manuals_dir} не найдена!")
        sys.exit(1)
    
    # Список файлов для конвертации
    files_to_convert = [
        'TECHNICAL_DOCS.md',
        'USER_GUIDE.md', 
        'API.md',
        'CONFIG_MANAGEMENT.md',
        'COMPENSATION_GUIDE.md',
        'MODBUS_PROTOCOL.md',
        'AGRO_RECOMMENDATIONS.md',
        'VERSION_MANAGEMENT.md',
        'WIRING_DIAGRAM.md'
    ]
    
    for filename in files_to_convert:
        md_file = manuals_dir / filename
        html_file = manuals_dir / filename.replace('.md', '.html')
        
        if not md_file.exists():
            print(f"Файл {md_file} не найден, пропускаем...")
            continue
        
        print(f"Конвертируем {filename}...")
        
        # Читаем Markdown
        with open(md_file, 'r', encoding='utf-8') as f:
            md_content = f.read()
        
        # Извлекаем заголовок
        title_match = re.search(r'^# (.*?)$', md_content, re.MULTILINE)
        title = title_match.group(1) if title_match else filename.replace('.md', '')
        
        # Конвертируем в HTML
        html_content = convert_markdown_to_html(md_content)
        
        # Создаем полный HTML документ
        nav_links = get_nav_links(filename)
        full_html = create_html_template(title, html_content, nav_links)
        
        # Сохраняем HTML файл
        with open(html_file, 'w', encoding='utf-8') as f:
            f.write(full_html)
        
        print(f"Создан {html_file}")
    
    print("Конвертация завершена!")

if __name__ == '__main__':
    main() 