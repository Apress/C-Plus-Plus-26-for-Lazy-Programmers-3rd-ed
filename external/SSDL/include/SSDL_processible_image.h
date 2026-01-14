// A processible image class to work with the SSDL library 
//		  -- from _C++26 for Lazy Programmers_

#ifndef SSDL_PROCESSIBLE_IMAGE
#define SSDL_PROCESSIBLE_IMAGE
#include <cassert>
#include <vector>
#include <span>
#include "SSDL_image.h"
#include "SSDL_color.h"
#include "SSDL_exception.h"

void copyTexture(SDL_Texture* dest, SDL_Texture* src, int w, int h);

class SSDL_ProcessibleImage 
{
public:
    SSDL_ProcessibleImage() : sdlTexture_(nullptr, SDL_DestroyTexture), 
        pixelFormat_(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), SDL_FreeFormat)
    {
    }
    SSDL_ProcessibleImage(const SSDL_ProcessibleImage& other) : sdlTexture_(nullptr, SDL_DestroyTexture) 
    { 
        copy(other);
    }
    SSDL_ProcessibleImage(SSDL_ProcessibleImage&& other) noexcept = default;
    SSDL_ProcessibleImage(SDL_Texture* sdlTexture) :
        sdlTexture_(sdlTexture, SDL_DestroyTexture),
        pixelFormat_(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), SDL_FreeFormat)
    {
        //Get width and height...
        Uint32 format;
        int access;
        SDL_QueryTexture(sdlTexture, &format, &access, &width_, &height_);
        if (format != SDL_PIXELFORMAT_RGBA8888 || access != SDL_TEXTUREACCESS_STREAMING)
            throw SSDL_Exception("Bad pixel format or access in SSDL_ProcessibleImage::SSDL_ProcessibleImage.");

        setupData();
    }

    const SSDL_ProcessibleImage& operator=(const SSDL_ProcessibleImage& other) 
    { 
        copy(other); return *this; 
    }
    SSDL_ProcessibleImage&       operator=(SSDL_ProcessibleImage&&      other) noexcept = default;

    int width            () const { return width_;            }
    int height           () const { return height_;           }
    int size             () const { return width() * height();} 
    operator SDL_Texture*()       { return sdlTexture_.get(); }
    operator SDL_Texture*() const { return sdlTexture_.get(); }

    //Image processing: getting pixels out of the SSDL_ProcessibleImage and putting them back in

    std::span<SSDL_Color> getPixels() const { return std::span<SSDL_Color>(dataSSDL_Color_.get(), width_ * height_); }
    void setPixels(std::span<SSDL_Color> colors);               //use this if altering colors but not size
    void setPixels(std::span<SSDL_Color> colors, int w, int h); //use this if making a new image size

    void commit(); //Update the texture to match the colors. Called automatically by setTo

    //Saving. Can only save to BMP.
    void save(const char* filename);
private:
    //convert pixels back and forth between SSDL_Color and Uint32
    SSDL_Color toRGB   (Uint32        pixel) const { SSDL_Color c; SDL_GetRGBA(pixel, pixelFormat_.get(), &c.r, &c.g, &c.b, &c.a); return c; }
    Uint32     toUint32(const SSDL_Color& c) const { return SDL_MapRGBA(pixelFormat_.get(), c.r, c.g, c.b, c.a); }

    void setupData(); //Given the texture, set up dataUint32_ and dataSSDL_Color
    void copy(const SSDL_ProcessibleImage&);

    int width_ =0, height_ = 0;
    std::shared_ptr<SDL_PixelFormat> pixelFormat_; //needed to interpret the pixels in SSDL_Image's texture
                                                   //shared because it's always going to be the same
    std::unique_ptr<SDL_Texture, void(*) (SDL_Texture*)> sdlTexture_;      //The texture itself
    std::unique_ptr<Uint32   []> dataUint32_;      //Its representation in Uint32s
    std::unique_ptr<SSDL_Color[]> dataSSDL_Color_; //...and in SSDL_Colors
};

SSDL_ProcessibleImage SSDL_LoadProcessibleImage(const char* filename);
#endif

