set(SRC_DIR "Source")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(Model ${SRC})

target_compile_options(Model PRIVATE /W4)

target_include_directories(Model PRIVATE "${GLFW_DIR}/include")
target_link_libraries(Model "glfw")

target_include_directories(Model PRIVATE "${GLAD_BIN_DIR}/include")
target_link_libraries(Model "glad")

target_include_directories(Model PRIVATE "${ASSIMP_DIR}/include")
target_link_libraries(Model "assimp")

target_include_directories(Model PRIVATE "${GLM_DIR}")

target_include_directories(Model PRIVATE "${STB_DIR}")

target_include_directories(Model PRIVATE "${TINYOBJ_DIR}")