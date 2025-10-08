// class List:  a linked list class
//      from _C++26 for Lazy Programmers_

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <concepts>
#include <cstddef> //for std::size_t

template<typename T>
concept ListElement = std::assignable_from<T, T>&& std::totally_ordered<T>;

template<typename T>
concept Printable = requires(std::ostream & out, const T & t)
{
    out << t;
};

template<typename T>
class List
{
public:
    class Underflow  {};                    // exception

    List () = default; 
    List (const List& other) : List ()      { copy(other);        }
    ~List() noexcept                        { eraseAllElements(); }

    //move functions
    List           (List&& other) noexcept;
    List& operator=(List&& other) noexcept;

    List& operator=(const List& other) 
    {
        if (this == &other) return *this; //prevent self-assignment
        eraseAllElements(); createEmptyList(); copy(other); 
        return *this;
    }

    //Another way to support operator==
    bool isEqual(const List<T>& other) const;

    std::size_t size () const { return size_; }
    bool   empty() const { return size() == 0; }

    void push_front(const T& newElement); // add newElement at front
    void pop_front ();                    // take one off the front
    const T& front () const
    {
        if (empty()) throw Underflow(); else return start_->data_;
    }

    void print(std::ostream&) const requires Printable<T>;
 private:
    struct Entry
    {
        T      data_ = T();               //default for whatever T is
                                          // even if it's a basic type
        Entry* next_ = nullptr;
    };

    std::size_t size_ = 0;
    Entry* start_= nullptr;               // points to first element

    void copy(const List& other);         // copies other's entries
                                          //   into this List

    void eraseAllElements();              // empties the list
    void createEmptyList ()
    {
        start_ = nullptr; size_ = 0;      // the list is...nothing
    }
};

//Comparisons

template<typename T>
bool operator==(const List<T>& a, const List<T>& b) { return a.isEqual(b); }

template<typename T>
bool List<T>::isEqual(const List<T>& other) const
{
    if (size_ != other.size_) return false;// diff sizes => not equal

    Entry* aPtr =       start_;
    Entry* bPtr = other.start_;

    for (;
        aPtr && bPtr;
        aPtr = aPtr->next_, bPtr = bPtr->next_)
        if (aPtr->data_ != bPtr->data_) return false;

    if (aPtr || bPtr) return false;   // one list is longer

    return true;                      // otherwise, equal  
}

// I/O

template<typename T>
inline
std::ostream& operator<<(std::ostream& out, const List <T>& foo)
{
    foo.print(out); return out;
}

template<typename T>
void List<T>::print(std::ostream& out) const requires Printable<T>
{
    out << '[';

    for (Entry* loc = start_; loc != nullptr; loc = loc->next_)
        out << loc->data_ << ' ';

    out << ']';
}

// Move functions

template<typename T>
List<T>::List(List&& other) noexcept
{
    start_ = other.start_; size_ = other.size_;
    other.createEmptyList();
}

template<typename T>
List<T>& List<T>::operator=(List&& other) noexcept
{
    eraseAllElements();                      // Clear out old list
    start_ = other.start_;                   // Take in new list
    size_ = other.size_;

    other.createEmptyList();                 // Make other list empty

    return *this;
}

// Adding and deleting

template<typename T>
void List<T>::eraseAllElements() { while (!empty()) pop_front();   }

template<typename T>
void List<T>::push_front(const T& newElement)
{
    Entry* newEntry = new Entry; // create an Entry
    newEntry->data_ = newElement;// put newElement in its data_ field
    newEntry->next_ = start_;    // put old version of start_
                                 //  in its next_ field

    start_          = newEntry;  // put address of new Entry 
                                 //  into start_
    ++size_;
}

template<typename T>
void List<T>::pop_front()
{
    if (empty()) throw Underflow();

    Entry* temp = start_;   // store location of thing to delete
    start_ = start_->next_; // let start_ = next thing after start_

    delete temp;            // delete the item
    --size_;
}

template<typename T>
void List<T>::copy(const List& other)
{
    size_ = 0;                    // start with 0 entries

    Entry* lastEntry = nullptr;   // last thing we added to this list,
                                  //   as we go thru other list
    Entry* otherP = other.start_; // where are we in the other list?

    // while not done with other list... 
    //    copy its next item into this list
    while (otherP)
    {
        // make a new entry with current element from other;
        //  put it at end of our list (so far)
        Entry* newEntry = new Entry;
        newEntry->data_ = otherP->data_;
        newEntry->next_ = nullptr;
        
        // if list is empty, make it start_ with this new entry
        // if not, make its previous Entry recognize new entry 
        //   as what comes next
        if (empty()) start_           = newEntry;
        else         lastEntry->next_ = newEntry;

        lastEntry = newEntry;  // keep pointer for lastEntry updated
        otherP = otherP->next_;// go on to next item in other list

        ++size_;
    }
}
#endif //LIST_H