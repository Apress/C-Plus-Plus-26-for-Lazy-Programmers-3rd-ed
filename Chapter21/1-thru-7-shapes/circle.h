// Circle class, for use with the SSDL library
//   -- from _C++26 for Lazy Programmers_

#ifndef CIRCLE_H
#define CIRCLE_H

#include <utility> // for std::move
#include "shape.h"

class Circle: public Shape
{
public:
   Circle(const Point2D& p = {}, int theRadius = 0, const char* txt = "", const SSDL_Color& c = WHITE) :
      Shape(p, txt, c), radius_(theRadius) 
   {
   } 
   Circle(const Circle& c) = delete;
   Circle& operator=(const Circle& c) = delete;

   int radius() const { return radius_; }

   void drawAux() const override
   { 
      SSDL_RenderDrawCircle(location().x_, location().y_, radius()); 
   }
private:
   int radius_;
};

#endif //CIRCLE_H
