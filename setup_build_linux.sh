#!/bin/bash

echo "Creating build_linux directory..."
mkdir -p build_linux

cd build_linux

echo "Running cmake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

echo "Running make..."
make

echo "Build complete!"
