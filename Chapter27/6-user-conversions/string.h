// String class
//      -- from _C++26 for Lazy Programmers_

#ifndef STRING_H
#define STRING_H

#include <cstring> // uses cstring functions all over
#include <iostream>

#ifdef _MSC_VER
#pragma warning (disable:4996) // Disable a warning about strcpy, etc.,
#endif                         //   in Visual Studio

class OutOfRangeException {};

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
    String(String&& other) noexcept   // move constructor
    {
        contents_ = other.contents_; 
        other.contents_ = nullptr;
    }
    ~String() noexcept { if (contents_) delete [] contents_; }

    // access function
    
    const char* c_str() const { return contents_;            }  
    explicit operator const char* () const { return c_str(); }

    // other functions, not operators
    
    std::size_t size () const { return strlen (c_str());     }
                      //Inefficient! Is there a better way?
    bool operator!   () const { return ! size();             } 

    // comparisons
    bool operator==(const String& other) const noexcept
    {
        return strcmp(c_str(), other.c_str()) == 0;
    }

    int operator<=>(const String& other) const noexcept
    {
        return strcmp(c_str(), other.c_str());
    }

    // assignment and concatenation

    String& operator= (const String& other);
    String& operator= (String&&      other) noexcept; //move =
    String& operator+=(const String& other);
    String  operator+ (const String& other) const
    {
        String result = *this; result += other; return result;
    }

    // [] and at
    char  operator[] (int index) const { return contents_[index]; }
    char& operator[] (int index)       { return contents_[index]; }

    char  at(int index) const
    {
        if (index >= size()) throw OutOfRangeException(); 
        else return contents_[index];
    }
    char& at(int index)
    {
        if (index >= size()) throw OutOfRangeException(); 
        else return contents_[index];
    }


    //I/O functions
    void  read (std::istream& in ); 
    void  print(std::ostream& out) const{ out << c_str();        }
private:
    char* contents_;
    void  copy(const char* str);
};

inline
std::istream& operator>>(std::istream& in, String& foo)
{
    foo.read(in); return in;
}

inline
std::ostream& operator<<(std::ostream& out, const String& foo)
{
    foo.print(out); return out;
}
#endif // STRING_H
