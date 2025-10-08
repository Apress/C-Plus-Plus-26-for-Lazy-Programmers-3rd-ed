// Program to play around a little with Shapes
//        -- from _C++26 for Lazy Programmers_

#include <vector>
#include "circle.h"

int main (int argc, char** argv) 
{
    Shape favoriteShape({ 0, 0 }, RED);

    Circle myCircle({ 50, 50 }, 25, RED);

    // setColor(), color() are inherited from Shape
    myCircle.setColor(BLUE);                 //no, make it BLUE

    //Recall sout is SSDL's equivalent of std::cout 
    sout << "The circle's color is now " << myCircle.color() << ".\n";

    Shape s = favoriteShape, t; t = s;
    Circle d = myCircle, e; e = d;

    SSDL_WaitKey();

    return 0;
}
