#include "code_point.h"

UTF8CodePoint::UTF8CodePoint(const std::string& s) : sequence{s} {}

std::string UTF8CodePoint::to_string() {
    return sequence;
}