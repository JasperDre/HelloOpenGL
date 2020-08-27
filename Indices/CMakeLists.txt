set(SRC_DIR "Source")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(Indices ${SRC})

target_compile_options(Indices PRIVATE /W4)

target_include_directories(Indices PRIVATE "${GLFW_DIR}/include")
target_link_libraries(Indices "glfw")

target_include_directories(Indices PRIVATE "${GLAD_BIN_DIR}/include")
target_link_libraries(Indices "glad")

target_include_directories(Indices PRIVATE "${ASSIMP_DIR}/include")
target_link_libraries(Indices "assimp")

target_include_directories(Indices PRIVATE "${GLM_DIR}")

target_include_directories(Indices PRIVATE "${STB_DIR}")

target_include_directories(Indices PRIVATE "${TINYOBJ_DIR}")