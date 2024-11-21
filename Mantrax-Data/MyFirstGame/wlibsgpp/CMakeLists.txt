cmake_minimum_required(VERSION 3.10)
project(MantraxRuntimeCore)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set("Project_Name" MantraxRuntimeCore)

if (MSVC)
    add_compile_options(/MP /w)
    add_compile_options(/bigobj)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

# Buscar archivos fuente
file(GLOB_RECURSE GARIN_CORE_SOURCES "../cpplibs/Mantrax-Core/src/*.cpp")
file(GLOB_RECURSE GARIN_SCENE_SOURCES "../cpplibs/Mantrax-Scene/src/*.cpp")
file(GLOB_RECURSE GARIN_UI_SOURCES "../cpplibs/Mantrax-UI/src/*.cpp" "../cpplibs/Mantrax-UI/includes/*.cpp" "../cpplibs/Mantrax-UI/src/UIComponents/*.cpp")
file(GLOB_RECURSE GARIN_VR_SOURCES "../cpplibs/Mantrax-VR/src/*.cpp")
file(GLOB_RECURSE GARIN_GRAPHICS_SOURCES "../cpplibs/Mantrax-Graphics/src/*.cpp" "../cpplibs/Mantrax-Graphics/libs/glad.c")
file(GLOB_RECURSE GARIN_MATHS_SOURCES "../cpplibs/Mantrax-Maths/src/*.cpp")
file(GLOB_RECURSE GARIN_PHYSICS_SOURCES "../cpplibs/Mantrax-Physics/src/*.cpp")
file(GLOB_RECURSE GARIN_AUDIO_SOURCES "../cpplibs/Mantrax-Audio/src/*.cpp")
file(GLOB_RECURSE GARIN_GLAD "../cpplibs/Mantrax-Graphics/libs/glad.c")
file(GLOB_RECURSE GARIN_UFBX "../cpplibs/Mantrax-Graphics/libs/ufbx.c")
file(GLOB_RECURSE GARIN_SCRIPTING "../cpplibs/Mantrax-Scripting/src/*.cpp")
file(GLOB_RECURSE GARIN_ENGINE "../cpplibs/Mantrax-Engine/src/*.cpp")
file(GLOB_RECURSE GARIN_RUNTIME "../cpplibs/GarinGameCore.cpp" "../cpplibs/InputWrapper.cpp" "../clscpp/*.cpp")

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
    ${GARIN_UFBX}
    ${GARIN_SCRIPTING}
    ${GARIN_ENGINE}
    ${GARIN_RUNTIME}
)
set(SOURCES
    ../cpplibs/Mantrax-Engine/src/main.cpp
)

# Crear la biblioteca compartida (DLL)
add_library(${Project_Name} SHARED ${ALL_SOURCES})

# Incluir directorios
target_include_directories(${Project_Name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-UI/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-UI/includes/UIComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-VR/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Physics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Audio/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/Mantrax-Scripting/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../clscpp/
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/
)

# Enlazar bibliotecas
file(GLOB_RECURSE GARIN_LIBS
    "../cpplibs/Mantrax-Core/libs/*.lib"
    "../cpplibs/Mantrax-Scene/libs/*.lib"
    "../cpplibs/Mantrax-UI/libs/*.lib"
    "../cpplibs/Mantrax-VR/libs/*.lib"
    "../cpplibs/Mantrax-Graphics/libs/*.lib"
    "../cpplibs/Mantrax-Maths/libs/*.lib"
    "../cpplibs/Mantrax-Physics/libs/*.lib"
    "../cpplibs/Mantrax-Audio/libs/*.lib"
    "../cpplibs/Mantrax-Scripting/libs/*.lib"
)

target_compile_definitions(${Project_Name} PRIVATE GARINLIBS_EXPORTS)
target_link_libraries(${Project_Name} PRIVATE ${GARIN_LIBS})