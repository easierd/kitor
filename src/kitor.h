#pragma once

#include <fstream>
#include <string>

#include "terminal.h"
#include "utf8_reader.h"

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
