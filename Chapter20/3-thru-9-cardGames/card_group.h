// CardGroup class (for playing cards)
//        -- from _C++26 for Lazy Programmers_

#ifndef CARD_GROUP_H
#define CARD_GROUP_H

#include <vector>
#include "card.h"

class IllegalMove {};  // Exception class

class CardGroup: public std::vector<Card>
{
public:        
    static constexpr int MAX_SIZE = 4 * 52; 
                                 //4 decks should be enough for any game

    CardGroup()                       = default;
    CardGroup(const CardGroup& other) = default;

    CardGroup& operator=(const CardGroup& other)
    {
        std::vector<Card>::operator=(other); return *this; 
        //Use vector<Card>'s version of =
    }

    Card remove   (int index);
    Card top() const { return vector<Card>::back();                        }
    Card removeTop() { Card c = top(); vector<Card>::pop_back(); return c; }

    bool   full() const { return size() >= MAX_SIZE; } 
    //vectors don't get full -- but I decided CardGroups can

    bool   isLegalToAdd      (const Card& c) const { return !full (); }
    bool   isLegalToRemoveTop()              const { return !empty(); }

    // addCard does NOT check that it's legal to add a card.  
    // We need it to create CardGroups during the deal. 
    void addCard(const Card& c) { push_back(c); }

    // makes sure the addition of the card is legal, then adds it
    void addCardLegally(const Card& c) 
    {
        if (isLegalToAdd(c)) addCard(c);   else throw IllegalMove();
    }
    Card removeTopLegally() 
    { 
        if (isLegalToRemoveTop()) return removeTop(); else throw IllegalMove(); 
    }

    void print(std::ostream&) const;

private:
    //Nothing here -- it's all inherited from std::vector<Card>
};

inline
std::ostream& operator<<(std::ostream& out, const CardGroup& foo)
{ 
    foo.print(out); return out; 
}

inline
bool operator==(const CardGroup& a, const CardGroup& b) noexcept
{
    //To use vector<Card>'s ==, cast a and b to be vector<Card>
    return static_cast<std::vector<Card>>(a) == 
                              static_cast<std::vector<Card>>(b);
}

using Hand = CardGroup;
#endif // CARD_GROUP_H
