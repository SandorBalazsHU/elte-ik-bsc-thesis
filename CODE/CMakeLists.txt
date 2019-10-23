#
cmake_minimum_required(VERSION 3.16)
project(SandorBalazsBSCThesis VERSION)
message("SandorBalazsBSCThesis - Build started!")

# Compilers and libraries setup
SET(CMAKE_C_COMPILER    C:\MinGW\bin)
SET(CMAKE_CXX_COMPILER  C:\MinGW\bin)
SET(SDL2_INCLUDE_DIRS   C:\DEV\ENV\LIB\SDL2)
SET(SDL2_INCLUDE_DIRS   C:\DEV\ENV\LIB\SDL2\lib)
SET(SDL2_LIBRARIES      SDL2main SDL2)

# Building flags
set (DEBUG_FLAG   1)
set (TEST_FLAG    0)
set (RELEASE_FLAG 0)
configure_file (
  "${PROJECT_SOURCE_DIR}/settings.h"
)

#Include SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#Link executable
add_executable(SandorBalazsBSCThesis TrafficModeller.cpp)
target_link_libraries(SandorBalazsBSCThesis ${SDL2_LIBRARIES})