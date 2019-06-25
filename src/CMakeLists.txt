file(GLOB_RECURSE SRC "*.cpp" "*.h")

include_directories(${Qt5Core_INCLUDE_DIRS})

add_executable(alsaPlayground ${SRC})

target_link_libraries(alsaPlayground
        asound
        Qt5::Core)