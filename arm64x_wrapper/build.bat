@echo off
setlocal

:find_toolchain
set VSWHERE="%PROGRAMFILES%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist %VSWHERE% (
  set VSWHERE="%PROGRAMFILES(X86)%\Microsoft Visual Studio\Installer\vswhere.exe"
)
if not exist %VSWHERE% (
  echo vswhere not found, could not find a working MSVC install.
  exit /b 1
)

for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -requires Microsoft.VisualStudio.Component.VC.Tools.ARM64EC -property installationPath`) do (
  set ARM64EC_TOOLCHAIN=%%i
)

if exist "%ARM64EC_TOOLCHAIN%\Common7\Tools\vsdevcmd.bat" (
  call "%ARM64EC_TOOLCHAIN%\Common7\Tools\vsdevcmd.bat" -arch=arm64 -host_arch=x86
  echo.
) else (
  echo ARM64EC capable toolchain not found
  exit /b 1
)

exit /b

call :find_toolchain
if errorlevel 1 exit /b 1

REM Build arm64x_wrapper.cpp
cl.exe /c /Fo:arm64x_wrapper.o arm64x_wrapper.cpp || exit /b 1

REM Build weasel.dll wrapper
link.exe /dll /noentry /machine:arm64x /defArm64Native:WeaselTSF_arm64.def /def:WeaselTSF_x64.def ^
  /out:weaselARM64X.dll arm64x_wrapper.o /ignore:4104

REM Build weasel.ime wrapper
link.exe /dll /noentry /machine:arm64x /defArm64Native:WeaselIME_arm64.def /def:WeaselIME_x64.def ^
  /out:weaselARM64X.ime arm64x_wrapper.o /ignore:4104

endlocal
exit /b