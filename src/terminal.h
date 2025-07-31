#pragma once

#include <termios.h>

class Terminal {
    public:
        Terminal();
        ~Terminal();
    private:
        struct termios orig_termios;
        bool is_raw_enabled;
};