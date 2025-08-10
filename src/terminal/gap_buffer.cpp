#include "gap_buffer.h"

#include <algorithm>
#include <stdexcept>
#include <cstring>

#include <iostream>

GapBuffer :: GapBuffer() {
    this->sz = DEFAULT_SIZE;
    this->text = std::make_unique<UTF8CodePoint[]>(sz);
    this->l = 0;
    this->r = sz;

    this->newlines = {-1};

    this->tab_size = DEFAULT_TAB_SIZE;
}



// return the actual text without the gap
std::string GapBuffer :: to_string() {
    return substring(0, l) + substring(r, sz - r);
}


// return the position of the previous newline
int GapBuffer::prev_newline() const {
    auto prev = std::lower_bound(newlines.begin(), newlines.end(), l);
    if (prev == newlines.end()) 
        return newlines.back();
    
    return *(prev - 1);
}


// return the position of the next newline
int GapBuffer::next_newline() const {
    auto next = std::lower_bound(newlines.begin(), newlines.end(), l);
    if (next == newlines.end()) {
        return -1;
    }
    return *next;
}


const std::vector<int> GapBuffer::get_newlines() const{
    return newlines;
}


// return the substring [pos, pos + count) handling utf8 code units
std::string GapBuffer :: substring(int pos, int count) const{
    std::string result {};
    if (pos >= l && pos < r) {
        pos += r - l;
    }
    for (; pos < sz && count > 0; pos++) {
        if (pos >= l && pos < r) {
            continue;
        }
        result += text[pos].to_string();
        count--;
    }

    return result;
}


// return the size of the entire buffer, including the gap
int GapBuffer :: size() const {
    return sz;
}


int GapBuffer :: get_l() const {
    return l;
}


// move the cursor to the left - shift the entire gap to the left
void GapBuffer :: left() {
    if (l > 0) {
        l--;
        r--;
        text[r] = text[l];
    }
}



// move the cursor to the right - shift the entire gap to the right
void GapBuffer :: right() {
    if (r < sz) {
        text[l] = text[r];
        r++;
        l++;
    }
}


// move the cursor to the previous newline character, or to position 0 if there's none
void GapBuffer::up() {
    auto prev_nl = prev_newline();
    if (prev_nl == -1) {
        prev_nl = 0;
    }
    while (l != prev_nl) {
        left();
    }
}


// move the cursor next to the next newline character, or to the last character if there's none
void GapBuffer::down() {
    auto next_nl = next_newline();
    if (next_nl == -1) {
        while (r != sz) {
            right();
        }
    } else {
        while (l <= next_nl) {
            right();
        }
    }
}



// if the gap is empty, expand the buffer
int GapBuffer :: insert(const UTF8CodePoint& ucp) {
    if (l == r) {
        expand();
    }

    auto next_nl_pos = std::lower_bound(newlines.begin(), newlines.end(), l);

    auto inserted {_insert(ucp, next_nl_pos)};
    
    adjust_newline(inserted, next_nl_pos);

    return inserted;
}


// insert a utf8 code point at the current position
// and move the cursor according to the configuration.
int GapBuffer::_insert(const UTF8CodePoint& ucp, std::vector<int>::iterator& next_nl_pos) {
    int inserted {0};

    if (ucp.to_string() == "\t") {
            auto spaces {tab_size - (l % tab_size)};
            for (int i = 0; i < spaces; i++ ){
                text[l++] = std::move(UTF8CodePoint(" "));
                inserted++;
            }
    } else {
        if (ucp.to_string() == "\n") {
            next_nl_pos = newlines.insert(next_nl_pos, l);
            next_nl_pos++;
        }
        text[l++] = std::move(ucp);
        inserted++;
    }

    return inserted;
}


void GapBuffer::adjust_newline(int inserted, std::vector<int>::iterator& next_nl_pos) {
    while(next_nl_pos != newlines.end()) {
        (*next_nl_pos) += inserted;
        next_nl_pos++;
    }
}


// delete the character at the left of the cursor
// newlines will be updated to decrease every following newline character
// there's no need to clean the gap
void GapBuffer :: del() {
    if (l > 0) {
        l--;
        auto it = std::lower_bound(newlines.begin(), newlines.end(), l);
        
        if (text[l].to_string() == "\n") {
            it = newlines.erase(it);
        }

        while(it != newlines.end()) {
            (*it)--;
            it++;
        }

    }
}


// expand the buffer to double its size
void GapBuffer :: expand() {
    auto grown_buffer= std::make_unique<UTF8CodePoint[]>(sz * 2);
    std::move(text.get(), text.get() + l, grown_buffer.get());
    std::move(text.get() + r, text.get() + sz, grown_buffer.get() + r + sz);

    r = r + sz;
    sz *= 2;
    text = std::move(grown_buffer);
}