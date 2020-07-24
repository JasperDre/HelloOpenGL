# Minimum CMake version
cmake_minimum_required(VERSION 3.9)

# Generated project name
project(HelloOpenGL)

# C++ standard (C++17)
set(CMAKE_CXX_STANDARD 17)

# Add folders for the generated projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

file(GLOB_RECURSE SRC
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(${PROJECT_NAME} ${SRC})

# GLFW
set(GLFW_DIR "${DEP_DIR}/glfw")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw")

# GLAD
set(GLAD_DIR "${DEP_DIR}/glad")
add_subdirectory("${GLAD_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/dependencies/glad/include")
target_link_libraries(${PROJECT_NAME} "glad")

# Debugging
message(STATUS "This is the BINARY directory " ${PROJECT_BINARY_DIR})
message(STATUS "This is the SOURCE directory " ${PROJECT_SOURCE_DIR})