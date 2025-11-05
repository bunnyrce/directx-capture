@echo off
REM Build script for DirectX desktop duplication demo (MSVC cl.exe)
REM //madebybunnyrce
REM This build script requires Windows SDK and Visual Studio environment.

if not defined VSINSTALLDIR (
    echo Visual Studio environment not found. Run "Developer Command Prompt for VS" first.
    goto :eof
)

cl /EHsc /W3 /MD "capture.cpp" /link d3d11.lib dxgi.lib
echo Build finished. If no errors, run capture.exe and receiver.exe in separate consoles.
pause
