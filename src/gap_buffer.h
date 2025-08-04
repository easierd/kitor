/*
 *  An implementation of the gap buffer data structure.
 *  It's used to efficiently manage text insertion and deletion
 *  following the cursor
*/

#pragma once

#include <memory>
#include <string>

class GapBuffer{

    // class invariant: l <= r && sz >= 0


    public:
        GapBuffer();
        GapBuffer(int sz);

        void insert(char c);
        void del();
        void left();
        void right();

        std::string to_string();

        int size();

        void expand();

    private:
        static constexpr int DEFAULT_SIZE {1024};

        int sz;
        std::unique_ptr<char[]> text;
        int l;
        int r;
};
