cmake_minimum_required(VERSION 3.18)
project(semestralka-POS)
set(CMAKE_CXX_STANDARD 20)


add_executable(semestralka-POS main.cpp console.cpp console.h download.cpp download.h downloadHandler.cpp downloadHandler.h)
target_link_libraries(semestralka-POS pthread)
target_link_libraries(semestralka-POS crypto ssl)
