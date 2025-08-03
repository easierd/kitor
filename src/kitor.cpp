#include "kitor.h"

#include <iostream>


Kitor::Kitor(const std::string& filename) : 
                text {""},
                os{std::ofstream{filename}} {}


void Kitor::run() {
    terminal.save_state();
    terminal.clear();

    char c;
    while(std:: cin.get(c)) {
        if (c == 3) {   // Ctrl-C
            break;
        }

        if (c == 127) { // Del
            terminal.delete_last();

            if (!text.empty()) {
                text.pop_back();
            }
            terminal.refresh(text);
            
            continue;
        }

        text.push_back(c);
        terminal.refresh(text);
    }
}


Kitor::~Kitor() {
    os << text;
    os.close();
    terminal.restore_state();
}