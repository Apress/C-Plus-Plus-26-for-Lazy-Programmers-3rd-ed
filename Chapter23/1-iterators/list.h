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
    class Underflow  {};                    // exceptions
    class BadPointer {};

    List()                                  { createEmptyList(); }
    List (const List& other) : List ()      { copy(other);        }
    ~List()                                 { eraseAllElements(); delete start_; }
                                            // the elements, then the caboose

    //move functions
    List           (List&& other) noexcept;
    List& operator=(List&& other) noexcept;

    List& operator=(const List& other) 
    {
        if (this == &other) return *this; //prevent self-assignment
        eraseAllElements(); createEmptyList(); copy(other); 
        return *this;
    }

    //Another way to support operator==. Doesn't use friend
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

    std::size_t size_;
    Entry* start_;                        // points to first element
    Entry* end_  ;                        // points just past last element
    void copy(const List& other);         // copies other's entries
                                          //   into this List

    void eraseAllElements();              // empties the list
    void createEmptyList ()
    {
        start_ = end_ = new Entry;        // caboose is both first and last car
        size_ = 0;                        //   for now
    }

public:
    class iterator                         // An iterator for List
    {
    public:
        iterator(const iterator& other) : where_(other.where_) {}
        iterator(Entry* where = nullptr) : where_(where) {}

        const iterator& operator= (const iterator& other)
        {
            where_ = other.where_;
        }

        //Another way to do ==. The community mostly dislikes == as a member,
        //  but this is simple to write & avoids template-related complexities
        bool operator== (List<T>::iterator other) { return where_ == other.where_; }

        const iterator& operator++()    // pre-increment, as in ++i
        {
            if (where_->next_ == nullptr) throw BadPointer();
            else where_ = where_->next_;
            return *this;
        }
        iterator operator++ (int)       // post-increment, as in i++
        {
            iterator result = *this; ++(*this); return result;
        }

        T& operator* ()
        {
            if (where_->next_ == nullptr) throw BadPointer();
            return where_->data_;
        }

        T* operator->()  // This really is how you do it.  It works!
        {
            if (where_->next_ == nullptr) throw BadPointer();
            return &(where_->data_);
        }
    private:
        Entry* where_;
    };

    iterator       begin() { return iterator(start_); }
    iterator       end() { return iterator(end_); }
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
        aPtr != end_ && bPtr != other.end_;
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
List<T>::List(List&& other) noexcept           // move ctor
{
    start_ = other.start_; end_ = other.end_;  size_ = other.size_;
    other.createEmptyList();
}

template<typename T>
List<T>& List<T>::operator= (List&& other) noexcept  // move =
{
    eraseAllElements(); delete start_; // erase all, incl. caboose
    start_ = other.start_; end_ = other.end_;  size_ = other.size_;
    other.createEmptyList();
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

    lastEntry->next_ = end_ = new Entry;  // attach caboose
}
#endif //LIST_H