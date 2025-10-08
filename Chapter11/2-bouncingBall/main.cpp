// Program to make a circle go back & forth across the screen
//         -- from _C++26 for Lazy Programmers_

#include "SSDL.h"

constexpr int RADIUS = 20;     // Ball radius & speed
constexpr int SPEED  =  5;     //  ...move 5 pixels for every frame

enum class Direction { LEFT, RIGHT };

struct Point2D                                  
{
    int x_=0, y_=0;
};

struct Ball             // A ball is an X, Y location, 
{                       // and a direction, left or right
    Point2D   location_;
    Direction direction_;
};

int main (int argc, char** argv)
{
    SSDL_SetWindowTitle ("Back-and-forth ball example.  "
                         "Hit Esc to exit.");

    // initialize ball position; size; direction of movement
    Ball ball;
    ball.location_  = { SSDL_GetWindowWidth () / 2, 
                        SSDL_GetWindowHeight() / 2 };
    ball.direction_ = Direction::RIGHT;
        
    constexpr int FRAMES_PER_SECOND = 70;
    SSDL_SetFramesPerSecond (FRAMES_PER_SECOND);
    
    while (SSDL_IsNextFrame())
    {
        SSDL_DefaultEventHandler();
        
        // *** DISPLAY THINGS ***
        SSDL_RenderClear();    // first, erase previous frame

        // then draw the ball
        SSDL_RenderDrawCircle (ball.location_.x_, ball.location_.y_, 
                               RADIUS);
                
        // *** UPDATE THINGS ***
        // update ball's x position based on speed
        //  and current direction
        if (ball.direction_ == Direction::LEFT)
            ball.location_.x_ -= SPEED;
        else
            ball.location_.x_ += SPEED;
        
        // if ball moves off screen, reverse its direction
        if      (ball.location_.x_ >= SSDL_GetWindowWidth()) 
            ball.direction_ = Direction::LEFT;
        else if (ball.location_.x_ <                      0) 
            ball.direction_ = Direction::RIGHT;
    }
    
    return 0;
}

