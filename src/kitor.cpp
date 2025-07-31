#include "kitor.h"

#include <iostream>

Kitor::Kitor(const std::string& filename) : 
                text {""},
                os{std::ofstream{filename}} {}


void Kitor::run() {
    char c;
    while(std:: cin.get(c)) {
        // ascii 3 is Ctrl-C
        if (c == 3) {
            break;
        }
        text.push_back(c);
        std::cout<< u8"\033[2J\033[1;1H"; 
        std::cout<<text;
    }
}


Kitor::~Kitor() {
    os << text;
    os.close();
}