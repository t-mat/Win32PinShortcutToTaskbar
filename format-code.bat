@rem Apply clang-format for all C/C++ files in %srcdirs%
@setlocal enabledelayedexpansion
@echo off
set /a errorno=1
for /F "delims=#" %%E in ('"prompt #$E# & for %%E in (1) do rem"') do set "esc=%%E"

@rem srcdirs : Set your source code directories
set srcdirs=.\src

@rem extensions : Set extensions of your source codes
set extensions=*.c   ^
               *.cc  ^
               *.cpp ^
               *.cxx ^
               *.hxx ^
               *.hpp ^
               *.hh  ^
               *.h

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

clang-format --version || goto :ERROR

for %%s in (%srcdirs%) do (
  pushd "%%s"
  for /r %%f in (%extensions%) do (
    echo clang-format -i "%%f"
         clang-format -i "%%f"
  )
  popd
)

echo Build Status -%esc%[92m SUCCEEDED %esc%[0m
set /a errorno=0
goto :END


:ERROR
echo Abort by error.
echo Build Status -%esc%[91m ERROR %esc%[0m


:END
exit /B %errorno%
