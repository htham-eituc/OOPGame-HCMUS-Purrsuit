@echo off
setlocal

:: Clean build
if exist build (
    echo Cleaning previous build...
    rd /s /q build
)
mkdir build
cd build

echo Configuring with CMake...
cmake .. -G "MinGW Makefiles"
if errorlevel 1 goto :error

echo Building...
mingw32-make
if errorlevel 1 goto :error

cd ..
echo.
echo Running game...
Purrsuit.exe
goto :eof

:error
echo [ERROR] Build failed!
exit /b 1
