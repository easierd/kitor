#include "kitor.h"
#include "input.cpp"

#include <iostream>


Kitor::Kitor(const std::string& filename) : 
                os{std::ofstream{filename}} {
    terminal.save_state();
    terminal.clear();
}


void Kitor::run() {
    char c;
    bool is_terminated = false;
    while((!is_terminated) && (std::cin.get(c))) {

        auto input = static_cast<Input>(c);
        switch (input) {
            case Input::CTRL_C: 
                is_terminated = true;
                break;

            case Input::DEL: 
                terminal.delete_last();
                terminal.refresh();
                break;
            
            case Input::ESC:
                std::cin.get(c);
                if (c == '[') {
                    std::cin.get(c);
                    switch (c) {
                    case 'D':
                        terminal.cursor_left();
                        break;
                    
                    default:
                        break;
                    }
                }
                break;

            default:
                terminal.putchar(c);
                terminal.refresh();
                break;
        }
    }
}


Kitor::~Kitor() {
    os << terminal.get_out();
    os.close();
    terminal.restore_state();
}