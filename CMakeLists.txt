cmake_minimum_required(VERSION 3.13)
project(alchemy CXX C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -pedantic -Werror)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(CMAKE_REQUIRED_INCLUDES "/usr/local/include /usr/include")
endif()

add_executable(alchemy main.cpp ut.c ut.h)
target_include_directories(alchemy PRIVATE ./)
add_subdirectory(aim)
add_subdirectory(analysis)
add_subdirectory(arm)
add_subdirectory(disassembler)
