// Vector class:  a variable-length array
//      -- from _C++ for Lazy Programmers_

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class Vector
{
public:
    Vector ()          { contents_ = new int[0]; howMany_ = 0; }
    Vector (const Vector& other)          { copy(other);       }
    Vector (Vector&& other) noexcept;                 // move ctor
    ~Vector() noexcept { if (contents_) delete [] contents_;   }

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&&      other) noexcept; // move =

    std::size_t  size   () const { return howMany_;     }
    int*         data   () const { return contents_;    }

    int  operator[](int index) const { return contents_[index]; }
    int& operator[](int index)       { return contents_[index]; }

    int  at(int index) const;
    int& at(int index);

    void push_back(int newElement);   // add newElement at the back
    void pop_back();                  // left as an exercise
    int  back() const;                // left as an exercise

    void print(std::ostream&) const;  // left as an exercise
private:
    int*        contents_;
    std::size_t howMany_;

    void copy(const Vector& other);
};

bool operator==(const Vector& a, const Vector& b) noexcept;

inline
std::ostream& operator<<(std::ostream& out, const Vector& foo)
{
    foo.print(out); return out;
}
#endif //VECTOR_H
