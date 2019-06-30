file(GLOB_RECURSE SRC "*.cpp" "*.h")

add_executable(alsaPlayground ${SRC})

target_include_directories(alsaPlayground PUBLIC . ${QT_INCLUDES})

target_link_libraries(alsaPlayground
        asound
        ${QT_LIBS})