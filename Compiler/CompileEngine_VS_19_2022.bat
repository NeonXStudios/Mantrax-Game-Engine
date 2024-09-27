@echo off
cd LIB
mkdir bin
cd bin
cmake ..
msbuild .\MantraxCore.sln

cd ..\..\

REM Verifica si el directorio de destino existe, si no, lo crea.
if not exist "COMPILER\bin\Debug\" (
    mkdir "COMPILER\bin\Debug\"
)

REM Copia el archivo DLL a la carpeta de destino.
xcopy "LIB\bin\Debug\MantraxCore.dll" "COMPILER\bin\Debug\" /Y



timeout /t 2 /nobreak

cd COMPILER
mkdir bin
cd bin
cmake ..
msbuild .\Mantrax_Engine.sln
pause