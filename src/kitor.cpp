#include "kitor.h"
#include "input.cpp"

#include <iostream>


Kitor::Kitor(const std::string& filename) : 
                filename{filename} {
    
    terminal.save_state();
    terminal.clear();

    std::ifstream file{filename, std::ios::binary | std::ios::ate};
    std::streamsize size = file.tellg();
    if (size > 0) {
        file.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        file.read(buffer.data(), size);
        std::vector<UTF8CodePoint> utf8_buffer {reader.read_buffer(buffer)};
        terminal.put_buffer(utf8_buffer);
    }
}


void Kitor::run() {
    bool is_terminated = false;
    while(!is_terminated) {

        auto seq = reader.next_input();
        if (seq.length() > 1) {
            terminal.put(seq);
            continue;
        }

        char c = seq[0];
        auto input = static_cast<Input>(c);

        switch (input) {
            case Input::CTRL_C: 
                is_terminated = true;
                break;

            case Input::DEL: 
                terminal.delete_last();
                break;
            
            case Input::ESC:
                c = reader.next_input()[0];
                if (c == '[') {
                    c = reader.next_input()[0];
                    switch (c) {
                    case 'D':
                        terminal.cursor_left();
                        break;
                    case 'C':
                        terminal.cursor_right();
                        break;
                    case 'A':
                        terminal.cursor_up();
                        break;
                    case 'B':
                        terminal.cursor_down();
                    default:
                        break;
                    }
                }
                break;

            default:
                terminal.put(seq);
                break;
        }
    }
}


Kitor::~Kitor() {
    std::ofstream os{filename};
    os << terminal.get_out();
    os.close();
    terminal.restore_state();
}