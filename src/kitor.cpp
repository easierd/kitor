#include "kitor.h"
#include "input.cpp"

#include <iostream>


Kitor::Kitor(const std::string& filename) : 
                os{std::ofstream{filename}} {
    terminal.save_state();
    terminal.clear();
}


void Kitor::run() {
    bool is_terminated = false;
    while(!is_terminated) {

        auto seq = reader.next_input();
        if (seq.length() > 1) {
            terminal.put(seq);
            continue;
        }

        char c = seq[0];
        auto input = static_cast<Input>(c);

        switch (input) {
            case Input::CTRL_C: 
                is_terminated = true;
                break;

            case Input::DEL: 
                terminal.delete_last();
                break;
            
            case Input::ESC:
                c = reader.next_input()[0];
                if (c == '[') {
                    c = reader.next_input()[0];
                    switch (c) {
                    case 'D':
                        terminal.cursor_left();
                        break;
                    case 'C':
                        terminal.cursor_right();
                        break;
                    case 'A':
                        terminal.cursor_up();
                        break;
                    case 'B':
                        terminal.cursor_down();
                    default:
                        break;
                    }
                }
                break;

            default:
                terminal.put(seq);
                break;
        }
    }
}


Kitor::~Kitor() {
    os << terminal.get_out();
    os.close();
    terminal.restore_state();
}