cmake_minimum_required(VERSION 3.13)
project(alchemy CXX C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(alchemy main.cpp ut.c ut.h features.h)
target_include_directories(alchemy PRIVATE ./)
add_subdirectory(disassembler)
add_subdirectory(arm)
