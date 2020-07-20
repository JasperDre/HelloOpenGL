# Minimum CMake version
cmake_minimum_required(VERSION 3.7)

# Generated project name
project(HelloOpenGL)

# C++ standard (C++17)
set(CMAKE_CXX_STANDARD 17)

# Add folders for the generated projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

# Custom macro function to add project filters
macro(MACRO_PROJECT_FILTER path input_list prepend)
  foreach(_source IN ITEMS ${${input_list}})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${prepend}/${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
  endforeach()
endmacro()

file(GLOB_RECURSE SRC
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

MACRO_PROJECT_FILTER(${SRC_DIR} SRC Source)

add_executable(${PROJECT_NAME} ${SRC})

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:RELEASE>")

# GLFW
set(GLFW_DIR "${DEP_DIR}/glfw")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build shared libraries")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(GLFW_VULKAN_STATIC OFF CACHE INTERNAL "Assume the Vulkan loader is linked with the application")
set(GLFW_USE_HYBRID_HPG ON CACHE INTERNAL "Force use of high-performance GPU on hybrid systems")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw")

# GLAD
set(GLAD_DIR "${DEP_DIR}/glad")
set(GLAD_PROFILE "compatibility" CACHE STRING "OpenGL profile")
set(GLAD_API "" OFF CACHE STRING "API type/version pairs")
set(GLAD_GENERATOR "c" OFF CACHE STRING "Language to generate the binding for")
set(GLAD_SPEC "gl" CACHE STRING "Name of the spec")
set(GLAD_EXTENSIONS "" CACHE STRING "Path to extensions file or comma separated list of extensions")
set(GLAD_ALL_EXTENSIONS OFF CACHE INTERNAL "Include all extensions instead of those specified by GLAD_EXTENSIONS")
set(GLAD_NO_LOADER OFF CACHE INTERNAL "No loader")
set(GLAD_REPRODUCIBLE OFF CACHE INTERNAL "Reproducible build")
set(GLAD_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLAD_DIR}")
target_link_libraries(${PROJECT_NAME} "glad" "${GLAD_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")

# Testing
enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# Debugging
message(STATUS "This is the BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is the SOURCE dir " ${PROJECT_SOURCE_DIR})