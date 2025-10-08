// Program to get a mouse click, and report its location
//        -- from _C++26 for Lazy Programmers_

#include "SSDL.h"

int main(int argc, char** argv)
{
    sout << "Click the mouse and see where you clicked.\n";

    // Get the mouse click
    SSDL_WaitMouse();                 // wait for click...
    int xLocation = SSDL_GetMouseX(); // and get its location
    int yLocation = SSDL_GetMouseY();

    // Print the mouse click
    sout << "The X position of your click was " 
         << xLocation << "\n";
    sout << "The Y position of your click was "
         << yLocation << "\n";

    // End the program
    sout << "\nHit a key to end the program.\n";

    SSDL_WaitKey();

    return 0;
}
