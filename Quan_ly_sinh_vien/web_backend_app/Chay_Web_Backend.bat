@echo off
chcp 65001 > nul
cd /d "%~dp0"

echo ============================================================
echo   STUDENT MANAGER - WEB BACKEND APP
echo ============================================================
echo.

echo Kiem tra Python...
python --version > nul 2>&1
if errorlevel 1 (
    echo Khong tim thay Python bang lenh python.
    echo Hay cai Python 3 hoac them Python vao PATH.
    echo Link tai: https://www.python.org/downloads/
    echo.
    pause
    exit /b 1
)

echo Dang mo trinh duyet...
start http://127.0.0.1:5050

echo Dang chay backend server...
echo Neu muon dung server, nhan Ctrl + C trong cua so nay.
echo.
python server.py

pause
