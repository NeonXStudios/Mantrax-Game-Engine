cmake -G "Visual Studio 17 2022"

cd Compiler-Lib/GarinLibrary

cmake -G "Visual Studio 17 2022"
msbuild GarinLib.sln

cd ../../

msbuild Garin_Engine.sln