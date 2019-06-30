set (SRCS "")
build_sources_list("${SRCS}")

set(TARGET_NAME mainlib)

add_library(${TARGET_NAME} SHARED ${SRCS})

target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_INCLUDE_DIR} ${QT_INCLUDES})

target_link_libraries(${TARGET_NAME} asound ${QT_LIBS})

add_subdirectory(audio/test)
