cmake_minimum_required(VERSION 3.17)
project(tehs)

set(CMAKE_C_STANDARD 99)

add_executable(tehs src/tehs.c libtehs/fork_process.c src/tehs.h)

set(CMAKE_BUILD_TYPE "Debug" )
