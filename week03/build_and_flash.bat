@echo off
setlocal

rem Run from the folder containing this script
cd /d "%~dp0"

set "BUILD_DIR=build"
set "FIRMWARE=%BUILD_DIR%\app_firmware.bin"
set "FLASH_ADDRESS=0x08000000"

echo ================================================
echo STM32 BUILD AND FLASH
echo ================================================
echo.

rem Check required tools
where cmake >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake was not found in PATH.
    goto :failed
)

where ninja >nul 2>&1
if errorlevel 1 (
    echo ERROR: Ninja was not found in PATH.
    goto :failed
)

where STM32_Programmer_CLI.exe >nul 2>&1
if errorlevel 1 (
    echo ERROR: STM32_Programmer_CLI.exe was not found in PATH.
    goto :failed
)

echo STEP 1: CLEANING BUILD DIRECTORY
if exist "%BUILD_DIR%" (
    echo Deleting existing build folder...
    rmdir /s /q "%BUILD_DIR%"

    if exist "%BUILD_DIR%" (
        echo ERROR: Could not delete the build folder.
        goto :failed
    )
) else (
    echo No existing build folder was found.
)
echo.

echo STEP 2: CONFIGURING PROJECT WITH CMAKE
cmake -S . -B "%BUILD_DIR%" -G Ninja

if errorlevel 1 (
    echo ERROR: CMake configuration failed.
    goto :failed
)
echo.

echo STEP 3: COMPILING FIRMWARE WITH NINJA
ninja -C "%BUILD_DIR%"

if errorlevel 1 (
    echo ERROR: Firmware build failed.
    goto :failed
)

if not exist "%FIRMWARE%" (
    echo ERROR: File %FIRMWARE% was not found.
    goto :failed
)
echo.

echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
STM32_Programmer_CLI.exe -c port=SWD -w "%FIRMWARE%" %FLASH_ADDRESS% -v -rst

if errorlevel 1 (
    echo ERROR: Flashing firmware failed.
    goto :failed
)
echo.

echo ================================================
echo BUILD AND FLASH COMPLETED SUCCESSFULLY
echo ================================================
echo.
pause
endlocal
exit /b 0

:failed
echo.
echo ================================================
echo BUILD AND FLASH PROCESS FAILED
echo ================================================
echo.
pause
endlocal
exit /b 1