set(SRC_DIR "Source")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(Cube ${SRC})

target_include_directories(Cube PRIVATE "${GLFW_DIR}/include")
target_link_libraries(Cube "glfw")

target_include_directories(Cube PRIVATE "${GLAD_BIN_DIR}/include")
target_link_libraries(Cube "glad")

target_include_directories(Cube PRIVATE "${GLM_DIR}")