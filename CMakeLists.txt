cmake_minimum_required(VERSION 3.10)

project(CompilerTry2 VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -g")

set (SOURCES
    ./src/main.cpp
)

add_executable(my_compiler ${SOURCES})
