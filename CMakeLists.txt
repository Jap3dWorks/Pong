cmake_minimum_required(VERSION 3.16)

SET(PROJECT_NAME Pong)
project(${PROJECT_NAME}
        VERSION 0.0.1
        )

include(cmake/functions.cmake)

set(LIBRARIES ${CMAKE_SOURCE_DIR}/libraries)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(NOT EXISTS ${LIBRARIES})
    error("Libraries path %s{LIBRARIES} doesn't exists!")
endif()

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(${LIBRARIES}/glfw3/glfw-3.3.7.bin.WIN64/include)
message("Include glm")
include_directories(${LIBRARIES}/glm/include)
message("Include stb_image")
include_directories(${LIBRARIES}/stb)
message("Include assimp")
include_directories(${LIBRARIES}/assimp/include)
message("Include doctest")
include_directories(${LIBRARIES}/doctest/doctest)
message("Include boost")
find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(src)

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
            __LOG_INFO__
            __LOG_WARNING__
            __LOG_ERROR__
    )
endif()



# -- Test and gltest --
# ---------------------
add_subdirectory(test)


