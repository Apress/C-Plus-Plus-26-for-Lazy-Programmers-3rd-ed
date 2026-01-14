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

//This is SSDL_Image.h.  Since it's a wrapper, it can handle whatever image formats
//  SDL2_Image supports.

//Essentially it's SDL_Texture* except that it has a destructor. 

#include <SDL.h>
#include "SSDL_display.h"
#include "SSDL_image.h"

SSDL_Image SSDL_LoadImage(const char* filename)
{
	SSDL_Display::Instance(); //be sure SDL is initialized before calling IMG_Load

	SDL_Surface* sdlSurface = IMG_Load(filename);
	if (!sdlSurface) throw SSDL_Exception();

	SDL_Texture* result = SDL_CreateTextureFromSurface(SSDL_Display::Instance(), sdlSurface);
	if (!result) throw SSDL_Exception(); 
	SDL_FreeSurface(sdlSurface);

	return SSDL_Image(result);
}

void SSDL_RenderImage(SDL_Texture* image, int x, int y, int stretchWidth, int stretchHeight)
{
    SDL_Rect dst; //src is dimensions of image; dst is where it's goin on screen
    dst.x = x; dst.y = y;

    if (stretchWidth == 0 || stretchHeight == 0) //what if they put in 0 for either?  then dont' stretch
        SDL_QueryTexture(image, nullptr, nullptr, &dst.w, &dst.h); //get width and height of image
    else
    {
        dst.w = stretchWidth; dst.h = stretchHeight;
    }

    SDL_RenderCopy(SSDL_Display::Instance(), image, nullptr, &dst);
}