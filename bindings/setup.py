#!/usr/bin/env python3
"""
Setup script для Python-обёрток JXCT
Собирает C++ модули с pybind11 для тестирования
"""

from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11

# Определяем C++ модули для сборки
ext_modules = [
    Pybind11Extension(
        "jxct_core",
        ["bindings/jxct_core.cpp"],
        include_dirs=[
            "include",
            "src",
            pybind11.get_include(),
        ],
        language='c++',
        cxx_std=17,
    ),
]

setup(
    name="jxct-bindings",
    version="1.0.0",
    author="JXCT Team",
    description="Python bindings for JXCT Soil Sensor",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
) 