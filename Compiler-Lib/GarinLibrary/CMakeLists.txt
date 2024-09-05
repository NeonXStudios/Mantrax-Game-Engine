cmake_minimum_required(VERSION 3.10)
project(GarinLib)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/MP /w)
endif()

# Buscar archivos fuente
file(GLOB_RECURSE GARIN_CORE_SOURCES "../../garin-core/src/*.cpp")
file(GLOB_RECURSE GARIN_SCENE_SOURCES "../../garin-scene/src/*.cpp")
file(GLOB_RECURSE GARIN_UI_SOURCES "../../garin-ui/src/*.cpp" "../../garin-ui/includes/*.cpp" "../../garin-ui/src/UIComponents/*.cpp")
file(GLOB_RECURSE GARIN_VR_SOURCES "../../garin-vr/src/*.cpp")
file(GLOB_RECURSE GARIN_GRAPHICS_SOURCES "../../garin-graphics/src/*.cpp" "../../garin-graphics/libs/glad.c")
file(GLOB_RECURSE GARIN_MATHS_SOURCES "../../garin-maths/src/*.cpp")
file(GLOB_RECURSE GARIN_PHYSICS_SOURCES "../../garin-physics/src/*.cpp")
file(GLOB_RECURSE GARIN_AUDIO_SOURCES "../../garin-audio/src/*.cpp")
file(GLOB_RECURSE GARIN_GLAD "../../garin-graphics/libs/glad.c")
file(GLOB_RECURSE GARIN_SCRIPTING "../../garin-scripting/src/*.cpp")
file(GLOB_RECURSE GARIN_ENGINE "../../garin-engine/src/*.cpp")

# Combinar todos los archivos fuente
set(ALL_SOURCES
    ${GARIN_CORE_SOURCES}
    ${GARIN_SCENE_SOURCES}
    ${GARIN_UI_SOURCES}
    ${GARIN_VR_SOURCES}
    ${GARIN_GRAPHICS_SOURCES}
    ${GARIN_MATHS_SOURCES}
    ${GARIN_PHYSICS_SOURCES}
    ${GARIN_AUDIO_SOURCES}
    ${GARIN_GLAD}
    ${GARIN_SCRIPTING}
    ${GARIN_ENGINE}
)
set(SOURCES
    ../garin-engine/src/main.cpp
)

# Crear la biblioteca compartida (DLL)
add_library(GarinLibrary SHARED ${ALL_SOURCES})

# Incluir directorios
target_include_directories(GarinLibrary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-ui/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-ui/includes/UIComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-vr/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-physics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-audio/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../garin-scripting/includes
)

# Enlazar bibliotecas
file(GLOB_RECURSE GARIN_LIBS
    "../../garin-core/libs/*.lib"
    "../../garin-scene/libs/*.lib"
    "../../garin-ui/libs/*.lib"
    "../../garin-vr/libs/*.lib"
    "../../garin-graphics/libs/*.lib"
    "../../garin-maths/libs/*.lib"
    "../../garin-physics/libs/*.lib"
    "../../garin-audio/libs/*.lib"
    "../../garin-scripting/libs/*.lib"
)

target_compile_definitions(GarinLibrary PRIVATE GARINLIBS_EXPORTS)
target_link_libraries(GarinLibrary PRIVATE ${GARIN_LIBS})