cmake_minimum_required(VERSION 3.16)
SET(PROJECT_NAME Pong)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Actor.cpp
        src/Collider.cpp
        src/Component.cpp
        src/Level.cpp
        src/Material.cpp
        src/pch.cpp
        src/Render.cpp
        src/Scene.cpp
        src/Shape.cpp
        src/Utils.cpp)

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
#add_dependencies(${PROJECTNAME} glfw3.lib)

message("Include stb_image")
include_directories(${LIBRARIES}/stb)

message("Include glm")
include_directories(${LIBRARIES}/glm/include)

message("Include assimp")
include_directories(${LIBRARIES}/assimp/include)
message("Link assimp")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/assimp/lib/assimp-vc140-mt.lib)

#message("Try find package OpenGL")
#find_package(OpenGL REQUIRED)
#if(NOT OPENGL_FOUND)
#    message(ERROR "OPENGL not found!")
#endif(NOT OPENGL_FOUND)
#target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OpenGL})

