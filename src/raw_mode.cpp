#include "raw_mode.h"

void RawMode :: enable() {
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        throw std::runtime_error("Failed to get terminal attributes");
    }

    struct termios raw = original_termios;

    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
   // raw.c_oflag &= ~OPOST;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        throw std::runtime_error("Failed to set raw mode");
    }

    raw_enabled = true;
}

void RawMode :: disable() {
    if (raw_enabled) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
        raw_enabled = false;
    }
}

RawMode :: ~RawMode() {
    disable();
}
