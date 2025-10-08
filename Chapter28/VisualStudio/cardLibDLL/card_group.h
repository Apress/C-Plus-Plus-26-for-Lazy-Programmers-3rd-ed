// CardGroup class (for playing cards)
//        -- from _C++26 for Lazy Programmers_

#ifndef CARDGROUP_H
#define CARDGROUP_H

#include "card.h"

namespace Cards
{
    class OutOfRange  {};         // Exception classes
    class IllegalMove {};   

    class CardGroup
    {
    public:
        static constexpr int MAX_SIZE = 208;    // if anybody wants a game
                                                //   w/ more than 4 decks,
                                                //  change this.

        CardGroup ()                      { howMany_ = 0;              }
        CardGroup (const CardGroup& other){ copy(other);               }
        CardGroup (const Card& other)     
        { 
            howMany_ = 0; addCard (other); 
        }

        CardGroup& operator= (const CardGroup& other) 
        { 
            copy(other); return *this; 
        }
        DECLSPEC bool operator== (const CardGroup& other) const;

        DECLSPEC Card& operator[] (unsigned int index);
        DECLSPEC Card  operator[] (unsigned int index) const;

        DECLSPEC Card remove         (unsigned int index);
        Card top            () const { return (*this)[size()-1];  }
        Card removeTop      ()       { return remove (size()-1);  }

        unsigned int size   () const { return howMany_;           }
        bool         empty() const { return size() == 0;        }
        bool         full () const { return size() >= MAX_SIZE; }

        // addCard does NOT check that it's legal to add a card.
        // We need this for creating CardGroups during the deal. 
        DECLSPEC void addCard (const Card&);

        // makes sure the addition of the card is legal, then adds it
        DECLSPEC void addCardLegally(const Card& other);
    
        DECLSPEC void print (std::ostream&) const;

    private:
        unsigned int howMany_;
        Card contents_ [MAX_SIZE];
        DECLSPEC void copy (const CardGroup&);  // copy cards over; used by =, copy ctor
    };

    inline
    std::ostream& operator<< (std::ostream& out, const CardGroup& foo)
    { 
        foo.print(out); return out; 
    }

    using Hand = CardGroup;
} //namespace Cards
#endif //CARDGROUP_H
