cmake_minimum_required(VERSION 3.7)
project(SO_projekt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -lpanel -lcurses -pthread ")
set(SOURCE_FILES main.cpp)
add_executable(SO_projekt ${SOURCE_FILES})