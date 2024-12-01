cmake_minimum_required(VERSION 3.15)
project(your_project_name)

set(CMAKE_C_STANDARD 11)

# Find MPI package
find_package(MPI REQUIRED)

# Add include directories for MPI
include_directories(${MPI_INCLUDE_PATH})

# Add your executable
add_executable(your_project_name main.c)

# Link MPI libraries
target_link_libraries(your_project_name ${MPI_C_LIBRARIES})

# Add MPI compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_C_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_C_LINK_FLAGS}")