@echo off
setlocal

:: Create build directory if it doesn't exist
if not exist build (
    mkdir build
)

cd build

echo Configuring with CMake...
cmake .. -G "Ninja"
if errorlevel 1 goto :error

echo Building...
ninja
if errorlevel 1 goto :error

cd ..
echo.
echo Running game...
Purrsuit.exe
goto :eof

:error
echo [ERROR] Build failed!
pause
exit /b 1
