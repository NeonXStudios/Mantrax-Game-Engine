cmake_minimum_required(VERSION 3.10)
project(GarinEngine)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añadir las opciones de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -Wall -Wextra -pthread -DGLM_ENABLE_EXPERIMENTAL -D_DEBUG -Wattributes")

# Incluir directorios de cabeceras
include_directories(
    ${CMAKE_SOURCE_DIR}/garin-graphics/include
    ${CMAKE_SOURCE_DIR}/garin-scene/include
    ${CMAKE_SOURCE_DIR}/garin-maths/include
    ${CMAKE_SOURCE_DIR}/garin-core/include
    ${CMAKE_SOURCE_DIR}/garin-ui/include
    ${CMAKE_SOURCE_DIR}/garin-physics/include
    ${CMAKE_SOURCE_DIR}/garin-scripting/include
    ${CMAKE_SOURCE_DIR}/garin-audio/include
    ${CMAKE_SOURCE_DIR}/projects/test/include
    ${CMAKE_SOURCE_DIR}/garin-vr/include
)

# Añadir directorios de bibliotecas
link_directories(
    ${CMAKE_SOURCE_DIR}/garin-graphics/lib
    ${CMAKE_SOURCE_DIR}/garin-scene/lib
    ${CMAKE_SOURCE_DIR}/garin-maths/lib
    ${CMAKE_SOURCE_DIR}/garin-core/lib
    ${CMAKE_SOURCE_DIR}/garin-ui/lib
    ${CMAKE_SOURCE_DIR}/garin-physics/lib
    ${CMAKE_SOURCE_DIR}/garin-scripting/lib
    ${CMAKE_SOURCE_DIR}/garin-audio/lib
    ${CMAKE_SOURCE_DIR}/garin-vr/lib
)

# Añadir los archivos fuente
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/garin-engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-engine/EditorSystems/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-engine/EditorSystems/ComponentsUI/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-engine/EditorSystems/WindowsUI/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-graphics/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-core/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-scene/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-ui/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-physics/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-scripting/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-audio/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/projects/test/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-maths/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/garin-vr/src/*.cpp"
)

# Definir el ejecutable
add_executable(GarinEngine ${SOURCES})

# Enlazar las bibliotecas
target_link_libraries(GarinEngine
    garin-graphics
    garin-maths
    garin-core
    garin-ui
    garin-physics
    garin-scripting
    garin-audio
    garin-vr
)

# Añadir limpieza
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "GarinEngine")
