// Shape class, for use with the SSDL library
//      -- from _C++26 for Lazy Programmers_

#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include "SSDL.h"
#include "point2d.h"

class Shape
{
public:
    Shape(const Point2D& p = {}, const SSDL_Color& c = WHITE) : location_(p), color_(c)
    {
    }
    Shape(const Shape& other) : Shape (other.location_, other.color_) {}

    //Shape& operator= (const Shape& s) = default;
    Shape& operator= (const Shape& other)
    {
        if (this == &other) return *this;
        location_ = other.location_; color_ = other.color_;
        return *this;
    }
    // Color
    void  setColor(const SSDL_Color& c) { color_ = c;    }
    const SSDL_Color& color() const     { return color_; }

    // Access functions
    const Point2D&     location()    const { return location_;    }

    // Drawing
    void   draw() const;

    // Moving
    void moveTo(const Point2D& p) { location_ = p; }
    void moveBy(int deltaX, int deltaY) { moveTo({ location_.x_ + deltaX, location_.y_ + deltaY }); }
private:
    Point2D     location_;
    SSDL_Color  color_;
};
#endif //SHAPE_H
