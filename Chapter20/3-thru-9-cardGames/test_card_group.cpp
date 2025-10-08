// Tester program for CardGroup
//    -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <cassert> // for assert
#include "card_group.h"

using namespace std;

int main()
{
    using enum Suit;
    using enum Rank;
    using enum Color;

    // CardGroup
    // Test ifull, empty
    CardGroup group1;
    assert( group1.empty());
    assert(!group1.full ());
    for (int i = 0; i < CardGroup::MAX_SIZE; ++i)
        group1.addCardLegally(Card());
    try
    {
        group1.addCardLegally(Card());
        cout << "addCardLegally's not finding an error--but should.\n";
    }
    catch (const IllegalMove&) {}
    assert(! group1.empty()); assert( group1.full());

    // Test copy ctor, =
    CardGroup CG1; CG1.addCard(Card(ACE, SPADES));       // Test ctors, ==, and !=
    assert(CG1.top() == Card(ACE,SPADES)); 
    Hand CG2 = CG1;                            // Hand == CardGroup
    assert(CG1 == CG2); assert(!(CG1 != CG2));

    CG1.addCardLegally(Card(ACE, DIAMONDS)); // Test size, =, ==, and !=
    assert(CG1.top() == Card(ACE,DIAMONDS));  // addCardLegally
    assert(CG1.size() == 2);   
    assert(!(CG1 == CG2));
    assert(CG1 != CG2);
    CG2 = CG1;
    assert(CG1 == CG2);
    assert(!(CG1 != CG2));
     
    assert(CG1[1] == Card(ACE, DIAMONDS));// Test []
    cout << "Two aces: " << CG1 << '\n';

    CG2.remove(0);                     // Test remove
    assert(CG2.top() == Card(ACE, DIAMONDS));

    CG1.removeTop();                   // Test removeTop
    assert(CG1.top() == Card(ACE, SPADES));
    CG1.removeTop();
    cout << "No cards: " << CG1 << '\n';

    cout << "If no errors were reported, testGroups "
        "must have succeeded!\n";

    return 0;
}
