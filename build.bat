@echo off
echo Building your game...

:: Set your SDL2 paths (adjust these to match your SDL installation)
set SDL2_PATH=C:\sdl2
set INCLUDE_PATH=%SDL2_PATH%\include\SDL2
set LIB_PATH=%SDL2_PATH%\lib

:: Output settings
set OUTPUT=game.exe

:: Source files
set SOURCES=src\*.cpp

:: Compile
g++ -Iinclude -I%INCLUDE_PATH% %SOURCES% -L%LIB_PATH% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o %OUTPUT%

if %errorlevel% neq 0 (
    echo.
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo.
    echo Build succeeded! Run .\%OUTPUT%
    echo.
    start %OUTPUT% 
)

pause
