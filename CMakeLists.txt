cmake_minimum_required(VERSION 3.5)

# C++ VERSION (17, 20)
set(CMAKE_CXX_STANDARD 17)

# C++ STANDARD REQUIRED
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PROJECT NAME
set(project_name "Garin_Engine")

# PROJECT VERSION
project(${project_name} VERSION 1.0.0)

# ADD DEFINITION NO SEE WARNING
add_definitions(-w)

# ADD DEFINITIONS NO WARNINGS
add_compile_options(/wd4701)

# ADD DEFINITIONS
add_definitions(-DGLM_ENABLE_EXPERIMENTAL -D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DWin32_LEAN_AND_MEAN)


# GARIN-ENGINE MAINS CPP
file(GLOB SOURCES
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
    "garin-audio/src/*.cpp"
    "garin-dll/projects/test/clscpp/*.cpp"
)

# COMPILE NAME
add_executable(${project_name} ${SOURCES})

# GARIN INCLUDES PATH
target_include_directories(${project_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-core/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-engine/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-graphics/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-scene/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-ui/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-vr/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-maths/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-physics/includes/physx
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-physics/includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-audio/includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/garin-dll/projects/test/clscpp/
)

# GARIN-ENGINE LIBS PATH
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

# Define la ubicación de los DLLs
file(GLOB DLLS "garin-physics/libs/*.dll")

foreach(dll ${DLLS})
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${dll}
            $<TARGET_FILE_DIR:${project_name}>)
endforeach()

target_link_libraries(${project_name} PRIVATE ${LIBRARIES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/GR)
endif()

# Define la carpeta de salida
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)

# Función para copiar un directorio si no existe
function(copy_directory_if_not_exists src_dir dst_dir)
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${dst_dir}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${src_dir} ${dst_dir}
        COMMAND ${CMAKE_COMMAND} -E echo "Copying ${src_dir} to ${dst_dir}"
        COMMENT "Checking and copying ${src_dir} to ${dst_dir} if it does not exist")
endfunction()

# Copiar directorios específicos y su contenido si no existen
copy_directory_if_not_exists(${CMAKE_CURRENT_SOURCE_DIR}/garin-dll ${OUTPUT_DIRECTORY})
copy_directory_if_not_exists(${CMAKE_CURRENT_SOURCE_DIR}/garin-dll/projects ${OUTPUT_DIRECTORY}/projects)
copy_directory_if_not_exists(${CMAKE_CURRENT_SOURCE_DIR}/garin-dll/data ${OUTPUT_DIRECTORY}/data)
copy_directory_if_not_exists(${CMAKE_CURRENT_SOURCE_DIR}/garin-dll/assets ${OUTPUT_DIRECTORY}/assets)
copy_directory_if_not_exists(${CMAKE_CURRENT_SOURCE_DIR}/garin-data/icons ${OUTPUT_DIRECTORY}/icons)
copy_directory_if_not_exists(${CMAKE_CURRENT_SOURCE_DIR}/garin-data/packages ${OUTPUT_DIRECTORY}/packages)
