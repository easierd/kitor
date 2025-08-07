/* 
    Interface to the low-level terminal i/o.
*/

#pragma once

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

#include "gap_buffer.h"
#include "../utf8/code_point.h"


class Terminal {

    // class invariant: the terminal's cursor and
    // the gap buffer's gap are always aligned

    public:
        Terminal();
        void clear();
        
        void delete_last();

        void cursor_left();
        void cursor_right();
        void cursor_up();
        void cursor_down();

        void put(const UTF8CodePoint&);

        void save_state();
        void restore_state();

        std::string get_out();

        int winrows();
        int wincols();

        ~Terminal();
    private:
        struct termios orig_termios;
        struct winsize w;

        int hidden_row;

        GapBuffer buffer;

        bool is_raw_enabled;

        void sync_cursor();
        void redraw();
        int first_visible();
        int last_visible();
};