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

//This is SSDL_Font, a wrapper for TTF_Font

#include <string>
#include <cstring>
#include <sstream>
#include "SSDL_font.h"
#include "SSDL_exception.h"
#include "SSDL_display.h"

using namespace std;

//Useful for adding (or not) ".ttf" to the end of filenames
bool matchFromEnd(const char* str, const char* substr)
{
    char* strEnd = (char*)str + strlen(str) - 1;
    char* substrEnd = (char*)substr + strlen(substr) - 1;

    for (;; --strEnd, --substrEnd)
    {
        if (toupper(*substrEnd) != toupper(*strEnd))  return false;

        if (substrEnd <= substr)    return true;
        else if (strEnd <= str)     return false;
    }
}

//All this does is set up the full path for a font
std::string SSDL_CreateFullFontPath(const char* fontPath, const char* filename)
{
        std::string newPath(fontPath); newPath += filename;
        if (!matchFromEnd(filename, ".ttf")) newPath += ".ttf";
        return newPath;
}

TTF_Font* SSDL_OpenSystemFont(const char* filename, int point)
{
    string finalFilename = SSDL_CreateFullFontPath(SSDL_Display::Instance().systemFontPath(), filename);
    return SSDL_OpenFont(finalFilename.c_str(), point);
    //Calling SSDL_OpenFont does mean we do matchFromEnd twice -- a little inefficient, but separate handling would make code more complex
}

TTF_Font* SSDL_OpenFont(const char* filename, int point)
{
    SSDL_Display::Instance(); //be sure SSDL is initialized...

    //We hope the filename ends in .ttf.  If not, and there's no extension, append it.
    //If not, and there is an (incorrect) extension, go ahead and append it, and watch it fail.

    string finalFilename = filename;
    if (!matchFromEnd(filename, ".ttf")) finalFilename += ".ttf";

    TTF_Font* result = TTF_OpenFont(finalFilename.c_str(), point);
    if (!result) 
        throw SSDL_Exception(std::string("Can't open font ") + filename + ".");
    
    return result;
}


