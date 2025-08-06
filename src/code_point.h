/* This class represents utf-8 code points as a whole entity, 
   managing multi-byte sequences */

#pragma once

#include<string>

class UTF8CodePoint {
    public:
        UTF8CodePoint();
        UTF8CodePoint(const std::string&);

        std :: string to_string(); 
    private:
        std :: string sequence;
};