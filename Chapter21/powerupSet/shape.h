// Shape class, for use with the SSDL library
//      -- from _C++26 for Lazy Programmers_

#ifndef SHAPE_H
#define SHAPE_H

#include "SSDL.h"
#include "point2d.h"

#ifdef _MSC_VER
#pragma warning (disable:4996) // Disable a warning about strcpy, etc.,
                               //   in Visual Studio
#endif

class Shape
{
public:
    Shape(const Point2D& p = {}, const char* text = "", const SSDL_Color& c = WHITE) : location_(p), color_(c)
    {
        copyDescription(text);
    }
    Shape(const Shape& other) = delete;
    virtual ~Shape() { if (description_) delete[] description_; }

    Shape& operator= (const Shape& s) = delete;

    // Color
    void  setColor(const SSDL_Color& c) { color_ = c; }
    const SSDL_Color& color() const { return color_; }

    // Access functions
    const Point2D& location() const { return location_; }
    const char* description() const { return description_; }

    // Drawing
    virtual void   drawAux() const = 0;
    void   draw() const
    {
        SSDL_SetRenderDrawColor(color()); drawAux();
    }

    // Moving
    void moveTo(const Point2D& p) { location_ = p; }
    void moveBy(int deltaX, int deltaY) { moveTo({ location_.x_ + deltaX, location_.y_ + deltaY }); }
private:
    Point2D    location_;
    SSDL_Color color_;
    char* description_;  // Using char* not std::string helps illustrate
                         //   how Chapter 21 affects dynamic memory
    void copyDescription(const char* desc)
    {
        description_ = new char[strlen(desc) + 1]; strcpy(description_, desc);
    }

};
#endif //SHAPE_H
