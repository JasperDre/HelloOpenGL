cmake_minimum_required(VERSION 3.9)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

project(HelloOpenGL)

set(DEP_DIR "${CMAKE_SOURCE_DIR}/Dependencies")

# GLFW
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

add_subdirectory(Triangle)
add_subdirectory(Cube)
add_subdirectory(Model)

# Debugging
message(STATUS "This is the BINARY directory " ${PROJECT_BINARY_DIR})
message(STATUS "This is the SOURCE directory " ${PROJECT_SOURCE_DIR})