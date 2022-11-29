cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)

project(sauce-engine)


add_executable(sauce-engine src/main.cpp)
add_subdirectory(src)
