cmake_minimum_required(VERSION 3.14)

#set(CMAKE_VERBOSE_MAKEFILE ON)

project(alsaPlayground)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -pthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/qt.cmake)
include(cmake/functions.cmake)

add_subdirectory(src)

add_custom_target(RunTests ${CMAKE_CTEST_COMMAND})