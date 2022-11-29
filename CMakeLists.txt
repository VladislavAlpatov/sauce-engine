cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)


link_directories("$ENV{DXSDK_DIR}Lib/x86")
include_directories("$ENV{DXSDK_DIR}Include")
add_compile_definitions(WIN32_LEAN_AND_MEAN)

project(sauce-engine)


add_executable(sauce-engine src/main.cpp)
add_subdirectory(src)


target_link_libraries(sauce-engine PRIVATE d3dx9)
target_link_libraries(sauce-engine PRIVATE d3d9)


find_package(freetype CONFIG REQUIRED)
target_link_libraries(sauce-engine PRIVATE freetype)
