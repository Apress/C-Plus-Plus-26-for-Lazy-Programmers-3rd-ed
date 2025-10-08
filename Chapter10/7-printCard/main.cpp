// Program to print some suits
//       -- from _C++26 for Lazy Programmers_

#include "SSDL.h"
#include <utility> //for std::to_underlying

//  Suit enumeration type
enum class Suit { CLUBS, DIAMONDS, HEARTS, SPADES };

void print(Suit);

int main (int argc, char** argv)
{
    constexpr int H = std::to_underlying(Suit::HEARTS);

    sout << "Q"; print(Suit::SPADES); sout << " is also known as \"Calamity Jane.\"\n";

    sout << "Hit any key to end.\n";
    SSDL_WaitKey ();

    return 0;
} 

void print(Suit suit)
{
    switch (suit)
    {
    case Suit::CLUBS:    sout << 'C'; break;
    case Suit::DIAMONDS: sout << 'D'; break;
    case Suit::HEARTS:   sout << 'H'; break;
    case Suit::SPADES:   sout << 'S'; break;
    default:             sout << '?'; break;
    }
}