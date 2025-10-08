// Waste class
//      -- from _C++26 for Lazy Programmers_

#ifndef WASTE_H
#define WASTE_H

#include "card_group.h"

class Waste: private CardGroup
{
public:
    using CardGroup::CardGroup;

    using CardGroup::operator=; 

    using CardGroup::empty, CardGroup::full;
    using CardGroup::top;
    using CardGroup::removeTop;
    using CardGroup::addCardLegally;
    using CardGroup::print;
    bool isEqual(const Waste& other) const
    {
        return static_cast<CardGroup>(*this) == static_cast<CardGroup>(other);
    }
};

inline
std::ostream& operator<<(std::ostream& out, const Waste& foo)
{
    foo.print (out); return out; 
}

inline
bool operator==(const Waste& a, const Waste& b) noexcept
{ 
    return a.isEqual(b);
}
#endif // WASTE_H
