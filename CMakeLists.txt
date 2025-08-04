cmake_minimum_required(VERSION 3.10)
project(Kitor)

add_executable(kitor src/main.cpp src/kitor.cpp src/terminal.cpp src/gap_buffer.cpp)

target_compile_options(kitor PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(kitor PRIVATE -fsanitize=address)

add_executable(test test/gap_buffer_test.cpp src/gap_buffer.cpp)