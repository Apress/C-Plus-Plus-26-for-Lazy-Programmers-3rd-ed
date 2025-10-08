// Vector class:  a variable-length array of ints
//      -- from _C++26 for Lazy Programmers_

#include <stdexcept>
#include "vector.h"

Vector::Vector(Vector&& other)            noexcept // move ctor
{
    contents_ = other.contents_; howMany_ = other.howMany_;
    other.contents_ = nullptr; 
}

Vector& Vector::operator=(Vector&& other) noexcept // move =
{
    if (contents_) delete[] contents_;
    contents_ = other.contents_; howMany_ = other.howMany_;
    other.contents_ = nullptr;
    return *this;
}

Vector& Vector::operator=(const Vector& other)
{
    if (this == &other) return *this; 
                       // don't assign to self -- you'll trash contents
    if (contents_) delete[] contents_; copy(other);
    return *this;
}

bool operator==(const Vector& a, const Vector& b) noexcept
{
    if (a.size() != b.size()) return false; // diff sizes => not equal

    bool noDifferences = true;

    // quit if you find a difference or run out of elements
    for (unsigned int i = 0; i < a.size() && noDifferences; ++i)
        if (a[i] != b[i]) noDifferences = false;

    return noDifferences;
}

int  Vector::at(int index) const
{
    if (index >= size()) 
        throw std::out_of_range("Out of range in Vector::operator[]");
    else return contents_[index];
}

int& Vector::at(int index)
{
    if (index >= size()) 
        throw std::out_of_range("Out of range in Vector::operator[]");
    else return contents_[index];
}

// add newElement at the back
void Vector::push_back(int newElement)
{
    int* newContents = new int[howMany_ + 1]; //make room for 1 more... 

    for (unsigned int i = 0; i < size(); ++i)
                                    //copy old elements to new array... 
        newContents[i] = contents_[i];

    newContents[howMany_] = newElement;       // add the new element... 

    ++howMany_;                               // remember we have 1 more... 

    delete[] contents_;                       // throw out old contents_
    contents_ = newContents;                  //  and keep new version
}

// Sort of like String::copy from Chapter 17, but without strcpy
void Vector::copy(const Vector& other)
{
    // set howMany to other's size; allocate that much memory
    contents_ = new int[howMany_ = other.size()];

    // then copy the elements over
    for (unsigned int i = 0; i < size(); ++i)
        contents_[i] = other[i];
}