cmake_minimum_required(VERSION 3.16)

cmake_path(GET CMAKE_SOURCE_DIR PARENT_PATH parentPath)
set(LIBRARIES ${parentPath}/libraries)
if(NOT EXISTS ${LIBRARIES})
    error("Libraries path %s{LIBRARIES} doesn't exists!")
endif()

SET(PROJECT_NAME Pong)
project(${PROJECT_NAME}
        VERSION 0.0.1
        )

set(CMAKE_CXX_STANDARD 20)

message("Add Default log definitions")
add_compile_definitions(
        __LOG_INFO__
        __LOG_WARNING__
        __LOG_ERROR__
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Enable Debug logger")
    add_compile_definitions(
            __LOG_DEBUG__
    )
endif()

include_directories("./include")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Pong/core/actor.cpp
        src/Pong/core/collider.cpp
        src/Pong/core/level.cpp
        src/Pong/core/map.cpp
        src/Pong/core/component.cpp
        )

# --External Libraries--
message("Include glfw")
include_directories(${LIBRARIES}/glfw3/glfw-3.3.7.bin.WIN64/include)
message("Link glfw3")
target_link_libraries(
        ${PROJECT_NAME} PRIVATE general ${LIBRARIES}/glfw3/glfw-3.3.7.bin.WIN64/lib-vc2019/glfw3.lib
)

# compile and linking glad
message("Compile and linking glad")
add_library(glad ${LIBRARIES}/glad/lib/glad.c)
target_include_directories(glad PUBLIC ${LIBRARIES}/glad/include)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PRIVATE glad)

message("Include stb_image")
include_directories(${LIBRARIES}/stb)

message("Include glm")
include_directories(${LIBRARIES}/glm/include)

message("Include assimp")
include_directories(${LIBRARIES}/assimp/include)
message("Link assimp")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/assimp/lib/assimp-vc140-mt.lib)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("copy Textures")
    set (source "${CMAKE_SOURCE_DIR}/Textures")
    set (destination "${CMAKE_CURRENT_BINARY_DIR}/Textures")
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
            DEPENDS ${destination}
            COMMENT "symbolic link resources folder from ${source} => ${destination}"
    )

    message("copy Shaders")
    set (source "${CMAKE_SOURCE_DIR}/Shaders")
    set (destination "${CMAKE_CURRENT_BINARY_DIR}/Shaders")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} => ${destination}"
    )
endif()

# ----------
# -- Test --
# ----------
project(TEST
        VERSION 0.1)

add_executable(
        TEST
        ./test/test.cpp
)

message("Compile and linking glad")
target_link_libraries(TEST PRIVATE glad)


# -------------
# -- gl_test --
# -------------
project(GL_TEST
        VERSION 0.1)

add_executable(${PROJECT_NAME} ./test/gl_test.cpp)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE general ${LIBRARIES}/glfw3/glfw-3.3.7.bin.WIN64/lib-vc2019/glfw3.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/assimp/lib/assimp-vc140-mt.lib)