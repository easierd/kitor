#include "gap_buffer.h"

#include <stdexcept>
#include <cstring>

#include <iostream>

GapBuffer :: GapBuffer() : GapBuffer(GapBuffer::DEFAULT_SIZE) {
}


GapBuffer :: GapBuffer(int sz) {
    if (sz < 0) {
        throw std::length_error{"Buffer size must be positive."};
    }

    this->sz = sz;
    this->text = std::make_unique<char[]>(sz);
    this->l = 0;
    this->r = sz;
}


// return the actual text without the gap
std::string GapBuffer :: to_string() {
    auto str = std::make_unique<char[]>(sz - r + l);
    std::memcpy(str.get(), text.get(), l);
    std::memcpy(str.get() + l, text.get() + r, sz - r);
    
    return std::string(str.get(), sz - r + l);
}


// return the size of the entire buffer, including the gap
int GapBuffer :: size() {
    return sz;
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


// insert character at the current position and move the cursor.
// if the gap is empty, expand the buffer
void GapBuffer :: insert(char c) {
    if (l == r) {
        expand();
    }
    text[l++] = c;
}


// delete the character at the left of the cursor
// there's no need to clean the gap
void GapBuffer :: del() {
    if (l > 0) {
        l--;
    }
}


// expand the buffer to double its size
void GapBuffer :: expand() {
    auto grown_buffer= std::make_unique<char[]>(sz * 2);
    std::memcpy(grown_buffer.get(), text.get(), l);
    std::memcpy(grown_buffer.get() + r + sz, text.get() + r, sz - r);

    r = r + sz;
    sz *= 2;
    text = std::move(grown_buffer);
}