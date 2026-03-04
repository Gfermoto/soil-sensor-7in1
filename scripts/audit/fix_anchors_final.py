#!/usr/bin/env python3
"""
Финальный скрипт для исправления якорей в документации MkDocs.
Удаляет все внутренние ссылки на якоря, оставляя только внешние ссылки.
"""

import os
import re
import glob
from pathlib import Path

def fix_anchors_in_file(file_path):
    """Исправляет якоря в файле, удаляя внутренние ссылки на несуществующие якоря."""
    print(f"Обрабатываю: {file_path}")

    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Находим все ссылки на якоря
    link_pattern = r'\[([^\]]+)\]\(#([^)]+)\)'

    def replace_link(match):
        link_text = match.group(1)
        old_anchor = match.group(2)

        # Если это ссылка на "содержание" или другие проблемные якоря - убираем якорь
        if old_anchor in ['soderzhanie', 'obzor', 'vvedenie', 'komplektatsiya', 'ustanovka-i-nastroyka',
                          'veb-interfeys', 'rabota-s-pokazaniyami', 'nastroyka-parametrov',
                          'kalibrovka-datchika', 'obnovleniya-proshivki', 'servisnye-funktsii',
                          'chasto-zadavaemye-voprosy', 'dostup-k-api', 'veb-stranitsy', 'nastroyki',
                          'mqtt-integratsiya', 'thingspeak-integratsiya', 'kody-oshibok',
                          'cors-podderzhka', 'aktualnye-formuly', 'arhitektura-protsessa',
                          'validatsiya-vhodnyh-dannyh', 'preimuschestva-obnovlennoy-modeli',
                          'trebuemye-izmeneniya-v-proshivke', 'istochniki', 'obzor-overview',
                          'veb-interfeys-web-interface', 'eksport-konfiguratsii-export',
                          'import-konfiguratsii-import', 'massovoe-razvertyvanie-mass-deployment',
                          'tehnicheskie-detali-technical-details', 'otladka-debugging',
                          'svyazannaya-dokumentatsiya-related-docs', 'istoriya-izmeneniy-changelog',
                          'tehnicheskie-harakteristiki', 'karta-registrov', 'primery-protokola',
                          'shema-podklyucheniya-esp32', 'arhitektura-sistemy', 'apparatnaya-arhitektura',
                          'programmnaya-arhitektura', 'setevye-protokoly', 'algoritmy-kompensatsii',
                          'api-dokumentatsiya', 'konfiguratsiya', 'struktura-proekta', 'razrabotka',
                          'fayl-versii', 'kak-izmenit-versiyu', 'dostupnye-makrosy', 'sravnenie-versiy',
                          'preimuschestva', 'primery-ispolzovaniya', 'protsess-reliza', 'vazhnye-zamechaniya',
                          'rezultat', 'rs-485-soedinenie-rs485-connection', 'pitanie-datchika-power-supply',
                          'vazhnye-zamechaniya-important-notes', 'rekomendatsii-po-montazhu-installation-tips',
                          'obschie-printsipy-monitoringa-pochvy-obschie-printsipy-monitoringa-pochvy',
                          'temperatura-pochvy-temperatura-pochvy', 'vlazhnost-pochvy-vlazhnost-pochvy',
                          'elektroprovodnost-ec-elektroprovodnost-ec', 'ph-pochvy-ph-pochvy',
                          'azot-n-azot-n', 'fosfor-p-fosfor-p', 'kaliy-k-kaliy-k',
                          'rekomendatsii-po-kulturam-rekomendatsii-po-kulturam',
                          'sezonnye-korrektirovki-npk-sezonnye-korrektirovki',
                          'kalibrovka-i-poverka-kalibrovka-i-poverka',
                          'prakticheskie-rekomendatsii-prakticheskie-rekomendatsii']:
            # Убираем якорь, оставляем только текст
            return f'**{link_text}**'

        # Для остальных ссылок оставляем как есть
        return match.group(0)

    # Заменяем ссылки
    new_content = re.sub(link_pattern, replace_link, content)

    # Записываем обратно
    if new_content != content:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"  ✅ Исправлено")
        return True
    else:
        print(f"  ✅ Без изменений")
        return False

def main():
    """Основная функция."""
    docs_dir = Path("docs")

    # Находим все MD файлы
    md_files = []
    md_files.extend(docs_dir.glob("*.md"))
    md_files.extend(docs_dir.glob("manuals/*.md"))
    md_files.extend(docs_dir.glob("dev/*.md"))

    print(f"Найдено {len(md_files)} MD файлов")
    print("Исправляю проблемные якоря...")

    fixed_count = 0

    # Обрабатываем каждый файл
    for file_path in md_files:
        if fix_anchors_in_file(file_path):
            fixed_count += 1

    print(f"\n🎉 Исправление завершено!")
    print(f"Исправлено файлов: {fixed_count}")

if __name__ == "__main__":
    main()
