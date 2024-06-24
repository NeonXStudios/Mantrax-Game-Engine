cmake_minimum_required(VERSION 3.22.1)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-Wall)

project("myapplication")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-core/src/CORE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-core/src/DEBUG)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-core/src/ECS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-core/src/LIGHTS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-core/src/MODEL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-graphics/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/garin-scene/src)

# Agrega los archivos de GLM a tu proyecto
add_library(glm STATIC IMPORTED)
set_target_properties(glm PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/libglm.a)

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(myapplication SHARED
        main.cpp
        AndroidOut.cpp
        Renderer.cpp
        Shader.cpp
        TextureAsset.cpp
        Utility.cpp)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)

# Configure libraries CMake uses to link your target library.
target_link_libraries(myapplication
        # La actividad del juego
        game-activity::game-activity

        # EGL y otras bibliotecas dependientes necesarias para el dibujo
        # e interacción con el sistema Android
        EGL
        GLESv3
        jnigraphics
        android
        log

        # Enlaza la biblioteca GLM y otra-lib
        glm
        )
