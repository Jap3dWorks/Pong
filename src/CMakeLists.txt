
add_executable(${PROJECT_NAME}
        main.cpp
        Pong/entity/actor.cpp
        Pong/core/collider.cpp
        Pong/core/level.cpp
        Pong/map/deprecated/map.cpp
        )

# --External Libraries--
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

message("Link assimp")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIBRARIES}/assimp/lib/assimp-vc140-mt.lib)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

CREATE_SYMLINKS()
