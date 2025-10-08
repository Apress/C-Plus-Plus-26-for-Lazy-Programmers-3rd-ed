//Program to animate some powerups on mouse clicks
//		-- from _C++26 for Lazy Programmers_

#include <vector>
#include <span>
#include "flashy_powerup.h"
#include "mega_powerup.h"
#include "wormhole.h"

using namespace std;

Powerup* selectPowerup  (vector<Powerup>& objects); 
void     printScore     (int score);
void     move           (span<Powerup> powerups); 

int main (int argc, char** argv) 
{
    //This should not compile when the exercise is done:
    //Powerup p;

    SSDL_SetWindowSize (500, 300);	//make window smaller
    SSDL_SetWindowTitle("Click on a powerup for points!");

    //Create a set of powerups
    vector<Powerup> powerups; 
    enum {RADIUS = 30};

    //...consisting of three powerups
    //These don't compile as is because we're trying to push a FlashyPowerup, etc., as a Powerup
    //   and there isn't room! What's the fix?
    //powerups.push_back (FlashyPowerup (Point2D( 75, 150), RADIUS));
    //powerups.push_back (Wormhole      (Point2D(250, 150), RADIUS));
    //powerups.push_back (MegaPowerup   (Point2D(425, 150), RADIUS));

    //color those powerups
    //powerups[0].setColor (BLUE);
    //powerups[1].setColor (SSDL_CreateColor (255, 255, 0)); //yellow
    //powerups[2].setColor (GREEN);

    //do a game loop
    while (SSDL_IsNextFrame ())
    {
        SSDL_DefaultEventHandler ();

        SSDL_RenderClear ();	//clear the screen

        static int score = 0;
        printScore (score);

        //see if the user clicked on a powerup
        Powerup* shapeSelected = selectPowerup (powerups);
        if (shapeSelected)	//if so, and it's not already animated
            if (! shapeSelected->isAnimated ())
            {			//animated it, and update score
                shapeSelected->startAnimation ();
                score += shapeSelected->value ();
            }

        //Drawing...but we should animate
        for (Powerup& p : powerups)
            p.draw ();//<-- Change this to animate
	
        ::move (powerups);        //let the powerups bounce around the screen
                                  //Why the ::? I'm using namespace std, and
                                  //  std::move exists, so the compiler gets confused.
                                  //  ::move says "use the one in the global namespace"
                                  //  (the one defined in this file)
                                  //Looks like using namespace std is not without risks
    }

    return 0;
}

void move (std::span<Powerup> powerups)
//using std::span works with vectors too! This function demonstrates its use
//If you replaced the signature with this one, the code would stay the same:
//void move (vector<Powerup>& powerups)
{
    enum {SPEED=5};

    static int dx = 1, dy=1;				//Initially, we're headed SE

    for (Powerup& p : powerups)	            //move 'em around the screen
        p.moveBy(dx*SPEED, dy*SPEED);

    bool isBounceX = false, isBounceY = false;          //Do we need to reverse direction?

    for (const Powerup& p: powerups)        //If any powerup...
	{
	    //goes off screen to left
	    if (p.location().x_ < p.radius()) isBounceX = true;
	    //...or right
	    if (p.location().x_ > SSDL_GetWindowWidth() - p.radius())
		isBounceX = true;
	    //...or top
	    if (p.location().y_ < p.radius()) isBounceY = true;
	    //...or bottom,
	    if (p.location().y_ > SSDL_GetWindowHeight()- p.radius())
		isBounceY = true;
	    //then bounce back in that direction
	}

    if (isBounceX) dx *= -1; //if we need to bounce, change direction
    if (isBounceY) dy *= -1;
}

void printScore (int score)
{
    SSDL_SetRenderDrawColor (WHITE);
    enum {SCORE_MSG_WIDTH = 80, SCORE_NUM_WIDTH=20};

    SSDL_SetCursor(SSDL_GetWindowWidth() - SCORE_MSG_WIDTH, 0); 
    sout << "Score:  " << score;
}

//from Chapter 12
bool getMouseClickWithDelay (int waitingPeriod)
{
    static int timeSinceLastMouseClick = 0;
    bool isClicked = false;	          //no click yet...

    //click, so reset waiting time
    //and return that a click happened
    if (timeSinceLastMouseClick > waitingPeriod && SSDL_GetMouseClick ())
    {
        timeSinceLastMouseClick = 0; isClicked = true;
    }
    else                                  //don't report click; just update time
        ++timeSinceLastMouseClick;

    return isClicked;
}

Powerup* selectPowerup (std::vector<Powerup>& powerups)
  //No const here because we'll be returning something that must be alterable
{
    int x = SSDL_GetMouseX(), y = SSDL_GetMouseY(); //get position
	
    enum {MOUSE_CLICK_DELAY = 10};		    //no mouse click? never mind
    if (! getMouseClickWithDelay (MOUSE_CLICK_DELAY)) return nullptr;

    //if mouse clicked, find the object clicked on (if any) and return a pointer to it
    for (Powerup& p : powerups) 
        if (p.contains(x, y))
            return &p;

    return nullptr; //but if nothing was clicked on, return nothing
}

