//Card class tester
//Will Briggs
//Spring 2012

#include <iostream>
#include <cstdlib> //for srand
#include <ctime>   //for time
#include <cassert> //for assert
#include "deck.h"
#include "waste.h"

using namespace std;

int main ()
{
	srand (static_cast<int>(time (nullptr)));

	//Test isFull
	CardGroup myCardGroup;
	assert (! myCardGroup.full()); assert (myCardGroup.empty());
	for (int i = 0; i < CardGroup::MAX_SIZE; ++i)
		myCardGroup.addCardLegally (Card ());
	assert (myCardGroup.full()); assert (! myCardGroup.empty());

	//Test shuffle, printing
	Deck deck;

	cout << "An unshuffled deck:  " << deck << endl;
	deck.shuffle();
	cout << "A shuffled deck:     " << deck << endl;

	//Test dealing
	Waste waste;
	cout << "Now I'll take the first 3 cards and put them into the waste.\n";
	for (int i = 0; i < 3; ++i)
	{
		Card c = deck.deal (); cout << c << endl; waste.addCardLegally (c);
	}
	cout << "This is the waste:     " << waste << endl;
	cout << "And this is the deck:  " << deck  << endl;

	return 0;
}