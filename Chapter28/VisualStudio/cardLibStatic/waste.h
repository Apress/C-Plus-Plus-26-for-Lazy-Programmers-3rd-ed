//  Waste class
//         -- from _C++26 for Lazy Programmers_

#ifndef WASTE_H
#define WASTE_H

#include "card_group.h"

namespace Cards
{
    class Waste: private CardGroup
    {
    public:
        Waste () {}
        Waste (const Waste&     other) : CardGroup (other) {}   
        Waste (const CardGroup& other) : CardGroup (other) {}

        Waste& operator=    (const Waste& other) = delete;           

        bool operator==     (const Waste& other) const 
        {
            return CardGroup::operator== (other); 
        }
        bool empty          () const { return CardGroup::empty ();  }
        bool full         () const { return CardGroup::full  ();  }
        unsigned int size   () const { return CardGroup::size ();     } 
        Card top            () const { return CardGroup::top();       } 
        Card removeTop      ()       { return CardGroup::removeTop(); } 
        void addCardLegally (const Card& foo) 
        {  
            CardGroup::addCardLegally (foo);    
        } 
        void print (std::ostream& out) const{ CardGroup::print (out); }
    };

    inline
    std::ostream& operator<< (std::ostream& out, const Waste& foo)
    {
        foo.print (out); return out; 
    }
} //namespace Cards
#endif //WASTE_H
