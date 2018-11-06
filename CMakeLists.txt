cmake_minimum_required(VERSION 3.12)
project(libtrie C)
set(CMAKE_C_STANDARD 11)
#SET(CMAKE_C_FLAGS "-O2")

include_directories(./utils/)
include_directories(./src/)
include_directories(/usr/include/datrie/)

# library
add_library(trie SHARED src/single_list.c src/libtrie.c)
TARGET_COMPILE_OPTIONS(trie PRIVATE -O0)

# tests
add_executable(tests tests/tests.c)
TARGET_LINK_LIBRARIES(tests trie)