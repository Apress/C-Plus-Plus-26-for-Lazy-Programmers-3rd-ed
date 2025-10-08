// Test CardGroup, Waste, Cell, Deck
//    -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <cassert> // for assert
#include "waste.h"
#include "cell.h"

using namespace std;

int main ()
{
    using enum Suit;
    using enum Rank;
    using enum Color;

    // Test Waste
    Waste waste; 
    Waste w1 (waste);
    w1 = waste;

    // Test Cell, thereby testing more of Waste
    Cell cell;
    cout << "No card:             " << cell << '\n';
    cell.addCardLegally (Card (Rank(2), CLUBS));
    try
    {
        cell.addCardLegally (Card());
        cout << "Error: added a second card to a cell!\n";
    }
    catch (const IllegalMove&) {}

    Cell c1 = cell;      assert(c1.top() == Card(Rank(2), CLUBS));
    //c1 = waste; <-- won't work; Cell::operator= expects another Cell 
    assert(  c1.full ());
    assert(! c1.Waste::full ());
	cout << "This is 2 of clubs: " << cell << '\n';;
    cell.removeTop();

    cout << "If no errors were reported, testWasteAndCell "
        "must have succeeded!\n";

    return 0;
}
