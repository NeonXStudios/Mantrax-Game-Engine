cmake_minimum_required(VERSION 3.5)

#C++ VERSION (17, 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#PROYECT NAME
set(proyect_name "GarinEngine")


project(${proyect_name} VERSION 1.0.0)



add_definitions(-w)
add_compile_options(/wd4701)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL -D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DWin32_LEAN_AND_MEAN)

file(GLOB SOURCES
    #GARIN-ENGINE RUN THE GAME ENGINE
    "garin-engine/src/*.cpp"
    "garin-graphics/libs/glad.c"
    "garin-core/src/*.cpp"
    "garin-scene/src/*.cpp"
    "garin-ui/src/*.cpp"
    "garin-ui/includes/*.cpp"
    "garin-vr/src/*.cpp"
    "garin-graphics/src/*.cpp"
    "garin-maths/src/*.cpp"
    "garin-physics/src/*.cpp"
)

add_executable(${proyect_name} ${SOURCES})

target_include_directories(${proyect_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-ui/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-vr/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-physics/includes
)

file(GLOB LIBRARIES
    #GARIN-ENGINE RUN THE GAME ENGINE
    "garin-engine/libs/*.lib"
    "garin-core/libs/*.lib"
    "garin-scene/libs/*.lib"
    "garin-ui/libs/*.lib"
    "garin-vr/libs/*.lib"
    "garin-graphics/libs/*.lib"
    "garin-maths/libs/*.lib"
    "garin-physics/libs/*.lib"
)

target_link_libraries(${proyect_name} PRIVATE ${LIBRARIES})