// read utf8 input from stdin
// it handles multi-byte sequences

#pragma once

#include<string>

class UTF8Reader {
    public:
        std::string next_input();
    private:
        int seq_length(char c);
};