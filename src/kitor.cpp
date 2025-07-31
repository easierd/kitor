#include <iostream>
#include <fstream>
#include <unistd.h>

#include <string>

#include "raw_mode.h"

int main(int argc, char ** argv) {

    if (argc != 2) {
        return 1;
    }

    std :: ofstream outfile(argv[1]);

    std :: string text{""};

    RawMode raw;
    raw.enable();

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

    outfile << text;
    outfile.close();
    return 0;

}
