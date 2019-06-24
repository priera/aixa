cmake_minimum_required(VERSION 3.14)

project(alsaPlayground)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive")

add_subdirectory(src)
