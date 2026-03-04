#!/usr/bin/env python3
"""
Скриншоты веб-интерфейса для статьи Habr.
Требует: устройство доступно по JXCT_DEVICE_IP (по умолчанию 192.168.2.65).
Использует CDP Page.captureScreenshot для точного полностраничного захвата.
"""

import base64
import os
import time
from pathlib import Path

from selenium import webdriver
from selenium.webdriver.chrome.options import Options

BASE_URL = os.environ.get("JXCT_DEVICE_IP", "192.168.2.65")
OUTPUT_DIR = Path(__file__).resolve().parent.parent / "habr_article" / "images"


def capture_full_page(driver, out_path: Path) -> None:
    """Полностраничный скриншот через Chrome DevTools Protocol."""
    metrics = driver.execute_cdp_cmd("Page.getLayoutMetrics", {})
    content = metrics["contentSize"]
    w = int(content["width"])
    h = int(content["height"])
    result = driver.execute_cdp_cmd(
        "Page.captureScreenshot",
        {
            "clip": {"x": 0, "y": 0, "width": w, "height": h, "scale": 1},
            "captureBeyondViewport": True,
        },
    )
    out_path.write_bytes(base64.b64decode(result["data"]))


def main():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    url_base = f"http://{BASE_URL}"

    opts = Options()
    opts.add_argument("--headless")
    opts.add_argument("--no-sandbox")
    opts.add_argument("--disable-dev-shm-usage")
    opts.add_argument("--disable-gpu")
    opts.add_argument("--window-size=1280,800")

    print(f"Подключение к {url_base}...")
    driver = webdriver.Chrome(options=opts)

    try:
        pages = [
            ("/", "01_main_settings.png", "Главная (настройки)"),
            ("/readings", "02_readings.png", "Показания датчика"),
            ("/calibration", "03_calibration.png", "Калибровка"),
            ("/service", "04_service.png", "Диагностика"),
        ]

        for path, filename, desc in pages:
            url = url_base + path
            print(f"  {desc}: {url}")
            driver.get(url)
            time.sleep(2.5)
            out_path = OUTPUT_DIR / filename
            capture_full_page(driver, out_path)
            print(f"    -> {out_path}")

        print(f"\nГотово. Скриншоты в {OUTPUT_DIR}")
    finally:
        driver.quit()


if __name__ == "__main__":
    main()
