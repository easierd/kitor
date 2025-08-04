#pragma once

#include <fstream>
#include <string>

#include "terminal.h"
#include "gap_buffer.h"

class Kitor {
    public:
        Kitor(const std::string& filename);
        void run();
        ~Kitor();
    private:
        std:: ofstream os;
        Terminal terminal;
};
