cmake_minimum_required(VERSION 3.10)
set (project_name "Mantrax_Engine")
project(${project_name})

if (MSVC)
    add_compile_options(/w)
    add_compile_options(/wd4251)
    add_compile_options(/bigobj)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
    "../../Mantrax-Engine/src/main.cpp"
)

set(MYLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../LIB/bin/Debug/")
link_directories(${MYLIB_DIR})

# Archivo ejecutable
add_executable(${project_name} WIN32 ${SOURCES})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-UI/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-UI/includes/UIComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-VR/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Physics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Audio/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Mantrax-Scripting/includes
)

file(GLOB LIBRARIES
    "../../Mantrax-Engine/libs/*.lib"
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

# Enlace a librerias
target_link_libraries(${project_name} PRIVATE ${LIBRARIES})

# Enlaces adicionales
target_link_libraries(${project_name} PRIVATE MantraxCore)
