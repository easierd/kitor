#pragma once

#include<termios.h>
#include<unistd.h>
#include<iostream>

class RawMode{
    private:
        struct termios original_termios;
        bool raw_enabled = false;
    public:
        void enable();
        void disable();
        ~RawMode();
};
