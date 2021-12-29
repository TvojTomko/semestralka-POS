cmake_minimum_required(VERSION 3.18)
project(semestralka-POS)

set(CMAKE_CXX_STANDARD 14)

add_executable(semestralka-POS main.cpp)
target_link_libraries(semestralka-POS pthread)
