#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import requests
import pytest


def get_base_url() -> str:
    # JXCT_DEVICE_IP can be set to override device IP, default kept for local runs
    ip = os.environ.get("JXCT_DEVICE_IP", "192.168.2.65")
    return f"http://{ip}"


@pytest.fixture(scope="session")
def device_base_url():
    return get_base_url()


def ensure_device_or_skip(base_url: str, timeout: float = 1.5) -> None:
    try:
        requests.get(f"{base_url}/health", timeout=timeout)
    except Exception:
        pytest.skip("ESP32 недоступен (ожидаемо для CI/без железа)")


@pytest.fixture(scope="function")
def driver():
    """Selenium WebDriver для E2E тестов (Chrome headless)"""
    try:
        from selenium import webdriver
        from selenium.webdriver.chrome.options import Options
        opts = Options()
        opts.add_argument("--headless")
        opts.add_argument("--no-sandbox")
        opts.add_argument("--disable-dev-shm-usage")
        opts.add_argument("--disable-gpu")
        opts.add_argument("--window-size=1920,1080")
        wd = webdriver.Chrome(options=opts)
        yield wd
        wd.quit()
    except Exception as e:
        pytest.skip(f"Selenium/Chrome недоступен: {e}")


