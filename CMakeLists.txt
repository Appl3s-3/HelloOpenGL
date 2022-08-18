cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

# ---- Project Info ----
set(PROJECT_NAME "prj")
set(CMAKE_BUILD_TYPE Debug)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/libs/include"
)

# ---- Project Executable ----
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})


# Add .lib files
#link_directories(${CMAKE_SOURCE_DIR}/libs)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_SOURCE_DIR}/dependencies/glfw-3.3.8")
target_link_libraries(${PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# ---- External Dependency Path ----
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/cmake")

# ---- Include Directories (containing .h files) ----

# -------- Libraries --------
add_library(GLAD STATIC "libs/glad/src/gl.c")
#set(NAME "name") # Static/Dynamic


set(LIBSRC)
set(LIBSTATIC)
set(LIBDYNAMIC)
set(LIBS ${LIBSRC} ${LIBSTATIC} ${LIBDYNAMIC})

target_link_libraries(${PROJECT_NAME} ${LIBS})
