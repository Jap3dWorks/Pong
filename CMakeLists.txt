cmake_minimum_required(VERSION 3.16)
project(Pong2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pong2 src/main.cpp)

include_directories(D:/libraries/glad/include)
target_link_directories(D:/libraries/glad/lib)

include_directories(D:/libraries/glfw3/include)
target_link_directories(D:/libraries/glfw3/lib)

include_directories(D:/libraries/stb_image/include)

