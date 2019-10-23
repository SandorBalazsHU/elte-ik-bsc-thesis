# Compilers and libraries setup
#set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")
set(SDL2_DIR "C:/DEV/ENV/LIB/SDL2")
set(SDL2_INCLUDE "${SDL2_INCLUDE_DIRS}/include/SDL2")
set(SDL2_LIB "${SDL2_INCLUDE_DIRS}/lib")
set(SDL2_BIN "${SDL2_INCLUDE_DIRS}/bin")
set(SDL2_LIBRARIES "SDL2 SDL2main SDL2_image")

#Project basic configuration
cmake_minimum_required(VERSION 3.16)
project(SandorBalazsBSCThesis CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(TARGET_NAME TrafficModeller)
message("${CMAKE_PROJECT_NAME} - Build started!")

#Folder structure
SET(EXECUTABLE_OUTPUT_PATH ../release/build)
#SUBDIRS(slib dlib app)

#Commands debug print
SET(CMAKE_VERBOSE_MAKEFILE on)

# Building flags
set (DEBUG_FLAG   1)
set (TEST_FLAG    0)
set (RELEASE_FLAG 0)
configure_file ("${PROJECT_SOURCE_DIR}/settings.h")

#Link executable
AUX_SOURCE_DIRECTORY(. SRC)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE})
link_directories(${SDL2_LIB})
add_executable(${TARGET_NAME} ${SRC})
target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})