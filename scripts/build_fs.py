Import("env")

# This script runs after normal firmware build (post script)
# It invokes PlatformIO target BUILD_FS to create SPIFFS image with UI static files.
# Resulting binary will be at .pio/build/<env>/spiffs.bin
# We also copy it to build directory with a descriptive name.

import os
import shutil
from SCons.Script import COMMAND_LINE_TARGETS
import sys  # add after existing imports

# Only run for esp32 environments (skip native)
if env['PIOPLATFORM'] != 'espressif32':
    print("[build_fs] Skipped (non-ESP32 env)")
else:
    if "buildfs" in COMMAND_LINE_TARGETS:
        # We're already in the context of a `buildfs` call. Avoid recursion and do nothing.
        print("[build_fs] Post-script invoked during buildfs target – no further action")
    else:
        print("[build_fs] Building SPIFFS image with UI static files ...")
        # Trigger buildfs via PlatformIO CLI because BUILDFS var not available in PIO 6.4+
        import subprocess, sys
        cmd = [sys.executable, "-m", "platformio", "run", "-e", env["PIOENV"], "-t", "buildfs"]
        if subprocess.call(cmd) != 0:
            sys.exit("buildfs failed")
        spiffs_bin = os.path.join(env.subst("$BUILD_DIR"), "spiffs.bin")
        if os.path.isfile(spiffs_bin):
            dst = os.path.join(env.subst("$BUILD_DIR"), "web_spiffs.bin")
            shutil.copy2(spiffs_bin, dst)
            print(f"[build_fs] SPIFFS image copied to {dst}")
        else:
            print("[build_fs] spiffs.bin not found (buildfs may have failed)") 