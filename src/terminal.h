#pragma once

#include <termios.h>
#include <string>

#include "gap_buffer.h"
/* 
    Interface to the low-level terminal i/o.
*/

class Terminal {
    public:
        Terminal();
        void refresh();
        void clear();
        void delete_last();
        void cursor_left();

        void putchar(char c);

        void save_state();
        void restore_state();

        std::string get_out();

        ~Terminal();
    private:
        struct termios orig_termios;
        GapBuffer buffer;
        bool is_raw_enabled;
};