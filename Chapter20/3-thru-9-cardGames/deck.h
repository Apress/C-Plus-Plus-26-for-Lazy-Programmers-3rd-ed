// Deck class
//    -- from _C++26 for Lazy Programmers_

#ifndef DECK_H
#define DECK_H

#include "card_group.h"

class Deck: private CardGroup
{
public:
    static constexpr int MAX_SIZE = 52;

    Deck();
    Deck(const Deck& other) = delete;
    Deck& operator=(const Deck& other) = delete;

    Card deal   ()              {  return removeTop();         }

    void shuffle();

    using CardGroup::print;
    using CardGroup::size;
    using CardGroup::empty;
    using CardGroup::full;
    using CardGroup::addCard;
};

inline
std::ostream& operator<<(std::ostream& out, const Deck& foo)
{
    foo.print(out); return out;
}
#endif //DECK_H
