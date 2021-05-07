build_sources_list("${SRCS}")

set(TARGET_NAME mainlib)
add_library(${TARGET_NAME} SHARED ${SRCS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/platform)
target_compile_definitions(${TARGET_NAME} PRIVATE LIB_EXPORTING)

target_include_directories(${TARGET_NAME} PUBLIC
        ${ROOT_INCLUDE_DIR}
        ${QT_INCLUDES}
        ${FREETYPE_INCLUDE_DIRS})

if (${MINGW})
    target_link_directories(${TARGET_NAME} PUBLIC ${FREETYPE_LINK_DIR})
    target_link_libraries(${TARGET_NAME} opengl32)
endif()

target_link_libraries(${TARGET_NAME}
        ${QT_LIBS}
        freetype)

set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${AIXA_DIR}/build/debug/src
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${AIXA_DIR}/build/release/src
)

# Despite their name, these classes are closer of being auxiliary applications than actual unit tests
add_subdirectory(math/dft/test)
add_subdirectory(audio/note/test)
