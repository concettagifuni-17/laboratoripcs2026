cmake_minimum_required(VERSION 3.20)
project(esercitazione1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow")

add_executable(argcargv argcargv.cpp)
add_executable(media media.cpp)