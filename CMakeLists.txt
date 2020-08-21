cmake_minimum_required(VERSION 3.9)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

project(HelloOpenGL)

set(DEP_DIR "${CMAKE_SOURCE_DIR}/Dependencies")

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
set(GLFW_DIR "${DEP_DIR}/GLFW")
add_subdirectory("${GLFW_DIR}" GLFW)

# GLAD
set(GLAD_DIR "${DEP_DIR}/GLAD")
set(GLAD_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/GLAD")
add_subdirectory("${GLAD_DIR}" GLAD)

# GLM
set(GLM_DIR "${DEP_DIR}/GLM")

# STB
set(STB_DIR "${DEP_DIR}/STB")

# TINYOBJ
set(TINYOBJ_DIR "${DEP_DIR}/TINYOBJ")

# ASSIMP
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build tools" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Build tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build tools" FORCE)
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "Build tools" FORCE)
set(ASSIMP_DIR "${DEP_DIR}/ASSIMP")
add_subdirectory("${ASSIMP_DIR}" ASSIMP)

add_subdirectory(Triangle)
add_subdirectory(Cube)
add_subdirectory(Model)

# Debugging
message(STATUS "This is the BINARY directory " ${PROJECT_BINARY_DIR})
message(STATUS "This is the SOURCE directory " ${PROJECT_SOURCE_DIR})