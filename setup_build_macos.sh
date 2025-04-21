#!/bin/bash

echo "[INFO] Setting up build environment for macos..."
export PATH="/usr/lib/llvm-19/bin:$PATH"
export LLVM_DIR="/usr/lib/llvm-19/lib/cmake/llvm"
export CLANG_DIR="/usr/lib/llvm-19/lib/cmake/clang"

echo "[INFO] Verifying LLVM installation and version..."
llvm_version=$(llvm-config --version)
if [[ $? -ne 0 ]]; then
    echo "[ERROR] LLVM is not installed or not found in PATH."
    exit 1
fi
if [[ $llvm_version != 19.* ]]; then
    echo "[INFO] LLVM version 19.x is required. Found version: $llvm_version"
    exit 1
fi
echo "[SUCCESS] LLVM version is $llvm_version"

echo "[INFO] Checking Git submodules..."
git submodule update --init --recursive

pushd external/dearimgui > /dev/null

current_branch=$(git rev-parse --abbrev-ref HEAD)
if [[ "$current_branch" != "docking" ]]; then
    echo "[ERROR] dearimgui is on branch '$current_branch'. Expected 'docking'."
    echo "Attempting to switch..."
    git fetch origin docking
    git checkout docking
    if [[ $? -ne 0 ]]; then
        echo "[ERROR] Failed to switch to 'docking' branch for dearimgui"
        exit 1
    fi
else
    echo "[SUCCESS] dearimgui is on the correct branch: docking"
fi

popd > /dev/null

echo "[INFO] Ensuring all submodules are up to date..."
git submodule update --recursive --remote

echo "[INFO] Creating build_macos directory..."
mkdir -p build_macos
cd build_macos

echo "[INFO] Running cmake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

echo "[INFO] Running make..."
make

echo "[SUCCESS] Build complete!"
