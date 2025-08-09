/*
 *  An implementation of the gap buffer data structure.
 *  It's used to efficiently manage text insertion and deletion
 *  following the cursor
*/

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "../utf8/code_point.h"

class GapBuffer{

    // class invariant: l <= r && sz >= 0
    //                  newlines[] is ordered

    public:
        GapBuffer();

        int get_l() const;
        int prev_newline() const;
        int next_newline() const;
        int size() const;

        const std::vector<int> get_newlines() const;

        int insert(const UTF8CodePoint&);
        void del();
        void left();
        void right();
        void up();
        void down();

        std::string to_string();
        std::string substring(int, int) const;

        void expand();

    private:
        static constexpr int DEFAULT_SIZE {1024};
        static constexpr size_t DEFAULT_TAB_SIZE {4};
        static constexpr bool DEFAULT_TAB_EXPAND {true};

        int sz;
        std::unique_ptr<UTF8CodePoint[]> text;
        std::vector<int> newlines;
        int l;
        int r;

        bool is_enabled_tab_expand;
        int tab_size;

        int _insert(const UTF8CodePoint&, std::vector<int>::iterator&);
        void adjust_newline(int, std::vector<int>::iterator&);
    
};
