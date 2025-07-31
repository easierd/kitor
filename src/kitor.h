#pragma once

#include <fstream>
#include <string>

#include "terminal.h"

class Kitor {
    public:
        Kitor(const std::string& filename);
        void run();
        ~Kitor();
    private:
        std:: string text;
        std:: ofstream os;
        Terminal terminal;
};
