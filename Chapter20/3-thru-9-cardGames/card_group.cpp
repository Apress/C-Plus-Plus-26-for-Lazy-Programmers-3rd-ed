// CardGroup class (playing cards)
//       -- from _C++26 for Lazy Programmers_

#include "card_group.h"

using namespace std;

Card CardGroup::remove(int index)
{
    Card result = at(index); //will throw an exception of index is too big

    // This loop copies each Card *after* index, back 1 slot.
    // So the array might look like so, if you try to remove card#1:
    // K♦ 2♣ 8♥ J♦
    // K♦ 8♥ 8♥ J♦
    // K♦ 8♥ J♦ J♦
    // ...then we reduce howMany_ and have
    // K♦ 8♥ J♦  
    for (int i = index; i < size()-1; ++i)  
        (*this)[i] = (*this)[i+1];

    pop_back();

    return result;
}

void CardGroup::print(ostream& out) const
{
    if (empty()) 
        out << "---"; 
    else 
        for (int i = 0; i < size(); ++i) 
            out << (*this)[i] << ' ';
}

