@echo off
cd /d "%~dp0"

cmake --build build
if errorlevel 1 (
    echo Kompilacja nie powiodla sie.
    pause
    exit /b 1
)

build\VoxelEngine.exe