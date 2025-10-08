//An image with a caption
//    -- from _C++26 for Lazy Programmers_

#ifndef CAPTIONED_IMAGE_H
#define CAPTIONED_IMAGE_H

#include <utility> //for std::move
#include <string>
#include "SSDL.h"

class CaptionedImage: public SSDL_Image
{
public:
    CaptionedImage() = default;
    CaptionedImage(const SSDL_Image& other, const char* c) : 
        SSDL_Image(other), caption_(c) 
    {
    }
    CaptionedImage(const CaptionedImage& other) : 
        CaptionedImage(other, other.caption_.c_str()) 
    {
    }
    CaptionedImage(CaptionedImage&& other) noexcept : 
        SSDL_Image(std::move(other)), caption_(std::move(other.caption_)) 
    {
    }

    CaptionedImage& operator=(const CaptionedImage& other)
    {
        if (this == &other) return *this; //no self-assignment
        SSDL_Image::operator=(other); caption_ = other.caption_; return *this;
    }
    CaptionedImage& operator=(CaptionedImage&& other) noexcept
    {
        SSDL_Image::operator=(std::move(other)); 
        caption_ = std::move(other.caption_); 
        return *this;
    }
    void draw(int x, int y)
    {
        SSDL_RenderImage(*this, x, y); 
        SSDL_RenderTextCentered(caption_.c_str(), 
                                x + SSDL_GetImageWidth(*this) / 2, y + SSDL_GetImageHeight(*this));
    }
private:
    std::string caption_;
    const char* junk_ = nullptr;
};
#endif //CAPTIONED_IMAGE_H

