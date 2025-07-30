#include<cassert>

#include<termios.h>
#include<unistd.h>
#include<iostream>

#include "../src/raw_mode.h"

void test_raw_mode() {
    struct termios before, during, after;
    tcgetattr(STDIN_FILENO, &before);

    {
      RawMode raw;  
      raw.enable();

      tcgetattr(STDIN_FILENO, &during);

      assert(!(during.c_lflag & ECHO));
      assert(!(during.c_lflag & ICANON));
    }

    tcgetattr(STDIN_FILENO, &after);

    assert(before.c_lflag == after.c_lflag);
}


int main(void) {
    test_raw_mode();
}
