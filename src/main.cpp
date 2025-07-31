#include<string>
#include "kitor.h"

int main(int argc, char ** argv) {

    if (argc != 2) {
        return 1;
    }

    std::string filename {argv[1]};

    Kitor editor {filename};
    editor.run();

    return 0;

}
