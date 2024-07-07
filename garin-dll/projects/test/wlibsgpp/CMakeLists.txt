cmake_minimum_required(VERSION 3.10)
project(GarinGameCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FS")

file(GLOB SOURCES 
    "../clscpp/*.cpp"
    "../cpplibs/*.cpp"
    "../cpplibs/garin-engine/src/*.cpp"
    "../cpplibs/garin-graphics/libs/glad.c"
    "../cpplibs/garin-core/src/*.cpp"
    "../cpplibs/garin-scene/src/*.cpp"
    "../cpplibs/garin-ui/src/*.cpp"
    "../cpplibs/garin-ui/includes/*.cpp"
    "../cpplibs/garin-vr/src/*.cpp"
    "../cpplibs/garin-graphics/src/*.cpp"
    "../cpplibs/garin-maths/src/*.cpp"
    "../cpplibs/garin-physics/src/*.cpp"
    "../cpplibs/garin-audio/src/*.cpp"
)

# list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/../cpplibs/garin-engine/src/main.cpp")

add_library(GarinGameCore SHARED ${SOURCES})

if (MSVC)
    # Añade la opción ForceSynchronousPDBWrites
    add_compile_options(/FS)
endif()

target_include_directories(GarinGameCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-ui/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-vr/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-physics/includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/garin-audio/includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpplibs/
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

file(GLOB LIBRARIES
    "../cpplibs/garin-engine/libs/*.lib"
    "../cpplibs/garin-core/libs/*.lib"
    "../cpplibs/garin-scene/libs/*.lib"
    "../cpplibs/garin-ui/libs/*.lib"
    "../cpplibs/garin-vr/libs/*.lib"
    "../cpplibs/garin-graphics/libs/*.lib"
    "../cpplibs/garin-maths/libs/*.lib"
    "../cpplibs/garin-physics/libs/*.lib"
    "../cpplibs/garin-audio/libs/*.lib"
)

# Definiciones de compilación
target_compile_definitions(GarinGameCore PRIVATE GARINGAMECORE_EXPORTS)
target_link_libraries(GarinGameCore PRIVATE ${LIBRARIES})
