#include "kitor.h"
#include "input.cpp"

#include <iostream>


Kitor::Kitor(const std::string& filename) : 
                text {""},
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
                if (!text.empty()) {
                    text.pop_back();
                }
                terminal.refresh(text);
                break;
            
            default:
                text.push_back(c);
                terminal.refresh(text);
                break;
        }
    }
}


Kitor::~Kitor() {
    os << text;
    os.close();
    terminal.restore_state();
}