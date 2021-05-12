cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0072 NEW)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
project(aixa VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

enable_testing()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -pthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if ((NOT DEFINED AIXA_DIR) OR (NOT IS_DIRECTORY ${AIXA_DIR}))
    message(FATAL_ERROR "Dependencies directory has not been set, or does not point to a valid directory")
endif()

set(DEPS_DIR ${AIXA_DIR}/deps CACHE PATH "Directory with dependencies")

include(cmake/qt.cmake)
include(cmake/freetype.cmake)
include(cmake/functions.cmake)
include(cmake/tests.cmake)

add_subdirectory(src)

add_custom_target(RunTests ${CMAKE_CTEST_COMMAND})