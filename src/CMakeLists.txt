file(GLOB_RECURSE SRC "*.cpp" "*.h")

add_executable(alsaPlayground ${SRC})

target_link_libraries(alsaPlayground asound)