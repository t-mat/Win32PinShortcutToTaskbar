@rem Find and run executable in %EXEPATH%
@setlocal enabledelayedexpansion
@echo off
pushd "%~dp0"
set /a errorno=1
for /F "delims=#" %%E in ('"prompt #$E# & for %%E in (1) do rem"') do set "esc=%%E"

set "EXEPATH=.\x64\Release"

call .\build.bat || goto :END

set "EXE="
for /f "tokens=*" %%i in ('where /f "%EXEPATH%:*.exe"') do ( set EXE=%%i )
if "%EXE%" == "" (
  echo ERROR - Can't find executable file in %EXEPATH%
  goto :ERROR
)

echo %EXE%
     %EXE% || goto :FAIL

echo Test -%esc%[92m OK %esc%[0m
set /a errorno=0
goto :END

:FAIL
echo Test -%esc%[91m FAIL %esc%[0m

:ERROR
echo Status -%esc%[91m ERROR %esc%[0m
set /a errorno=1

:END
popd
exit /B %errorno%
