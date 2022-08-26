project(TEST
        VERSION 0.1)

add_executable(
        TEST
        ./test.cpp
)

message("Compile and linking glad")
target_link_libraries(TEST PRIVATE glad)

message("Link Boost")
find_package(Boost)
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(TEST LINK_PUBLIC ${Boost_LIBRARIES})

CREATE_SYMLINKS()


# -------------
# -- gl_test --
# -------------
project(GL_TEST
        VERSION 0.1)

add_executable(${PROJECT_NAME} ./gl_test.cpp)

target_link_libraries(
        ${PROJECT_NAME} PRIVATE general ${LIBRARIES}/glfw3/glfw-3.3.7.bin.WIN64/lib-vc2019/glfw3.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/assimp/lib/assimp-vc140-mt.lib)
