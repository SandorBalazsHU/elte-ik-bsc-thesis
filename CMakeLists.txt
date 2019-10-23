# Compilers and libraries setup
#INCLUDE(CMakeForceCompiler)
#CMAKE_FORCE_C_COMPILER(gcc GNU)
#CMAKE_FORCE_CXX_COMPILER(g++ GNU)
#set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")
set(CMAKE_GENERATOR "Eclipse CDT4 - MinGW Makefiles")
set(SDL2_DIR "C:/DEV/ENV/LIB/SDL2-2.0.10/i686-w64-mingw32")
set(SDL2_INCLUDE "${SDL2_DIR}/include/SDL2")
set(SDL2_LIB "${SDL2_DIR}/lib")
set(SDL2_BIN "${SDL2_DIR}/bin")
set(SDL2_LIBRARIES "mingw32 SDL2 SDL2main")

#Project basic configuration
cmake_minimum_required(VERSION 3.16)
project(SandorBalazsBSCThesis CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(TARGET_NAME TrafficModeller)
message("${CMAKE_PROJECT_NAME} - Build started!")

#Folder structure
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/release/build")
#SUBDIRS(slib dlib app)

#Commands debug print
#SET(CMAKE_VERBOSE_MAKEFILE on)

# Building flags
set (DEBUG_FLAG   1)
set (TEST_FLAG    0)
set (RELEASE_FLAG 0)
configure_file ("${PROJECT_SOURCE_DIR}/code/settings.h.in" "${PROJECT_SOURCE_DIR}/code/settings.h")

#Link executable
AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/code" SRC)
#find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE})
link_directories(${SDL2_LIB})
add_executable(${TARGET_NAME} ${SRC})
target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})