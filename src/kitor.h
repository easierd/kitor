#pragma once

#include <fstream>
#include <string>

#include "terminal/terminal.h"
#include "utf8/utf8_reader.h"

class Kitor {
    public:
        Kitor(const std::string& filename);
        void run();
        ~Kitor();
    private:
        std :: ofstream os;
        UTF8Reader reader;
        Terminal terminal;
};
