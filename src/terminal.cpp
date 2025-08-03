#include "terminal.h"
#include <unistd.h>
#include <iostream>

Terminal::Terminal() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        throw std::runtime_error("Failed to get terminal attributes");
    }

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
   // raw.c_oflag &= ~OPOST;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        throw std::runtime_error("Failed to set raw mode");
    }

    is_raw_enabled = true;
}


void Terminal :: clear() {
    std::cout<< "\033[H\033[2J\033[3J";
    std::cout.flush();
}


void Terminal :: refresh(const std::string& text) {
    clear();
    std::cout<<text;
    std::cout.flush();
}


void Terminal :: save_state() {
    std::cout << "\033[s";
    std::cout << "\033[?47h";
    std::cout.flush();
}


void Terminal :: restore_state() {
    std::cout << "\033[?47l";     
    std::cout << "\033[u";
    std::cout.flush();
}


void Terminal :: delete_last() {
    std:: cout << "\033[1D";
}


Terminal::~Terminal() {
    if (is_raw_enabled) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        is_raw_enabled = false;
    }
}