#pragma once

#include <termios.h>
#include <string>

/* 
    Interface to the low-level terminal i/o.
*/

class Terminal {
    public:
        Terminal();
        void refresh(const std::string&);
        void clear();
        void delete_last();

        void save_state();
        void restore_state();

        ~Terminal();
    private:
        struct termios orig_termios;
        bool is_raw_enabled;
};