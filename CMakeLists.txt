cmake_minimum_required(VERSION 3.14)
project(alsaPlayground)

set(CMAKE_CXX_STANDARD 14)

add_executable(alsaPlayground main.cpp)

target_link_libraries(alsaPlayground asound)