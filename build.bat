@echo off

set "archivo=Garin-Engine.exe"

:menu
cls
echo =====================================
echo 1. Compilar proyecto
echo 2. Iniciar programa
echo 3. Salir
echo =====================================
set /p choice="Selecciona una opcion (1, 2 o 3): "

if "%choice%"=="1" goto compile
if "%choice%"=="2" goto run_program
if "%choice%"=="3" goto exit_script
echo Opcion no valida. Intentalo de nuevo.
goto menu

:compile
echo El proceso de compilacion puede tardar dependiendo de la velocidad de tu computadora.
echo By UltraHStudios.com
call :CompileCPPProyect
if %ERRORLEVEL% neq 0 (
    echo Error durante la compilacion. Verifica el Makefile y los archivos de codigo.
    pause
    goto menu
)
cd bin
if exist %archivo% (
    echo Archivo compilado exitosamente. Ejecutando...
    Garin-Engine
    cd ..
    pause
    goto menu
) else (
    echo Error durante la compilacion. El archivo no existe.
    cd ..
    pause
    goto menu
)

:run_program
cd bin
if exist %archivo% (
    echo Ejecutando %archivo%...
    Garin-Engine
    cd ..
    goto menu
) else (
    echo Error: el archivo no existe. Por favor, compilalo primero.
    cd ..
    pause
    goto menu
)

:CompileCPPProyect
mingw32-make -j8
exit /b

:exit_script
exit
