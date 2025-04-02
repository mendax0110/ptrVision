@echo off

IF EXIST "%~dp0build_windows\Debug\ptrVision.exe" (
    SET EXE_PATH=%~dp0build_windows\Debug\ptrVision.exe
) ELSE (
    IF EXIST "%~dp0build_windows\Release\ptrVision.exe" (
        SET EXE_PATH=%~dp0build_windows\Release\ptrVision.exe
    ) ELSE (
        echo Error: ptrVision.exe not found in build_windows directory.
        exit /b 1
    )
)

SET TEST_DIR=%~dp0test

echo "Running tests with {EXE_PATH} in {TEST_DIR} directory"
"%EXE_PATH%" "%TEST_DIR%"
