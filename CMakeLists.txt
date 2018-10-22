cmake_minimum_required(VERSION 3.12)
project(libtrie C)
set(CMAKE_C_STANDARD 99)

include_directories(./utils/)
include_directories(./src/)
include_directories(/usr/include/datrie/)
add_executable(tests src/single_list.c src/libtrie.c tests/tests.c)
TARGET_LINK_LIBRARIES(tests /usr/lib/x86_64-linux-gnu/libdatrie.so)
