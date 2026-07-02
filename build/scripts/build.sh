#!/bin/bash

CONFIG_FILE="build/config/build.conf"

if [ ! -f "$CONFIG_FILE" ]; then
    echo "[ERROR] Build configuration not found."
    exit 1
fi

# Load configuration
source "$CONFIG_FILE"

echo "====================================="
echo " Kernec Build System"
echo "====================================="
echo

echo "Project      : $PROJECT_NAME"
echo "Version      : $PROJECT_VERSION"
echo "Codename     : $PROJECT_CODENAME"
echo "Architecture : $TARGET_ARCH"
echo "Compiler     : $TOOLCHAIN"
echo "Build Type   : $BUILD_TYPE"
echo "Optimization : $OPT_LEVEL"
echo

echo "[INFO] Build orchestration not yet implemented."
echo
echo "Future build stages:"
echo " 1. Verify toolchain"
echo " 2. Build kernel"
echo " 3. Build userspace"
echo " 4. Build packages"
echo " 5. Assemble root filesystem"
echo " 6. Generate bootable ISO"
echo "[Stage 1/6] Verifying build environment..."

./build/scripts/check-env.sh

if [ $? -ne 0 ]; then
    echo
    echo "[BUILD FAILED]"
    exit 1
fi

echo
