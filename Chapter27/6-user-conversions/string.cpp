// class String, for char arrays
//      -- from _C++26 for Lazy Programmers_

#include <cstring>
#include "string.h"

using namespace std;

String& String::operator=(String&& other) noexcept // move =
{
    if (contents_) delete[] contents_;

    contents_ = other.contents_;
    other.contents_ = nullptr;

    return *this;
}

String& String::operator=(const String& other)
{
    if (this == &other) return *this;      // never assign *this to itself
    if (contents_) delete[] contents_; copy(other.c_str());
    return *this;
}

void String::copy(const char* str)
{
    if (!str) str = ""; //Probably shouldn't let them send in nullptr
    //  If they do, treat it as ""

    contents_ = new char[strlen(str) + 1]; // get new memory
    // The +1 is room for final '\0'
    strcpy(contents_, str);                // copy contents over
}

String& String::operator+=(const String& other)
{
    char* oldContents = contents_;

    contents_ = new char[size() + other.size() + 1];
    // 1 extra space at the end for the null char

    strcpy(contents_, oldContents);  // copy old into new
    strcat(contents_, other.c_str());// append other contents

    delete[] oldContents;

    return *this;
}

void String::read(std::istream& in)
{
    static char buffer[BIGGEST_READABLE_STRING_PLUS_ONE];
    in >> buffer;
    *this = buffer;
}