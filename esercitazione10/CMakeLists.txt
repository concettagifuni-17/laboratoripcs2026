cmake_minimum_required(VERSION 3.20)
project(esercitazione10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

find_package(Eigen3 REQUIRED)

list(APPEND libraries_list PUBLIC Eigen3::Eigen)

add_executable(grad_coniugato grad_coniugato.cpp)

target_link_libraries(grad_coniugato ${libraries_list})