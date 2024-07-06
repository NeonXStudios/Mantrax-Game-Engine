cmake_minimum_required(VERSION 3.10)
project(GarinGameCore)

set(CMAKE_CXX_STANDARD 17)

# Fuentes del proyecto
set(SOURCES 
    "GarinGameCore.cpp"
    "TestDllImport.cpp"
)

# Directorios de inclusión

# Añadir la biblioteca estática
add_library(GarinGameCore SHARED ${SOURCES})

target_include_directories(GarinGameCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-ui/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-vr/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-physics/includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-audio/includes/
)



# Definiciones de compilación
target_compile_definitions(GarinGameCore PRIVATE GARINGAMECORE_EXPORTS)
