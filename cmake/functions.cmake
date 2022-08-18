function(CREATE_SYMLINKS)
    foreach(DIRNAME shaders assets)
        message("simlink shaders")
        set (source "${CMAKE_SOURCE_DIR}/src/${DIRNAME}")
        set (destination "${CMAKE_CURRENT_BINARY_DIR}/${DIRNAME}")
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
                DEPENDS ${destination}
                COMMENT "symbolic link resources folder from ${source} => ${destination}"
        )
    endforeach()
endfunction()