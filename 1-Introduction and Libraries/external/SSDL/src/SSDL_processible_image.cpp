// A processible image class to work with the SSDL library 
//          -- from _C++26 for Lazy Programmers_
 
#include "SSDL_processible_image.h"
#include "SSDL_display.h"

void copyTexture(SDL_Texture* dest, SDL_Texture* src, int w, int h)
{
    void* srcPixels;
    void* destPixels;
    int srcPitch, destPitch;

    //lock source and destination textures
    if (SDL_LockTexture(src,  nullptr, &srcPixels,  &srcPitch ) !=0) throw SSDL_Exception();
    if (SDL_LockTexture(dest, nullptr, &destPixels, &destPitch) !=0) { SDL_UnlockTexture(src); throw SSDL_Exception(); }
    
    if (srcPitch != destPitch) throw SSDL_Exception("Source and dest differ in dimensions in SSDL_ProcessibleImage.cpp's copyTexture");

    //Why not just do one memcpy for all? apparently pitch_ may not = width()*sizeof(Uint32); 
    for (int y = 0; y < h; ++y)
        memcpy(static_cast<char*>(destPixels)+y*srcPitch, static_cast<char*>(srcPixels) + y * srcPitch, w * sizeof(Uint32));

    //unlock
    SDL_UnlockTexture(dest); SDL_UnlockTexture(src);
}

void SSDL_ProcessibleImage::setupData()
{
    dataUint32_     = std::make_unique<Uint32    []>(size());
    dataSSDL_Color_ = std::make_unique<SSDL_Color[]>(size());

    void* pixels; int pitch;
    if (SDL_LockTexture(static_cast<SDL_Texture*>(*this), nullptr, &pixels, &pitch) != 0)
        throw SSDL_Exception();

    //Get dataUint32_ from the texture itself
    
    //Why not just do one memcpy for all? apparently pitch_ may not = width()*sizeof(Uint32)
    for (int y = 0; y < height(); ++y)
        memcpy(&dataUint32_.get()[y * width()], static_cast<char*>(pixels) + y * pitch, width() * sizeof(Uint32));

    //Get dataSSDL_Color_ from dataUint32_
    for (int p = 0; p < size(); ++p) dataSSDL_Color_[p] = toRGB(dataUint32_[p]);

    SDL_UnlockTexture(static_cast<SDL_Texture*>(*this));
}

SSDL_ProcessibleImage SSDL_LoadProcessibleImage(const char* filename)
{
    SSDL_Display::Instance(); //be sure SDL is initialized before calling IMG_Load

    //Load the surface from the image file
    SDL_Surface* loadedSurface = IMG_Load(filename);
    if (!loadedSurface) throw SSDL_Exception();

    //Convert it to SDL_PIXELFORMAT_RGBA8888, which is friendly to direct access to pixels 
    //  (I think; anyway, it works)
    SDL_Surface* sdlSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    if (!sdlSurface) throw SSDL_Exception();

    //Now make a texture from it, because that's what we render
    SDL_Texture* result = SDL_CreateTexture(SSDL_Display::Instance(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        sdlSurface->w, sdlSurface->h);
    if (!result) throw SSDL_Exception();
    
    //Copy the surface into the texture
    void* pixels;
    int pitch;
    SDL_LockTexture(result,nullptr,&pixels, &pitch);
        memcpy(pixels, sdlSurface->pixels, sdlSurface->h * sdlSurface->pitch);
    SDL_UnlockTexture(result);

    SDL_FreeSurface(sdlSurface); SDL_FreeSurface(loadedSurface);
    
    return SSDL_ProcessibleImage(result);
}

void SSDL_ProcessibleImage::copy (const SSDL_ProcessibleImage& other)
{
    width_ = other.width_; height_ = other.height_; pixelFormat_ = other.pixelFormat_;

    //Create *this's texture
    SDL_Texture* temp = SDL_CreateTexture(SSDL_Display::Instance(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, other.width(), other.height());
    sdlTexture_ = std::unique_ptr<SDL_Texture, void(*) (SDL_Texture*)>(temp, SDL_DestroyTexture);
    
    //Copy into it
    copyTexture(sdlTexture_.get(), other.sdlTexture_.get(), other.width_, other.height_);

    //Now create dataSSDL_Color_ and dataUint32_
    setupData();
}

//Saving
void SSDL_ProcessibleImage::save(const char* filename)
{
    //Note that the pitch may be changed later by the system and revealed by SDL_LockTexture, which is fine
    SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceWithFormatFrom(static_cast<void*>(dataUint32_.get()), width(), height(), 32, width()*sizeof(Uint32), SDL_PIXELFORMAT_RGBA8888);
    if (!sdlSurface) throw SSDL_Exception();

    if (SDL_SaveBMP(sdlSurface, filename)!=0) 
        throw SSDL_Exception();

    SDL_free(sdlSurface);
}

//Allowing image processing
void SSDL_ProcessibleImage::setPixels(std::span<SSDL_Color> pixels, int w, int h)
{
    width_ = w; height_ = h;
    if (size() > pixels.size())
        throw SSDL_Exception("Image size too small in SSDL_ProcessibleImage::setPixels");

    //Create a new texture
    SDL_Texture* temp = SDL_CreateTexture(SSDL_Display::Instance(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    sdlTexture_ = std::unique_ptr<SDL_Texture, void(*) (SDL_Texture*)>(temp, SDL_DestroyTexture);

    //  and new versions of dataUint32_, dataSSDL_Color_
    dataSSDL_Color_ = std::make_unique<SSDL_Color[]>(size());
    dataUint32_     = std::make_unique<Uint32    []>(size());
    int pitch = w * sizeof(SSDL_Color);
    for (int y = 0; y < h; ++y)
        memcpy(dataSSDL_Color_.get() + y * w, pixels.data() + y * w, pitch);

    commit();
}

void SSDL_ProcessibleImage::setPixels(std::span<SSDL_Color> colors)
{
    if (colors.size() != size()) throw SSDL_Exception("Can only SSDL_ProcessibleImage::setTo data of same size unless size specified.");
    if (!dataSSDL_Color_)        throw SSDL_Exception("Can only SSDL_ProcessibleImage::setTo if already initialized.");
    memcpy(dataSSDL_Color_.get(), colors.data(), colors.size() * sizeof(SSDL_Color));
    commit();
}

//Those changes you've made to the SSDL_Color view? Put 'em in dataUint32_ and into the texture.
void SSDL_ProcessibleImage::commit()
{
    for (int p = 0; p < size(); ++p) dataUint32_[p] = toUint32(dataSSDL_Color_[p]);

    void* pixels; int pitch;
    if (SDL_LockTexture(static_cast<SDL_Texture*>(*this), nullptr, &pixels, &pitch) != 0)
        throw SSDL_Exception();

    for (int y = 0; y < height(); ++y)
        memcpy(static_cast<char*>(pixels) + y * pitch, &dataUint32_[y * width()], width() * sizeof(Uint32));

    SDL_UnlockTexture(static_cast<SDL_Texture*>(*this));
}
