// read utf8 input from stdin
// it handles multi-byte sequences

#pragma once

#include<vector>

#include "code_point.h"

class UTF8Reader {
    public:
        std::vector<UTF8CodePoint> read_buffer(const std::vector<char>&);
        UTF8CodePoint next_input();
};