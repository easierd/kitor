#include "gap_buffer.h"

#include <algorithm>
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
    this->text = std::make_unique<UTF8CodePoint[]>(sz);
    this->newlines = {-1};
    this->l = 0;
    this->r = sz;
}


// return the actual text without the gap
std::string GapBuffer :: to_string() {
    std::string result {};
    for (int i = 0; i < l; i++) {
        result += text[i].to_string();
    }

    result += right_substring();

    return result;
}


// return the position of the previous newline
int GapBuffer::prev_newline() const {
    auto prev = std::lower_bound(newlines.begin(), newlines.end(), l);
    if (prev == newlines.end()) 
        return newlines.back();
    
    return *(prev - 1);
}


const std::vector<int> GapBuffer::get_newlines() const{
    return newlines;
}


// return the substring from the cursor to the end of the buffer
std::string GapBuffer :: right_substring() {
    std::string result {};
    for (int i = r; i < sz; i++) {
        result += text[i].to_string();
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


// insert a utf8 code point at the current position and move the cursor.
// if the gap is empty, expand the buffer
// increase index of all the following newline characters
void GapBuffer :: insert(const UTF8CodePoint& ucp) {
    if (l == r) {
        expand();
    }

    // O(log N) guaranteed 
    auto it = std::lower_bound(newlines.begin(), newlines.end(), l);

    if (ucp.to_string() == "\n") {
        it = newlines.insert(it, l);
        it++;
    }

    while(it != newlines.end()) {
        (*it)++;
        it++;
    }

    text[l++] = std::move(ucp);
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