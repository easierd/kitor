/*
 *  An implementation of the gap buffer data structure.
 *  It's used to efficiently manage text insertion and deletion
 *  following the cursor
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../utf8/code_point.h"

class GapBuffer{

    // class invariant: l <= r && sz >= 0
    //                  newlines[] is ordered

    public:
        GapBuffer();
        GapBuffer(int sz);

        int get_l() const;
        int prev_newline() const;
        int size() const;

        const std::vector<int> get_newlines() const;

        void insert(const UTF8CodePoint&);
        void del();
        void left();
        void right();

        std::string to_string();
        std::string right_substring();

        void expand();

    private:
        static constexpr int DEFAULT_SIZE {1024};

        int sz;
        std::unique_ptr<UTF8CodePoint[]> text;
        std::vector<int> newlines;
        int l;
        int r;
};
