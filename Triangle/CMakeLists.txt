set(SRC_DIR "Source")
set(DEP_DIR "${CMAKE_SOURCE_DIR}/Dependencies")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(Triangle ${SRC})

# GLFW
set(GLFW_DIR "${DEP_DIR}/GLFW")
add_subdirectory("${GLFW_DIR}" GLFW)
target_include_directories(Triangle PRIVATE "${GLFW_DIR}/include")
target_link_libraries(Triangle "glfw")

# GLAD
set(GLAD_DIR "${DEP_DIR}/GLAD")
add_subdirectory("${GLAD_DIR}" GLAD)
target_include_directories(Triangle PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/GLAD/include")
target_link_libraries(Triangle "glad")

# GLM
set(GLM_DIR "${DEP_DIR}/GLM")
target_include_directories(Triangle PRIVATE "${GLM_DIR}")

# Debugging
message(STATUS "This is the BINARY directory " ${PROJECT_BINARY_DIR})
message(STATUS "This is the SOURCE directory " ${PROJECT_SOURCE_DIR})