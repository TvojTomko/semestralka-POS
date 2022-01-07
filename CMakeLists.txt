cmake_minimum_required(VERSION 3.18)
project(semestralka)

set(CMAKE_CXX_STANDARD 14)

add_executable(semestralka main.cpp jsonParser.cpp jsonParser.h)

target_link_libraries(semestralka json-c)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak ")
