#include "terminal.h"
#include <unistd.h>
#include <iostream>



// enable raw mode
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


void Terminal :: putchar(char c) {
    buffer.insert(c);
}


// clear screen, print the text and move the cursor to the right position
void Terminal :: refresh() {
    clear();
    std::cout << buffer.to_string();
    std::cout << "\033[" + std::to_string(buffer.get_l() + 1) + "G";
    std::cout.flush();
}


// save the terminal's state in order to restore it when the program exits
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
    buffer.del();
    std::cout << "\033[" + std::to_string(buffer.get_l() + 1) + "G";
}


void Terminal :: cursor_left() {
    buffer.left();
    std::cout << "\033[" + std::to_string(buffer.get_l() + 1) + "G";
}


// return the text managed by the gap buffer, without the gap
std::string Terminal :: get_out() {
    return buffer.to_string();
}


// disable raw mode
Terminal::~Terminal() {
    if (is_raw_enabled) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        is_raw_enabled = false;
    }
}