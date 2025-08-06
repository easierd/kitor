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

    ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
}


void Terminal :: clear() {
    std::cout<< "\033[H\033[2J\033[3J";
    std::cout.flush();
}


// insert the code point into the gap buffer
// and draw it on the screen
void Terminal::put(const UTF8CodePoint& ucp) {
    buffer.insert(ucp);
    std::cout<<ucp.to_string();
    redraw();
}


// clear the screen from the cursor position to the end of the line, 
// rewrite the right substring of the buffer
void Terminal :: redraw() {
    std::cout << "\033[0J";
    std::cout << buffer.right_substring();
    sync_cursor();
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
    if (buffer.get_l() % wincols() == wincols() - 1) {
        sync_cursor();
        std::cout << " ";
        std::cout.flush();
    } else {
        std::cout << "\033[D \033[D";
    }
    sync_cursor();
    redraw();

}


void Terminal :: cursor_left() {
    buffer.left();
    sync_cursor();
}


void Terminal:: cursor_right() {
    buffer.right();
    sync_cursor();
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


// return the screen's height
int Terminal::winrows() {
    return w.ws_row;
}


// return the screen's width
int Terminal::wincols() {
    return w.ws_col;
}


// align buffer gap with the terminal cursor - guarantees the class invariant
void Terminal :: sync_cursor() {
    int x = 1 + buffer.get_l() / wincols();
    int y = 1 + buffer.get_l() % wincols(); 
    std::cout << "\033[" + std::to_string(x) + ";" + std::to_string(y) + "f" << std :: flush;
}