cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

# ---- Project Info ----
set(PROJECT_NAME "drPlayground")
set(CMAKE_BUILD_TYPE Debug)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# ---- Project Executable ----
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# ---- Header File Directories ----
include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/libs/include"
)

# ---- External Dependency Path ----
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/cmake")

# ---- Libraries ----
# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_SOURCE_DIR}/dependencies/glfw")

find_package(OpenGL REQUIRED)

add_library(LIBGLAD STATIC "libs/glad/src/gl.c")
set(LIBPNG "${CMAKE_SOURCE_DIR}/libs/libpng.a")
set(LIBFREETYPE "${CMAKE_SOURCE_DIR}/libs/libfreetype.a")
set(LIBZ "${CMAKE_SOURCE_DIR}/libs/libz.a")

set(LIBRARIES
    glfw OpenGL::GL LIBGLAD ${LIBPNG} ${LIBFREETYPE} ${LIBZ}
)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
