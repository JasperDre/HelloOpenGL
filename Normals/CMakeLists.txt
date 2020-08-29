set(SRC_DIR "Source")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.hpp"
)

add_executable(Normals ${SRC})

target_compile_options(Normals PRIVATE /W4)

target_include_directories(Normals PRIVATE "${GLFW_DIR}/include")
target_link_libraries(Normals "glfw")

target_include_directories(Normals PRIVATE "${GLAD_BIN_DIR}/include")
target_link_libraries(Normals "glad")

target_include_directories(Normals PRIVATE "${ASSIMP_DIR}/include")
target_link_libraries(Normals "assimp")

target_include_directories(Normals PRIVATE "${GLM_DIR}")

target_include_directories(Normals PRIVATE "${STB_DIR}")

target_include_directories(Normals PRIVATE "${TINYOBJ_DIR}")