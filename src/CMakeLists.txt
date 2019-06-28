file(GLOB_RECURSE SRC "*.cpp" "*.h")

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_executable(alsaPlayground ${SRC})

target_include_directories(alsaPlayground PUBLIC .)

target_link_libraries(alsaPlayground
        asound
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)