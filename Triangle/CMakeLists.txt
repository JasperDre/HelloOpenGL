set(SRC_DIR "Source")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(Triangle ${SRC})

target_compile_options(Triangle PRIVATE /W4)

target_include_directories(Triangle PRIVATE "${GLFW_DIR}/include")
target_link_libraries(Triangle "glfw")

target_include_directories(Triangle PRIVATE "${GLAD_BIN_DIR}/include")
target_link_libraries(Triangle "glad")

target_include_directories(Triangle PRIVATE "${GLM_DIR}")