#include "utf8_reader.h"

#include<stdexcept>
#include<unistd.h>
#include<iostream>

std:: string UTF8Reader :: next_input() {
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

    for (int i = 1; i < seq_len; i++) {
        if ((n = read(STDIN_FILENO, &c, 1)) == -1) {
            throw std::runtime_error{"Unable to read from stdin"};
        }

        if (n == 0) {
            break;
        }

        input += c;
    }

    return input;
}


int UTF8Reader :: seq_length(char c) {
    if ((c & '\x80') == '\x00') return 1;
    if ((c & '\xe0') == '\xc0') return 2;
    if ((c & '\xf0') == '\xe0') return 3;
    if ((c & '\xf8') == '\xf0') return 4;
    return -1; 
}