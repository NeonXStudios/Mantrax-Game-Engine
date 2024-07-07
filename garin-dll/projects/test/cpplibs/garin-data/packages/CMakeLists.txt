cmake_minimum_required(VERSION 3.10)
project(GarinGameCore)

set(CMAKE_CXX_STANDARD 17)

# Fuentes del proyecto


file(GLOB SOURCES 
    "*.cpp"
    "../../garin-engine/src/*.cpp"
    "../../garin-graphics/libs/glad.c"
    "../../garin-core/src/*.cpp"
    "../../garin-scene/src/*.cpp"
    "../../garin-ui/src/*.cpp"
    "../../garin-ui/includes/*.cpp"
    "../../garin-vr/src/*.cpp"
    "../../garin-graphics/src/*.cpp"
    "../../garin-maths/src/*.cpp"
    "../../garin-physics/src/*.cpp"
    "../../garin-audio/src/*.cpp"
    "../../garin-dll/projects/test/clscpp/*.cpp"
)

list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/../../garin-engine/src/main.cpp")

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
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

file(GLOB LIBRARIES
    "../../garin-engine/libs/*.lib"
    "../../garin-core/libs/*.lib"
    "../../garin-scene/libs/*.lib"
    "../../garin-ui/libs/*.lib"
    "../../garin-vr/libs/*.lib"
    "../../garin-graphics/libs/*.lib"
    "../../garin-maths/libs/*.lib"
    "../../garin-physics/libs/*.lib"
    "../../garin-audio/libs/*.lib"
)

# Definiciones de compilación
target_compile_definitions(GarinGameCore PRIVATE GARINGAMECORE_EXPORTS)
target_link_libraries(GarinGameCore PRIVATE ${LIBRARIES})
