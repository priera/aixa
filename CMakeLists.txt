cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0072 NEW)

#set(CMAKE_VERBOSE_MAKEFILE ON)

project(aixa)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -pthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#install libfreetype6-dev

include(cmake/functions.cmake)
include(cmake/qt.cmake)
include(cmake/log4cplus.cmake)

set(FREETYPE_INCLUDE_DIRS /usr/include/freetype2 /usr/include/libpng16)

add_subdirectory(src)

add_custom_target(RunTests ${CMAKE_CTEST_COMMAND})