@echo off

echo Creating build_windows directory...
mkdir build_windows

cd build_windows

echo Running cmake...
cmake -G "Visual Studio 17 2022" -A x64 -T host=x64 -DCMAKE_BUILD_TYPE=Release ..\

echo Running cmake build...
cmake --build . --config Release

echo Build complete!
