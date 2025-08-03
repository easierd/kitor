#pragma once

#include <termios.h>
#include <string>


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