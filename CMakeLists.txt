cmake_minimum_required(VERSION 3.10)
project(Kitor)

add_executable(kitor src/kitor.cpp src/raw_mode.cpp)

enable_testing()

add_executable(kitor_test test/kitor_test.cpp src/raw_mode.cpp)

add_test(NAME kitor_tests COMMAND kitor_test)

