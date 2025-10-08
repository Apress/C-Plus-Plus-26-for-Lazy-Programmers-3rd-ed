/*Simple SDL, a wrapper library for SDL.
  
  Copyright (C) 2025 Will Briggs.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution. */

//This is ssdl_exception.cpp, which provides an exception class that SSDL throws

#ifndef SSDL_EXCEPTION_H
#define SSDL_EXCEPTION_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <exception>

class SSDL_Exception: public std::runtime_error
{
public:
	//I have this exception to handle the error msg automatically unless I have something specific to say
	SSDL_Exception()                     : SSDL_Exception(SDL_GetError()) {}
    SSDL_Exception(const std::string& s) : SSDL_Exception(s.c_str()) {}
    SSDL_Exception(const char* s) : std::runtime_error::runtime_error(s) {}
};

void SSDL_terminate ();
#endif