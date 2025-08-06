cmake_minimum_required(VERSION 3.10)
project(Kitor)

# Set the C++ standard and build type BEFORE defining targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Main executable
add_executable(kitor
    src/main.cpp
    src/kitor.cpp
    src/terminal/terminal.cpp
    src/terminal/gap_buffer.cpp
    src/utf8/utf8_reader.cpp
    src/utf8/code_point.cpp
)

# Compile/link options for the main executable
target_compile_options(kitor PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(kitor PRIVATE -fsanitize=address)

# Test executable
add_executable(utf8_test
    test/utf8_reader_test.cpp
    src/utf8/code_point.cpp
    src/utf8/utf8_reader.cpp
)

# Enable warnings and sanitizers for test build too
target_compile_options(utf8_test PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(utf8_test PRIVATE -fsanitize=address)
