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

#ifndef SSDL_IMAGE_H
#define SSDL_IMAGE_H

#include <memory>      //for shared_ptr
#include <SDL_image.h>

inline void SSDL_QueryImage(SDL_Texture* image, int& w, int& h) { SDL_QueryTexture(image, nullptr, nullptr, &w, &h);               }
inline int  SSDL_GetImageWidth  (SDL_Texture* image)            { int w, h; SSDL_QueryImage (image, w, h); return w;               }
inline int  SSDL_GetImageHeight (SDL_Texture* image)            { int w, h; SSDL_QueryImage (image, w, h); return h;               }
inline auto SSDL_GetImageSize   (SDL_Texture* image)            { int w, h; SSDL_QueryImage (image, w, h); return std::tuple(w, h);}

class SSDL_Image
{
public:
	SSDL_Image (SDL_Texture* sdlTexture = nullptr) : sdlTexture_ (sdlTexture, SDL_DestroyTexture) 
	{ 
	}
	SSDL_Image(const SSDL_Image& other) { *this = other;  }

	SSDL_Image(SSDL_Image&& other) noexcept : sdlTexture_(other.sdlTexture_) { other.sdlTexture_ = nullptr; }
	operator       SDL_Texture* ()		 { return sdlTexture_.get (); }
	operator       SDL_Texture* () const { return sdlTexture_.get (); }
	
	int width () const { return SSDL_GetImageWidth (sdlTexture_.get()); }
	int height() const { return SSDL_GetImageHeight(sdlTexture_.get()); }
	int size()   const { auto [w, h] = SSDL_GetImageSize(sdlTexture_.get()); return w * h; }

	const SSDL_Image& operator= (const SSDL_Image& other)
	{
		sdlTexture_ = other.sdlTexture_; return *this;
	}
	const SSDL_Image& operator= (SSDL_Image&& other) noexcept
	{
		sdlTexture_ = other.sdlTexture_; other.sdlTexture_ = nullptr; return *this;
	}
private:
	std::shared_ptr<SDL_Texture> sdlTexture_;	//yes, I do need to use SDL_Texture not SDL_Surface.  
								//Efficiency.  Wd be less efficient if we manipulate the
								// bits... but we won't.
};

SSDL_Image  SSDL_LoadImage(const char* filename);
void        SSDL_RenderImage(SDL_Texture* image, int x, int y, int stretchWidth = 0, int stretchHeight = 0);
#endif