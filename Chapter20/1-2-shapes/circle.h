// Circle class, for use with the SSDL library
//   -- from _C++26 for Lazy Programmers_

#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape
{
public:
    Circle(const Point2D& p = {}, int theRadius = 0, const SSDL_Color& c = WHITE) : Shape(p, c), radius_(theRadius)
    {
    }

    Circle(const Circle& c) = default;
    Circle& operator= (const Circle& c) = default;

    int radius() const { return radius_; }

    void draw()  const 
    { 
        SSDL_SetRenderDrawColor(color()); SSDL_RenderDrawCircle(location().x_, location().y_, radius()); 
    }
private:
    int radius_;
};
#endif //CIRCLE_H
