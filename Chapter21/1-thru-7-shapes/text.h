// Text class, for use with the SSDL library
//      -- from _C++26 for Lazy Programmers_

#ifndef TEXT_H
#define TEXT_H

#include "shape.h"

class Text : public Shape
{
public:
   Text(const Point2D& p = {}, const char* txt = "", const char* name = "", const SSDL_Color& c = WHITE) :
      Shape(p, name, c)
   {
      copy(txt);
   }
   Text(const Text& other) = delete;
   ~Text() override { if (contents_) delete contents_; }

   Text& operator=(const Text& other) = delete;

   void drawAux() const override
   {
      SSDL_RenderText (contents_, location().x_, location().y_);
   }

   const char* contents() const { return contents_; }
private:
   char* contents_;
   void copy(const char* txt) //used for copying contents
   {
      contents_ = new char[strlen(txt) + 1]; strcpy(contents_, txt);
   }
};

#endif //TEXT_H
