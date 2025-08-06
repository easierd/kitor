// read utf8 input from stdin
// it handles multi-byte sequences

#pragma once

#include "code_point.h"

class UTF8Reader {
    public:
        UTF8CodePoint next_input();
};