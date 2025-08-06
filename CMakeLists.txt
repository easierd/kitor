cmake_minimum_required(VERSION 3.10)
project(Kitor)

add_executable(kitor src/main.cpp src/kitor.cpp src/terminal.cpp src/gap_buffer.cpp src/utf8_reader.cpp)

target_compile_options(kitor PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(kitor PRIVATE -fsanitize=address)

add_executable(gap_buffer_test test/gap_buffer_test.cpp src/gap_buffer.cpp)
add_executable(utf8_test test/utf8_reader_test.cpp src/utf8_reader.cpp)