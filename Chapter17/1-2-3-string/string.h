// String class
//        -- from _C++26 for Lazy Programmers_

#ifndef STRING_H
#define STRING_H

#include <cstring> // uses cstring functions all over
#include <iostream>

#ifdef _MSC_VER
#pragma warning (disable:4996) // Disable a warning about strcpy, etc.,
#endif                         //  in Visual Studio

class String 
{
public:
    static constexpr std::size_t BIGGEST_READABLE_STRING_PLUS_ONE   = 257; 
                      // biggest string we can read, incl '\0'
    static int biggestReadableString()
    { 
        return BIGGEST_READABLE_STRING_PLUS_ONE - 1; 
    }

    String(const char* other="") { copy(other);             }
    String(const String &other) : String (other.c_str())   {}
                      // a "delegated" constructor
    ~String()         { if (contents_) delete [] contents_;  }

    // access function
    const char* c_str() const { return contents_;        }  

    // functions related to size
    std::size_t size () const { return strlen (c_str()); }
                      // Inefficient! Is there a better way?
    bool operator!   () const { return ! size();         } 

    // assignment
    String& operator= (const String& other);
    String& operator+=(const String& other);

    // []
    char  operator[] (int index) const { return contents_[index]; }
    char& operator[] (int index)       { return contents_[index]; }

    //I/O
    void  read (std::istream& in ); 
    void  print(std::ostream& out) const{ out << c_str();        }
private:
    char* contents_;
    void  copy(const char* str);
};

//I/O
inline
std::istream& operator>>(std::istream& in, String& foo)
{
    foo.read (in); return in;
}

inline
std::ostream& operator<<(std::ostream& out, const String& foo)
{
    foo.print(out); return out;
}

// comparisons
inline
bool operator==(const String& a, const String& b)
{
    return strcmp(a.c_str(), b.c_str()) == 0;
}

inline
int operator<=>(const String& a, const String& b)
                   // automagically generates <, <=, >, and >=
{
    return strcmp(a.c_str(), b.c_str());
}

//arithmetic ops
inline
String operator+(const String& a, const String& b)
{
    String result = a; result += b; return result;
}
#endif //STRING_H
