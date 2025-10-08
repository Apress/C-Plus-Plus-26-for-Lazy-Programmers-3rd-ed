//MegaPowerup class: a Powerup for an arcade game
//	 -- from _C++26 for Lazy Programmers_

#ifndef MEGA_POWERUP_H
#define MEGA_POWERUP_H

#include "powerup.h"

class MegaPowerup: public Powerup
{
public:
	enum {FRAMES_PER_COLOR = 10, MAX_COLORS=6};
	//should multiply to get Powerup::MAX_FRAMES_IN_ANIMATION = 60

	MegaPowerup () : Powerup () 
	{
		colors_[0] = colors_[2] = colors_[4] = BLACK;
		colors_[1] = colors_[3] = colors_[5] = WHITE;
	}
	MegaPowerup (const MegaPowerup& c) = delete;
	MegaPowerup (const Point2D& p, int theRadius, const char* txt="") : 
		Powerup (p, theRadius, txt) 
	{
		colors_[0] = colors_[2] = colors_[4] = BLACK;
		colors_[1] = colors_[3] = colors_[5] = WHITE;
	} 

	MegaPowerup& operator= (const MegaPowerup& c) = delete;
	void animate();
	int value () const { return 25; } //This powerup is worth 25 points!
private:
	SSDL_Color colors_[MAX_COLORS]; 
};
#endif