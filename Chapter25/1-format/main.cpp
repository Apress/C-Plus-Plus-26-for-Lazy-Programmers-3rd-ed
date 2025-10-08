// Program to construct neat output for a HUD using std::format
//       -- from _C++26 for Lazy Programmers_

#include <format>
#include "SSDL.h"

int main(int argc, char** argv)
{
    int points = 3200;       // Some arbitrary data to test
    double time = 30.2;      //  printing to HUD
    char mood[] = "Annoyed";
        
    SSDL_RenderTextCentered(
        std::format("Points: {}   /   Time left: {}   /   Mood: {}", 
                    points, time, mood), 
        SSDL_GetWindowWidth() / 2, 10);

    SSDL_WaitKey();         // Wait for user to hit a key
    return 0;
}
