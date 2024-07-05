cmake_minimum_required(VERSION 3.10)
project(GarinGameCore)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES GarinGamecore.cpp)

include_directories(src)

add_library(GarinGameCore SHARED ${SOURCES})

target_compile_definitions(GarinGameCore PRIVATE GARINGAMECORE_EXPORTS)
