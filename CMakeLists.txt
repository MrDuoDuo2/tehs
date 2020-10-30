cmake_minimum_required(VERSION 3.17)
project(tehs)

set(CMAKE_CXX_STANDARD 14)

add_executable(tehs src/main.cc libtehs/fork_process.cc libtehs/header_unpack.cc libtehs/header_unpack.h)

set(CMAKE_BUILD_TYPE "Debug" )

set(CMAKE_CXX_FLAGS -pthread)