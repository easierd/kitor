/* This class represents utf-8 code points as a whole entity, 
   managing multi-byte sequences. 
   Instances are created by the reader, and they're read-only for all other components. 
*/

#pragma once

#include<string>

class UTF8Reader;

class UTF8CodePoint {

    friend class UTF8Reader;

    public:
        UTF8CodePoint() = default;
        UTF8CodePoint (const UTF8CodePoint&) = default;
        UTF8CodePoint& operator=(const UTF8CodePoint&) = default;
        UTF8CodePoint(UTF8CodePoint&&) = default;
        UTF8CodePoint& operator=(UTF8CodePoint&&) = default;
        ~UTF8CodePoint() = default;

        std::string to_string() const; 
        int length() const;
        char operator[] (int) const;
    private:
        UTF8CodePoint(const std::string&);

        std::string sequence;
};
