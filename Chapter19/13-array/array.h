// An Array class, like the one in #include <array>
//      -- from _C++26 for Lazy Programmers_

#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <initializer_list>

template<typename T, std::size_t N>
class Array
{
public:
    Array() {}
    Array(const Array& a)
    {
        for (int i = 0; i < N; ++i)
            contents_[i] = a[i];
    }

    std::size_t size() const { return N; }
    T  operator[](int index) const { return contents_[index]; }
    T& operator[](int index)       { return contents_[index]; }
    T  at(std::size_t index) const 
    { 
        if (index >= N) 
            throw std::out_of_range("Out of range in Array::at");
        return contents_[index];
    }
    T& at(std::size_t index) 
    {
        if (index >= N) 
            throw std::out_of_range("Out of range in Array::at");
        return contents_[index];
    }
private:
    T contents_[N];
};
#endif

