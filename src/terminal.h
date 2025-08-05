#pragma once

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

#include "gap_buffer.h"
/* 
    Interface to the low-level terminal i/o.
*/

class Terminal {

    // class invariant: the terminal's cursor and
    // the gap buffer's gap are always aligned

    public:
        Terminal();
        void clear();
        
        void delete_last();

        void cursor_left();
        void cursor_right();

        void putchar(char);
        void putseq(const std::string&);

        void save_state();
        void restore_state();

        std::string get_out();

        int winrows();
        int wincols();

        ~Terminal();
    private:
        struct termios orig_termios;
        struct winsize w;

        GapBuffer buffer;

        bool is_raw_enabled;

        void sync_cursor();
        void redraw();
};