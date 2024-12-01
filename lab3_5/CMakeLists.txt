cmake_minimum_required(VERSION 3.15)
project(lab3_5)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(lab3_5 main.cpp)
target_link_libraries(lab3_5 PRIVATE OpenMP::OpenMP_CXX)