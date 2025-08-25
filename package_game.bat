@echo off
setlocal

:: Change this to your game exe name
set EXE=Purrsuit.exe

:: Where MinGW is installed (adjust to your system!)
set MINGW=D:\msys64\mingw64\bin

:: Output folder for packaging
set OUT=dist

echo Creating package in %OUT%...

:: Create output folder
if exist %OUT% rmdir /s /q %OUT%
mkdir %OUT%
mkdir %OUT%\assets

:: Copy your exe
copy %EXE% %OUT%\

:: Copy your assets (adjust path as needed)
xcopy assets %OUT%\assets /E /I

:: Copy required MinGW runtime DLLs
copy %MINGW%\libgcc_s_seh-1.dll %OUT%\
copy %MINGW%\libstdc++-6.dll %OUT%\
copy %MINGW%\libwinpthread-1.dll %OUT%\

:: Copy SDL2 DLLs (adjust path if you keep them elsewhere)
echo Copying SDL2 DLLs...
set SDL2_PATH=D:\sdl2

copy "%SDL2_PATH%\bin\SDL2.dll"  %OUT%\
copy "%SDL2_PATH%\bin\SDL2_image.dll"  %OUT%\
copy "%SDL2_PATH%\bin\SDL2_mixer.dll"  %OUT%\
copy "%SDL2_PATH%\bin\SDL2_ttf.dll"  %OUT%\

pause
