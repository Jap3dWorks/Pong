cmake_minimum_required(VERSION 3.16)
SET(PROJECT_NAME Pong)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

# log levels
add_compile_definitions(
        __LOG_DEBUG__
        __LOG_INFO__
        __LOG_WARNING__
        __LOG_ERROR__
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Core/Actor.cpp
        src/Core/Collider.cpp
        src/Core/Level.cpp
        src/Graphic/Material.cpp
#        include/Core/Render.h
        src/Core/Render.cpp
        src/Core/Scene.cpp
        src/Core/Shape.cpp
        src/Core/Component.cpp
        src/Core/Utils.cpp
#        include/Graphic/Lights.h
        src/Core/DataComparers.cpp
#        include/Core/DataComparers.h
        src/Levels/blending_level.cpp
#        include/Levels/blending_level.h
#        include/sandbox.h
        )

include_directories("./include")

# --External Libraries--
set(LIBRARIES D:/libraries)

# compile and linking glad
message("Compile and linking glad")
add_library(glad "${LIBRARIES}/glad/include/glad/glad.h"  "${LIBRARIES}/glad/lib/glad.c")
target_include_directories(glad PUBLIC ${LIBRARIES}/glad/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

message("Include glfw")
include_directories(${LIBRARIES}/glfw3/include)
message("Link glfw3")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/glfw3/lib/glfw3.lib)

message("Include stb_image")
include_directories(${LIBRARIES}/stb)

message("Include glm")
include_directories(${LIBRARIES}/glm/include)

message("Include assimp")
include_directories(${LIBRARIES}/assimp/include)
message("Link assimp")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/assimp/lib/assimp-vc140-mt.lib)

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