@echo off
setlocal

:: Create build directory if it doesn't exist
if not exist build (
    mkdir build
)

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
pause
exit /b 1
