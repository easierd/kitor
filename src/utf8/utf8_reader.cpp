#include "utf8_reader.h"

#include<stdexcept>
#include<unistd.h>
#include<iostream>


// determine the utf8 sequence's length from
// the first byte
int seq_length(char c) {
    if ((c & '\x80') == '\x00') return 1;
    if ((c & '\xe0') == '\xc0') return 2;
    if ((c & '\xf0') == '\xe0') return 3;
    if ((c & '\xf8') == '\xf0') return 4;
    return -1; 
}

UTF8CodePoint UTF8Reader :: next_input() {
    std::string input;

    char c{0};
    int n{0};

    if((n = read(STDIN_FILENO, &c, 1)) == -1) {
        throw std::runtime_error{"Unable to read from stdin"};
    }

    // nothing's been read
    if (n == 0) {
        return input;
    }

    input += c;

    int seq_len = seq_length(c);
    if (seq_len == - 1) {
        // TODO: log error
        return input;
    }

    for (int i = 1; i < seq_len; i++) {
        if ((n = read(STDIN_FILENO, &c, 1)) == -1) {
            throw std::runtime_error{"Unable to read from stdin"};
        }

        if (n == 0) {
            break;
        }

        input += c;
    }

    return UTF8CodePoint{input};
}



std::vector<UTF8CodePoint> UTF8Reader::read_buffer(const std::vector<char>& buffer) {
    std::vector<UTF8CodePoint> utf8_buffer{};
    for (auto i = 0U; i < buffer.size(); i++) {
        std::string input{};
        char c {buffer[i]};
        auto seq_len {seq_length(c)};

        input += c;

        if (seq_len == -1) {
            // TODO: log error
            utf8_buffer.push_back(UTF8CodePoint{input});
            continue;
        }

        while(--seq_len > 0) {
            input += buffer[i++];
        }

        utf8_buffer.push_back(UTF8CodePoint{input});
    }

    return utf8_buffer;
}
