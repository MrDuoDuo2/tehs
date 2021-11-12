cmake_minimum_required(VERSION 3.17)
project(tehs)

set(CMAKE_CXX_STANDARD 14)

add_executable(tehs src/main.c libtehs/fork_process.c)

set(CMAKE_BUILD_TYPE "Debug" )

set(CMAKE_CXX_FLAGS -pthread)