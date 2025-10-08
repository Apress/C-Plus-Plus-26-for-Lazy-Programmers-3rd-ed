//FlashyPowerup class: a Powerup for an arcade game
//	 -- from _C++26 for Lazy Programmers_

#include "flashy_powerup.h"

void FlashyPowerup::animate()
{
    if (!isAnimated ()) { draw (); return; }

    static int lastColor = 6;
    int whichColor = framesLeftInAnimation_ / FRAMES_PER_COLOR;
    if (whichColor != lastColor) lastColor = whichColor;
    
    SSDL_SetRenderDrawColor(colors_[whichColor]);
    drawAux();
    --framesLeftInAnimation_;
    if (framesLeftInAnimation_ == 0)
        lastColor = 6;
}
