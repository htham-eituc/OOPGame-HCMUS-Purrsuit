@echo off
setlocal

:: ================================
:: CONFIGURATION
:: ================================

:: Your game exe name
set EXE=Purrsuit.exe

:: Where MinGW is installed (adjust to your system!)
set MINGW=D:\msys64\mingw64\bin

:: Where SDL2, SDL2_image, SDL2_mixer, SDL2_ttf are installed
:: Typically this is the root of your SDL2 SDK, adjust to match yours
set SDL2_PATH=D:\SDL2

:: Output folder for packaging
set OUT=dist

:: ================================
:: START PACKAGING
:: ================================

echo Creating package in %OUT%...

:: Create output folder
if exist %OUT% rmdir /s /q %OUT%
mkdir %OUT%
mkdir %OUT%\assets

:: Copy your exe
echo Copying game exe...
copy "%EXE%" "%OUT%\" >nul

:: Copy your assets
echo Copying assets...
xcopy assets "%OUT%\assets" /E /I /Y >nul

:: Copy required MinGW runtime DLLs
echo Copying MinGW runtime DLLs...
copy "%MINGW%\libgcc_s_seh-1.dll" "%OUT%\" >nul
copy "%MINGW%\libstdc++-6.dll" "%OUT%\" >nul
copy "%MINGW%\libwinpthread-1.dll" "%OUT%\" >nul

:: Copy SDL2 DLLs
echo Copying SDL2 DLLs...
copy "%SDL2_PATH%\bin\SDL2.dll"       "%OUT%\" >nul
copy "%SDL2_PATH%\bin\SDL2_image.dll" "%OUT%\" >nul
copy "%SDL2_PATH%\bin\SDL2_mixer.dll" "%OUT%\" >nul
copy "%SDL2_PATH%\bin\SDL2_ttf.dll"   "%OUT%\" >nul

echo.
echo ======================================
echo Packaging complete! Check the %OUT% folder.
echo ======================================

pause
