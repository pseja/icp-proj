cmake_minimum_required(VERSION 3.10)
project(oblivion)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
# debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Werror -Wpedantic -g")

# source files directory
add_subdirectory(src)
# external libraries directory
add_subdirectory(external)
