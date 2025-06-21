Import("env")

# This script runs after normal firmware build (post script)
# It invokes PlatformIO target BUILD_FS to create SPIFFS image with UI static files.
# Resulting binary will be at .pio/build/<env>/spiffs.bin
# We also copy it to build directory with a descriptive name.

import os
import shutil

# Only run for esp32 environments (skip native)
if env['PIOPLATFORM'] != 'espressif32':
    print("[build_fs] Skipped (non-ESP32 env)")
else:
    print("[build_fs] Building SPIFFS image with UI static files ...")
    # Trigger buildfs
    env.Execute("$BUILDFS")
    spiffs_bin = os.path.join(env.subst("$BUILD_DIR"), "spiffs.bin")
    if os.path.isfile(spiffs_bin):
        dst = os.path.join(env.subst("$BUILD_DIR"), "web_spiffs.bin")
        shutil.copy2(spiffs_bin, dst)
        print(f"[build_fs] SPIFFS image copied to {dst}")
    else:
        print("[build_fs] spiffs.bin not found (buildfs may have failed)") 