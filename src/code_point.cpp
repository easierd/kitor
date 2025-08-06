#include "code_point.h"

UTF8CodePoint::UTF8CodePoint(const std::string& s) : sequence{s} {}

std::string UTF8CodePoint::to_string() const {
    return sequence;
}

int UTF8CodePoint::length() const {
    return sequence.length();
}

char UTF8CodePoint::operator[](int i) const {
    return sequence[i];
}