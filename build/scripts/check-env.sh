#!/bin/bash

echo "====================================="
echo " Kernec Environment Verification"
echo "====================================="
echo

REQUIRED_TOOLS=("gcc" "make" "cmake" "git")

MISSING=0

for tool in "${REQUIRED_TOOLS[@]}"
do
    if command -v "$tool" >/dev/null 2>&1; then
        VERSION=$("$tool" --version 2>/dev/null | head -n 1)
        echo "[ OK ] $VERSION"
    else
        echo "[FAIL] $tool not found"
        MISSING=1
    fi
done

echo

if [ "$MISSING" -eq 0 ]; then
    echo "[SUCCESS] Build environment is ready."
    exit 0
else
    echo "[ERROR] One or more required tools are missing."
    exit 1
fi
