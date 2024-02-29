@echo off

set "archivo=Garin-Engine.exe"

echo The compilation process takes time depending on the speed of your computer.
echo By
echo UltraHStudios.com
call :CompileCPPProyect
cd bin
if exist %archivo% (
    echo File compiled successfully by running it.
    Garin-Engine
    pause
    exit
) else (
    echo Error during compilation file does not exist.
    pause
)

:CompileCPPProyect
mingw32-make
