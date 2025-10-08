// Program to make circles move back & forth across the screen
//        -- from _C++26 for Lazy Programmers_

#include <span>
#include "SSDL.h"

constexpr int RADIUS = 20; //Ball radius & speed
constexpr int SPEED = 5; // ...move 5 pixels for every frame

enum class Direction { LEFT, RIGHT };

struct Point2D
{
    int x_ = 0, y_ = 0;
};

struct Ball             // A ball is an X, Y location, 
{                       // and a direction, left or right
    Point2D   location_;
    Direction direction_;
};

// Ball functions
void initializeBalls(std::span<Ball> balls);
void drawBalls      (std::span<const Ball> balls);
void moveBalls      (std::span<Ball> balls);
void bounceBalls    (std::span<Ball> balls);

int main(int argc, char** argv)
{
    SSDL_SetWindowTitle("Back-and-forth balls example.  "
        "Hit Esc to exit.");

    // initialize balls' position, size, rate and direction
    constexpr size_t MAX_BALLS = 3;
    Ball balls[MAX_BALLS];
    initializeBalls(balls);

    constexpr int FRAMES_PER_SECOND = 70;
    SSDL_SetFramesPerSecond(FRAMES_PER_SECOND);

    while (SSDL_IsNextFrame())
    {
        // *** HANDLE EVENTS AND INPUT (if any) ***
        SSDL_DefaultEventHandler();

        // *** DISPLAY THINGS ***
        SSDL_RenderClear();   // first, erase previous frame
        drawBalls(balls);

        // *** UPDATE  THINGS ***
        moveBalls(balls);
        bounceBalls(balls);   // if ball moves offscreen, 
        //  reverse its direction
    }

    return 0;
}

// Ball functions

void initializeBalls(std::span<Ball> balls)
{
    // Where to put the next ball
    int x = 0, y = SSDL_GetWindowHeight() / 6;

    for (Ball& b : balls)
    {
        b.location_ = { x, y };
        b.direction_ = Direction::RIGHT;

        x += SSDL_GetWindowWidth() / 3;
        y += SSDL_GetWindowHeight() / 3;
    }
}

void drawBalls(std::span<const Ball> balls)
{
    for (Ball b : balls)
        SSDL_RenderDrawCircle(b.location_.x_,
            b.location_.y_, RADIUS);
}

// update balls' x positions based on speed & current dir
void moveBalls(std::span<Ball> balls)
{
    for (Ball& b : balls)
        if (b.direction_ == Direction::LEFT)
            b.location_.x_ -= SPEED;
        else
            b.location_.x_ += SPEED;
}

void bounceBalls(std::span<Ball> balls)
{
    // if any ball moves off screen, reverse its direction
    for (Ball& b : balls)
        if (b.location_.x_ >= SSDL_GetWindowWidth())
            b.direction_ = Direction::LEFT;
        else if (b.location_.x_ < 0)
            b.direction_ = Direction::RIGHT;
}
