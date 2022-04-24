cmake_minimum_required(VERSION 3.20)
project(tehs)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_CXX_FLAGS -pthread)

include_directories(./
                    ./src/core
                    ./src/os/unix)

add_executable(tehs src/core/tehs.c )
