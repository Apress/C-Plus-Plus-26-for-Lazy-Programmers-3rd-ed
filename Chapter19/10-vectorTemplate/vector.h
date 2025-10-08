// Vector class:  a variable-length array
//      -- from _C++ for Lazy Programmers_

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template<typename T>
class Vector
{
public:
    Vector() { contents_ = new T [0]; howMany_ = 0; }
    Vector(const Vector& other) { copy(other); }
    Vector(Vector&& other) noexcept;  // move ctor
    ~Vector() noexcept { if (contents_) delete[] contents_; }

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept; // move =

    std::size_t   size() const { return howMany_;     }
    const T*      data() const { return contents_;    }

    const T& operator[] (int index) const
    {
        return contents_[index];
    }
          T& operator[] (int index)
    {
        return contents_[index];
    }

    const T& at(int index) const;
          T& at(int index);

    void push_back  (const T& newElement); // add newElement at back
    void pop_back();                       // left as an exercise
    const T& back() const;                 // left as an exercise

    void print(std::ostream&) const;  // left as an exercise
private:
    T*          contents_;
    std::size_t howMany_;

    void copy(const Vector& other);
};

template<typename T>
inline
std::ostream& operator<<(std::ostream& out, const Vector<T>& foo)
{
    foo.print(out); return out;
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept              // move ctor
{
        contents_ = other.contents_;howMany_ = other.howMany_; 
        other.contents_ = nullptr;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept // move =
{
        if (contents_) delete[] contents_;
        contents_ = other.contents_; howMany_ = other.howMany_;
        other.contents_ = nullptr;
        return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (this == &other) return *this; // don't assign to self -- you'll trash contents
    if (contents_) delete[] contents_; copy(other);
    return *this;
}

template<typename T>
bool operator==(const Vector<T>& a, const Vector<T>& b) noexcept
{
    if (a.size() != b.size()) return false; // diff sizes => not equal

    bool noDifferences = true;

    // quit if you find a difference or run out of elements
    for (unsigned int i = 0; i < a.size() && noDifferences; ++i)
        if (a[i] != b[i]) noDifferences = false;

    return noDifferences;
}

template<typename T>
const T& Vector<T>::at(int index) const
{
    if (index >= size()) throw std::out_of_range("Out of range in Vector::operator[]");
    else return contents_[index];
}

template<typename T>
T&       Vector<T>::at(int index)
{
    if (index >= size()) throw std::out_of_range("Out of range in Vector::operator[]");
    else return contents_[index];
}


// add newElement at the back
template<typename T>
void Vector<T>::push_back(const T& newElement)
{
    T* newContents = new T[howMany_ + 1];     // make room for 1 more... 

    for (unsigned int i = 0; i < size(); ++i) // copy existing elements
                                              //  into new array... 
        newContents[i] = contents_[i];

    newContents[howMany_] = newElement;       // add the new element... 

    ++howMany_;                               // remember we have one more... 

    delete[] contents_;                       // throw out old contents_
    contents_ = newContents;                  //  and keep new version
}

template<typename T>
void Vector<T>::copy(const Vector& other)
{
    // set howMany to other's size; allocate that much memory
    contents_ = new T [howMany_ = other.size()];
    
    // then copy the elements over
    for (unsigned int i = 0; i < size(); ++i)
        contents_[i] = other[i];
}
#endif //VECTOR_H
