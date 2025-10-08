// Cell class
//  -- from _C++26 for Lazy Programmers_

#ifndef CELL_H
#define CELL_H

#include "waste.h"

class Cell: public Waste 
{
public:
    using Waste::Waste;
    Cell& operator=(const Cell& other) { Waste::operator=(other); return *this; }

    bool full() const { return !empty(); } //Can only have one card in a Cell!

    // public inheritance, so all public members of Waste are here...
    
    void addCardLegally(const Card& card) 
    { 
        if (full()) throw IllegalMove(); // Cell must be empty
        else Waste::addCardLegally(card); 
    }
};

inline
bool operator==(const Cell& a, const Cell& b) noexcept 
{
    return static_cast<Waste>(a) == static_cast<Waste>(b); 
}
#endif //CELL_H
