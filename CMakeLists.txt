cmake_minimum_required(VERSION 3.14)

project(alsaPlayground)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -pthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#consider using a FindQt5.cmake file
set(Qt5_DIR /home/pedro/Qt/5.12.4/gcc_64/lib/cmake/Qt5)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(src)
