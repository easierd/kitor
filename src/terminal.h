#pragma once

#include <termios.h>
#include <string>


class Terminal {
    public:
        Terminal();
        void refresh(const std::string&);
        void clear();
        ~Terminal();
    private:
        struct termios orig_termios;
        bool is_raw_enabled;
};