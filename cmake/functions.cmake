function(build_sources_list SRCS)
    file(GLOB_RECURSE  SOURCEFILES CONFIGURE_DEPENDS *.cpp *.h *.cu *.cuh *.hpp *.ui *.qrc *.rc)
    #traverse unit test files and remove from sources
    file(GLOB_RECURSE SRCS_UTEST CONFIGURE_DEPENDS  *_UTest.* )
    if(SRCS_UTEST)
        list(REMOVE_ITEM SOURCEFILES ${SRCS_UTEST})
    endif()
    set (SRCS ${SOURCEFILES} PARENT_SCOPE)
endfunction()

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

function(win_deploy_target TARGET_NAME)
    set(SRC_BUILD_DIR ${AIXA_DIR}/build/${CMAKE_BUILD_TYPE}/src)

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${Qt5_BIN_DIR}/windeployqt.exe --plugindir ${SRC_BUILD_DIR}/plugins ${SRC_BUILD_DIR}/${TARGET_NAME}.exe)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${FREETYPE_LINK_DIR}/freetype.dll ${SRC_BUILD_DIR})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${Qt5_BIN_DIR}/Qt5Multimedia.dll ${SRC_BUILD_DIR})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${Qt5_BIN_DIR}/Qt5Network.dll ${SRC_BUILD_DIR})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${Qt5_BIN_DIR}/../plugins/audio ${SRC_BUILD_DIR}/plugins/audio)
endfunction()