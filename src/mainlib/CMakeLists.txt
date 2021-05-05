set (SRCS "")
build_sources_list("${SRCS}")

set(TARGET_NAME mainlib)

add_library(${TARGET_NAME} SHARED ${SRCS})

target_include_directories(${TARGET_NAME} PUBLIC
        ${ROOT_INCLUDE_DIR}
        ${QT_INCLUDES}
        ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(${TARGET_NAME}
        ${QT_LIBS}
        freetype)

# Despite their name, these classes are closer of being auxiliary applications than actual unit tests
add_subdirectory(math/dft/test)
add_subdirectory(audio/note/test)
