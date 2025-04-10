#!/bin/bash

echo "Setting up build environment for Linux..."
export PATH="/usr/lib/llvm-12/bin:$PATH"
export LLVM_DIR="/usr/lib/llvm-12/lib/cmake/llvm"
export CLANG_DIR="/usr/lib/llvm-12/lib/cmake/clang"

echo "Verifying LLVM installation and version..."
llvm_version=$(llvm-config --version)
if [[ $? -ne 0 ]]; then
    echo "LLVM is not installed or not found in PATH."
    exit 1
fi
if [[ $llvm_version != 12.* ]]; then
    echo "LLVM version 12.x is required. Found version: $llvm_version"
    exit 1
fi
echo "LLVM version is $llvm_version, proceeding with build..."

echo "Creating build_linux directory..."
mkdir -p build_linux

cd build_linux

echo "Running cmake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

echo "Running make..."
make

echo "Build complete!"
