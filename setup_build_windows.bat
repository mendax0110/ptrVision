@echo off
setlocal

set BUILD_TYPE=%1

if "%BUILD_TYPE%"=="" (
    echo [!] No build type specified. Usage:
    echo     build.bat clang
    echo     build.bat msvc
    exit /b 1
)

if /i "%BUILD_TYPE%"=="clang" (
    set BUILD_DIR=build_clang
    echo [*] Building with Clang...
    set GENERATOR=Ninja
    set CMAKE_C_COMPILER=C:/clang_llvm/bin/clang.exe
    set CMAKE_CXX_COMPILER=C:/clang_llvm/bin/clang++.exe
) else if /i "%BUILD_TYPE%"=="msvc" (
    set BUILD_DIR=build_msvc
    echo [*] Building with MSVC...
    set GENERATOR="Visual Studio 17 2022"
    set GENERATOR_PLATFORM=x64
) else (
    echo [!] Unknown build type: %BUILD_TYPE%
    echo     Use 'clang' or 'msvc'
    exit /b 1
)

if exist %BUILD_DIR% (
    echo Cleaning existing build directory: %BUILD_DIR%
    rmdir /s /q %BUILD_DIR%
)

echo Creating build directory: %BUILD_DIR%
mkdir %BUILD_DIR%
cd %BUILD_DIR%

echo Running CMake...
if /i "%BUILD_TYPE%"=="clang" (
    cmake -G %GENERATOR% ^
        -DCMAKE_C_COMPILER=%CMAKE_C_COMPILER% ^
        -DCMAKE_CXX_COMPILER=%CMAKE_CXX_COMPILER% ^
        -DCMAKE_BUILD_TYPE=Release ^
        ..
) else (
    cmake -G %GENERATOR% -A %GENERATOR_PLATFORM% -DCMAKE_BUILD_TYPE=Release ..
)

if %errorlevel% neq 0 (
    echo [!] CMake configuration failed!
    exit /b %errorlevel%
)

echo Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo [!] Build failed!
    exit /b %errorlevel%
)

echo [✓] Build successful!
endlocal
pause
