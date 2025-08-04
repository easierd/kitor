cmake_minimum_required(VERSION 3.10)
project(Kitor)

add_executable(kitor src/main.cpp src/kitor.cpp src/terminal.cpp src/gap_buffer.cpp)

add_executable(test test/gap_buffer_test.cpp src/gap_buffer.cpp)