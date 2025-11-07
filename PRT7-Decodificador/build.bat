@echo off
REM Script de construccion unico para el Decodificador PRT-7 (Windows + MinGW)
REM Siempre usa CMake con el generador "MinGW Makefiles" y el directorio único "build".
REM Soporta: build limpio (borra caches), cierre del exe en uso y compilacion en un paso.

echo ================================================
echo       CONSTRUCTOR DEL DECODIFICADOR PRT-7
echo ================================================
echo.

REM Verificar si CMake esta disponible
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake no encontrado en el sistema.
    echo Por favor, instale CMake y agregarlo al PATH.
    pause
    exit /b 1
)

REM Crear directorio de construccion si no existe

REM Limpieza opcional o forzada si hay conflictos de generador
if /I "%1"=="clean" (
    echo Limpiando directorios previos de build...
    if exist build (
        rmdir /S /Q build
    )
    if exist build_mingw (
        rmdir /S /Q build_mingw
    )
)

REM Intentar cerrar el ejecutable si estuviera corriendo para evitar LNK1104/Permission denied
taskkill /IM prt7_decodificador.exe /F >nul 2>nul

REM Asegurar directorio de construccion unico
if not exist "build" (
    echo Creando directorio de construccion...
    mkdir build
)

REM Navegar al directorio de construccion
cd build

echo Configurando proyecto con CMake (MinGW Makefiles)...
cmake .. -G "MinGW Makefiles"
if %ERRORLEVEL% neq 0 (
    echo ERROR: Fallo la configuracion de CMake.
    pause
    exit /b 1
)

echo.
echo Compilando proyecto...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
    echo ERROR: Fallo la compilacion.
    pause
    exit /b 1
)

echo.
echo ================================================
echo       COMPILACION EXITOSA
echo ================================================
echo.
echo El ejecutable se encuentra en: build\bin\prt7_decodificador.exe
echo.
echo Para ejecutar el programa:
echo   cd build\bin
echo   prt7_decodificador.exe
echo.

REM Preguntar si desea ejecutar el programa
set /p ejecutar="¿Desea ejecutar el programa ahora? (s/n): "
if /i "%ejecutar%"=="s" (
    echo.
    echo Ejecutando el decodificador...
    echo.
    .\bin\prt7_decodificador.exe
)

pause