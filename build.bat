@rem Find and build all *.sln file in current directory
@echo off
setlocal enabledelayedexpansion
set /a errorno=1
for /F "delims=#" %%E in ('"prompt #$E# & for %%E in (1) do rem"') do set "esc=%%E"

rem Set Visual C++ environment for Windows, desktop, x64.
rem
rem https://github.com/Microsoft/vswhere
rem https://github.com/microsoft/vswhere/wiki/Find-VC#batch

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%vswhere%" (
  echo Failed to find "vswhere.exe".  Please install the latest version of Visual Studio.
  goto :ERROR
)

set "InstallDir="
for /f "usebackq tokens=*" %%i in (
  `"%vswhere%" -latest ^
               -products * ^
               -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
               -property installationPath`
) do (
  set "InstallDir=%%i"
)
if "%InstallDir%" == "" (
  echo Failed to find Visual C++.  Please install the latest version of Visual C++.
  goto :ERROR
)

call "%InstallDir%\VC\Auxiliary\Build\vcvars64.bat" || goto :ERROR


rem https://docs.microsoft.com/visualstudio/msbuild/msbuild-command-line-reference

for %%i in (*.sln) do (
  echo msbuild "%%i"
  msbuild "%%i" ^
          /nologo ^
          /v:minimal ^
          /m ^
          /p:Configuration=Release ^
          /p:Platform=x64 ^
          /t:Clean,Build ^
          || goto :ERROR
)
echo Build Status -%esc%[92m SUCCEEDED %esc%[0m
set /a errorno=0
goto :END


:ERROR
echo Abort by error.
echo Build Status -%esc%[91m ERROR %esc%[0m


:END
exit /B %errorno%
