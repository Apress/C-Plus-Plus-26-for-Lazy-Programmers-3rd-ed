//PowerupSet class: a Shape that is a vector of Powerup
//	-- from _C++26 for Lazy Programmers_

#ifndef POWERUP_SET_H
#define POWERUP_SET_H

#include <vector>
#include "powerup.h"

class PowerupSet //inheritance?
{
 public:
    PowerupSet (const char* text = "") : Shape (0, 0, text) {}
    PowerupSet (const PowerupSet&);

    void animate();
};

#endif
