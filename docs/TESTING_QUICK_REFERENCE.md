# Тестирование — быстрая справка

## Команды

```bash
pio test -e native
python -m pytest test/ --maxfail=1 -q
python scripts/run_simple_tests.py
```

## E2E

```bash
export JXCT_DEVICE_IP=192.168.2.65
python -m pytest test/test_api_schema.py test/e2e/test_web_ui.py -q
```

## Анализ

```bash
python scripts/run_clang_tidy_analysis.py
pio check -e static-analysis
```

## Метрики

- Покрытие: ~70.8%
- Unit: 67, E2E: 23
- Clang-tidy: 125+ предупреждений
