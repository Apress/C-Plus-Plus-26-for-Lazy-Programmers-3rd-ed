//Wormhole class: a Powerup for an arcade game
//	 -- from _C++26 for Lazy Programmers_

#ifndef WORMHOLE_H
#define WORMHOLE_H

#include "powerup.h"

class Wormhole: public Powerup
{
public:
	Wormhole () : Powerup () {}
	Wormhole (const Wormhole& c) = delete;
	Wormhole (const Point2D& p, int theRadius, const char* txt="") :
		Powerup (p, theRadius, txt) 
	{
	} 

	Wormhole& operator= (const Wormhole& c) = delete;

	void animate();

	int value () const { return 10; } //This powerup is worth 10 points!
};
#endif //WORMHOLE_H