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
        // ascii 3 is Ctrl-C
        if (c == 3) {
            break;
        }
        terminal.refresh(text);
        text.push_back(c);
    }
}


Kitor::~Kitor() {
    os << text;
    os.close();
    terminal.restore_state();
}