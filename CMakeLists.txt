cmake_minimum_required(VERSION 3.10)
set (project_name "Garin_Engine")
project(${project_name})

if (MSVC)
    add_compile_options(/w)
endif()

set(CMAKE_CXX_STANDARD 17)


if (MSVC)
    # Deshabilitar la advertencia específica
    add_compile_options(/wd4251)
endif()


# Directorios de inclusión
file(GLOB SOURCES
    "garin-engine/src/main.cpp"
)

set(MYLIB_DIR "/")
link_directories(${MYLIB_DIR})

# Archivo ejecutable
add_executable(${project_name} ${SOURCES})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-ui/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-vr/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-physics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-audio/includes
)

file(GLOB LIBRARIES
    "garin-engine/libs/*.lib"
    "garin-core/libs/*.lib"
    "garin-scene/libs/*.lib"
    "garin-ui/libs/*.lib"
    "garin-vr/libs/*.lib"
    "garin-graphics/libs/*.lib"
    "garin-maths/libs/*.lib"
    "garin-physics/libs/*.lib"
    "garin-audio/libs/*.lib"
)

# Enlace a librerias
target_link_libraries(${project_name} PRIVATE ${LIBRARIES})

# Enlaces adicionales
target_link_libraries(${project_name} PRIVATE GarinLibrary)
