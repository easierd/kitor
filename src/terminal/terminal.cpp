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
    // raw.c_iflag &= ~(ICRNL);
    // raw.c_oflag &= ~OPOST;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        throw std::runtime_error("Failed to set raw mode");
    }

    is_raw_enabled = true;
    hidden_row = 0;

    ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
}


void Terminal :: clear() {
    std::cout<< "\033[H\033[2J\033[3J";
    std::cout.flush();
}


// insert the code point into the gap buffer
// and draw it on the screen
void Terminal::put(const UTF8CodePoint& ucp) {
    auto inserted = buffer.insert(ucp);
    auto s = buffer.substring(buffer.get_l() - inserted, last_visible() - (buffer.get_l() - inserted) + 1);

    std::cout << "\033[0J" 
        << "\033[?25l" 
        << s
        << std::flush;

    sync_cursor();
}


void Terminal::put_buffer(const std::vector<UTF8CodePoint>& utf8_buffer) {
    for (auto& ucp : utf8_buffer) {
        put(ucp);
    }
}


// clear the screen from the cursor position to the end of the line, 
// rewrite the right substring of the buffer
void Terminal :: redraw() {
    std::cout << "\033[0J" << std::flush;
    std::cout << "\033[?25l" << std::flush;
    std::cout << buffer.substring(buffer.get_l(), last_visible() - buffer.get_l() + 1) ;
    sync_cursor();
}


void Terminal::full_redraw() {
        int fv = first_visible();
        int lv = last_visible();
        auto s = buffer.substring(fv, lv - fv + 1);
        if (s.back() == '\n') {
            s.pop_back();
        }
        std::cout<< "\033[H\033[2J\033[3J" 
            << "\033[?25l" 
            << s;

}


int Terminal::first_visible() {
    int h = hidden_row;
    int max_f = h * wincols();
    auto n = buffer.get_newlines();
    int f = n[0] + 1;

    for (size_t i = 1; h > 0 && i < n.size(); i++) {
        if (n[i] > max_f) break;
        
        h -= 1 + (n[i] - f) / wincols(); 
        f = n[i] + 1; 
    }

    return f + h * wincols();
}


int Terminal::last_visible() {
    int v = winrows();
    int l = first_visible() - 1;
    auto n = buffer.get_newlines();

    for (size_t i = 1; v > 0 && i < n.size(); i++) {
        if (n[i] < l) continue;
        v -= 1 + (n[i] - (l + 1)) / wincols();
        l = n[i];
    }

    l += wincols() * v;
    return l;
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


void Terminal::cursor_up() {
    buffer.up();
    sync_cursor();
}


void Terminal::cursor_down() {
    buffer.down();
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
    int n = buffer.prev_newline();
    auto newlines = buffer.get_newlines();

    int x = 1 + (buffer.get_l() - (n + 1)) / wincols() ;
    for (size_t i = 1; i < newlines.size() && newlines[i] <= n; i++) {
        x += 1 + (newlines[i] - (newlines[i - 1] + 1)) / wincols();
    }

    x -= hidden_row;

    int y = 1 + (buffer.get_l() - (n + 1)) % wincols();

    if (x <= 0) {
        hidden_row -= (-x + 1);
        x += -x + 1;
        full_redraw();
    }

    if (x > winrows()) {
        hidden_row+= (x - winrows());
        x -= (x - winrows());
        full_redraw();
    }    

    std::cout << "\033[" + std::to_string(x) + ";" + std::to_string(y) + "f" << std :: flush;
    std::cout << "\033[?25h" << std::flush;
}