cmake_minimum_required(VERSION 3.10)
project(MantraxCore)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set("Project_Name" MantraxCore)

if (MSVC)
    add_compile_options(/MP /w)
    add_compile_options(/bigobj)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

# Buscar archivos fuente
file(GLOB_RECURSE GARIN_CORE_SOURCES "../../Mantrax-Core/src/*.cpp")
file(GLOB_RECURSE GARIN_SCENE_SOURCES "../../Mantrax-Scene/src/*.cpp")
file(GLOB_RECURSE GARIN_UI_SOURCES "../../Mantrax-UI/src/*.cpp" "../../Mantrax-UI/includes/*.cpp" "../../Mantrax-UI/src/UIComponents/*.cpp")
file(GLOB_RECURSE GARIN_VR_SOURCES "../../Mantrax-VR/src/*.cpp")
file(GLOB_RECURSE GARIN_GRAPHICS_SOURCES "../../Mantrax-Graphics/src/*.cpp" "../../Mantrax-Graphics/libs/glad.c")
file(GLOB_RECURSE GARIN_MATHS_SOURCES "../../Mantrax-Maths/src/*.cpp")
file(GLOB_RECURSE GARIN_PHYSICS_SOURCES "../../Mantrax-Physics/src/*.cpp")
file(GLOB_RECURSE GARIN_AUDIO_SOURCES "../../Mantrax-Audio/src/*.cpp")
file(GLOB_RECURSE GARIN_GLAD "../../Mantrax-Graphics/libs/glad.c")
file(GLOB_RECURSE GARIN_UFBX "../../Mantrax-Graphics/libs/ufbx.c")
file(GLOB_RECURSE GARIN_SCRIPTING "../../Mantrax-Scripting/src/*.cpp")
file(GLOB_RECURSE GARIN_ENGINE "../../Mantrax-Engine/src/*.cpp" "../../Mantrax-Engine/src/EngineUI/*.cpp" "../../Mantrax-Engine/src/EngineUI/UI/*.cpp")

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
)
set(SOURCES
    ../../Mantrax-Engine/src/main.cpp
)

# Crear la biblioteca compartida (DLL)
add_library(${Project_Name} SHARED ${ALL_SOURCES})

# Incluir directorios
target_include_directories(${Project_Name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Engine/includes/EngineUI
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Engine/includes/EngineUI/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-UI/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-UI/includes/UIComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-VR/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Physics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Audio/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Scripting/includes
)

# Enlazar bibliotecas
file(GLOB_RECURSE GARIN_LIBS
    "../../Mantrax-Core/libs/*.lib"
    "../../Mantrax-Scene/libs/*.lib"
    "../../Mantrax-UI/libs/*.lib"
    "../../Mantrax-VR/libs/*.lib"
    "../../Mantrax-Graphics/libs/*.lib"
    "../../Mantrax-Maths/libs/*.lib"
    "../../Mantrax-Physics/libs/*.lib"
    "../../Mantrax-Audio/libs/*.lib"
    "../../Mantrax-Scripting/libs/*.lib"
)

target_compile_definitions(${Project_Name} PRIVATE GARINLIBS_EXPORTS)
target_link_libraries(${Project_Name} PRIVATE ${GARIN_LIBS})