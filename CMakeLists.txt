cmake_minimum_required(VERSION 3.18)
project(semestralka)
set(CMAKE_CXX_STANDARD 20)


add_executable(semestralka main.cpp console.cpp console.h download.cpp download.h downloadHandler.cpp downloadHandler.h jsonParser.cpp jsonParser.h fileHandler.cpp fileHandler.h)
target_link_libraries(semestralka pthread)
target_link_libraries(semestralka crypto ssl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak ")




target_link_libraries(semestralka json-c)
