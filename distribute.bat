setlocal enabledelayedexpansion
set INCLUDE_DIR=%~dp0include\
set SRC_DIR=%~dp0src\
set DISTRIBUTE_DIR=%~dp0.distribute\
%~d0
rd /s /q "%DISTRIBUTE_DIR%"

if exist "%INCLUDE_DIR%" (
cd "%INCLUDE_DIR%"
if not exist "%DISTRIBUTE_DIR%define\include" (
	mkdir "%DISTRIBUTE_DIR%define\include"
)
for %%i in (*) do (
	xcopy /F /I /R /K /Y "%INCLUDE_DIR%%%i" "%DISTRIBUTE_DIR%define\include\"
)
for /d %%i in (*) do (
	if not exist "%DISTRIBUTE_DIR%%%i\include\%%i" (
		mkdir "%DISTRIBUTE_DIR%%%i\include\%%i"
	)
	if %errorlevel% EQU 0 (
		xcopy /E /Y "%INCLUDE_DIR%%%i" "%DISTRIBUTE_DIR%%%i\include\%%i"
		for %%f in ("%INCLUDE_DIR%%%i\*") do (
			if exist "%INCLUDE_DIR%%%~nfDefine.h" (
				del /s /q /f "%DISTRIBUTE_DIR%define\include\%%~nfDefine.h"
				xcopy /F /I /R /K /Y "%INCLUDE_DIR%%%~nfDefine.h" "%DISTRIBUTE_DIR%%%i\include\"
			)
		)
	)
)
)

if exist "%SRC_DIR%" (
cd "%SRC_DIR%"
for /d %%i in (*) do (
	if not exist "%DISTRIBUTE_DIR%%%i\src\%%i" (
		mkdir "%DISTRIBUTE_DIR%%%i\src\%%i"
	)
	if %errorlevel% EQU 0 (
		xcopy /E /Y "%SRC_DIR%%%i" "%DISTRIBUTE_DIR%%%i\src\%%i"
	)
)
)

if exist "%DISTRIBUTE_DIR%" (
cd "%DISTRIBUTE_DIR%"
for /d %%i in (*) do (
	echo %%i
	if exist "%~dp0..\qwer_%%i" (
		cd "%~dp0..\qwer_%%i"
		if %errorlevel% EQU 0 (
			call git pull --rebase
			if %errorlevel% EQU 0 (
				xcopy /E /Y "%DISTRIBUTE_DIR%%%i" "%~dp0..\qwer_%%i"
			)
			cd "%DISTRIBUTE_DIR%"
		)
	)
)
rd /s /q "%DISTRIBUTE_DIR%"
)