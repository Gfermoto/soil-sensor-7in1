#!/usr/bin/env python3
"""
Профессиональный статический анализ JXCT
Использует clang-tidy с системой исключений
"""

import subprocess
import sys
# All imports are used in the script

def run_with_timeout(cmd, timeout=60):
    """Запускает команду с таймаутом"""
    try:
        print(f"   Running: {' '.join(cmd)}")
        result = subprocess.run(
            cmd, 
            capture_output=True, 
            text=True, 
            timeout=timeout,
            shell=False,
            encoding='utf-8',
            errors='replace'
        )
        return result.returncode == 0, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        print(f"   ⏰ Timeout after {timeout} seconds")
        return False, "", f"Timeout after {timeout} seconds"
    except Exception as e:
        print(f"   ❌ Error: {e}")
        return False, "", str(e)

def run_clang_tidy_analysis():
    """Запускает профессиональный clang-tidy анализ"""
    cmd = ['python', 'scripts/run_clang_tidy_analysis.py']
    return run_with_timeout(cmd, timeout=60)

def main():
    print("Professional Static Analysis JXCT")
    print("=" * 50)
    
    # Clang-tidy анализ
    print("Running clang-tidy analysis...")
    clang_ok, clang_out, clang_err = run_clang_tidy_analysis()
    
    print(f"Return code: {0 if clang_ok else 1}")
    
    if clang_out:
        print("Output:")
        print("-" * 40)
        print(clang_out)
        print("-" * 40)
    
    if clang_err:
        print("Errors:")
        print("-" * 40)
        print(clang_err)
        print("-" * 40)
    
    # Итоговая статистика
    print("\n" + "=" * 50)
    print("SUMMARY:")
    print(f"clang-tidy: {'PASSED' if clang_ok else 'FAILED'}")
    
    if clang_ok:
        print("Status: PROFESSIONAL QUALITY ✅")
        return 0
    else:
        print("Status: NEEDS ATTENTION ❌")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 