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

//This is the SSDL_Display -- the screen you see when the program runs. 

#include <exception>
#include <cstring> 
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SSDL_display.h"
#include "SSDL_image.h"
#include "SSDL_font.h"

#ifdef _MSC_VER                                 //If it's Visual Studio
#pragma warning(disable : 4996) //Disable the "feature" that makes strcpy an error!
#endif

#define WINDOWS_FONT_PATH "C:\\Windows\\Fonts\\"
#define MACOS_FONT_PATH "/Library/Fonts/"
#define UNIX_FONT_PATH "/usr/share/fonts/"
#define MINGW_FONT_PATH "/c/windows/fonts/"

//What's this crazy stuff here?  I need to ensure that SSDL_Display::Instance()
// is called (thus initializing SDL among other things) before main starts.
//This will do it.  Why do I care?  In case someone does something in main
// with an SDL feature like mouse cursor that I am not supporting.  It won't
// work till SDL is initialized.  Now it is.

//Weakness:  what if they do this SDL feature globally?  Then order of initialization
// is not guaranteed by C++.  But if they know enough to use an SDL feature, they
// should know enough not to do things with it before SDL is initialized.  So there.

//Will Briggs, 11-12-2015

//Note: it would be really bad to call any function that used SSDL_Display::Instance() while in the ctor!
// I won't put up tests as this is only an error the library creator can make, not the library user
// Just don't do it. :)

SSDL_Display::SSDL_Display() 
{
    std::set_terminate (SSDL_terminate);

    if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
        throw SSDL_Exception ();

    sdlWindow_ = SDL_CreateWindow ("",              //no title by default
				    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				    DEFAULT_WIDTH, DEFAULT_HEIGHT,
				    0);           //flags are 0 by default
    if (! sdlWindow_) throw SSDL_Exception ();

    //defaults below:  index shd be -1 (pick the first renderer that works best)
    //rendererFlags should be 0
    int rendererIndex = -1;
    int rendererFlags = 0;
    sdlRenderer_ = SDL_CreateRenderer (sdlWindow_, rendererIndex, rendererFlags);

    if (! sdlRenderer_) throw SSDL_Exception ();

    //Some distros Unix, sometimes, don't start with a blank screen. Make them. 2021-06-08
    SDL_SetRenderDrawColor(sdlRenderer_, 0, 0, 0, 255); SDL_RenderClear(sdlRenderer_); 
    SDL_RenderPresent (sdlRenderer_);

    SDL_ClearError(); //Why? SDL2_Image documentation warns that IMG_Init does not necessarily
    // set the Error, so I figure I'd better clear it.  I hope it sets it if there's an error!
    static const int IMG_FLAGS = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF; //all available types
    if (! (IMG_Init (IMG_FLAGS) & IMG_FLAGS)) throw SSDL_Exception ();

    if (TTF_Init () == -1) throw SSDL_Exception ();

    //Had to avoid calling SSDL_SetRenderDrawColor because it calls SSDL_Display::Instance
    //which calls this ctor, and a thread-safe mechanism got this thing stuck waiting forever
    SDL_SetRenderDrawColor(sdlRenderer_, 255, 255, 255, 255);//the default color for drawing is WHITE.  I don't use the constant
    //because I don't _know_ that it's been initialized yet.

    int point = 16;
    std::string filename = "NotoSans_Condensed-Bold.ttf"; 
    std::string finalFilename;

    //If I am running an SSDL program, it has to be where it can find its default font
    //If I am building, it'll be in SSDL_FONT_PATH2. 
    //Test the first, and if that fails, test the 2nd. If that fails, give up.
    //I put them in this order because I don't want to be testing in a folder named "external -- trial"
    // and have it keep looking in "external."

#define SSDL_FONT_PATH1 "../fonts/Noto_Sans/static/" 
#define SSDL_FONT_PATH2 "../../external/SSDL/fonts/Noto_Sans/static/"

    finalFilename = SSDL_CreateFullFontPath(SSDL_FONT_PATH1, filename.c_str());
    currentFont_  = TTF_OpenFont(finalFilename.c_str(), point);
    if (!currentFont_)   //It's not in ../../external/SSDL/fonts. Is it in ../fonts?
    {
        finalFilename = SSDL_CreateFullFontPath(SSDL_FONT_PATH2, filename.c_str());
        currentFont_  = TTF_OpenFont(finalFilename.c_str(), point);
    }
    if (!currentFont_)   //Still not found? Give up
        //Note: VS doesn't thereby do a popup window if in Debug configuration.
    {
        std::string msg = std::string("Missing default font "); msg += filename;
        msg += ".It's likely looking in the wrong folder.\n"
            "Has the folder you're compiling in been moved?\n"
            "This font should be in folder <repository>/external/SSDL/fonts/Arimo/static.\n";

        throw SSDL_Exception(msg.c_str());
    }
    TTF_SetFontHinting(currentFont_, TTF_HINTING_NORMAL);

#ifdef _WIN32
    systemFontPath_ = WINDOWS_FONT_PATH;
#elif defined(__APPLE__)
    systemFontPath_ = MACOS_FONT_PATH;
#elif defined(__MINGW32__) || defined(__MINGW64__)
    systemFontPath_ = MINGW_FONT_PATH;
#else //It's Unix
    systemFontPath_ = UNIX_FONT_PATH;
#endif
    //If none of these font paths work, you can add your own here like so:
    // systemFontPath = "/wherever/this/distribution/keeps/its/fonts" 

    setCanvas(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    //Unix sometimes leaves trash on the opening screen. Don't let it
    SDL_SetRenderTarget(sdlRenderer_, canvas_);  // Now go back to drawing on the canvas.
    SDL_SetRenderDrawColor(sdlRenderer_, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer_);
    SDL_SetRenderDrawColor (sdlRenderer_, 255, 255, 255, 255); //back to white
    
    SDL_SetRenderTarget(sdlRenderer_, nullptr);  // Draw to the screen
    SDL_RenderCopy(sdlRenderer_, canvas_, nullptr, nullptr); //What we draw is the canvas //SDL3 calls this SDL_RenderTexture
    SDL_RenderPresent(sdlRenderer_);             // OK, send it to the screen
    SDL_SetRenderTarget(sdlRenderer_, canvas_);  // Now go back to drawing on the canvas.
}

SSDL_Display::~SSDL_Display ()
{
    //Here's why this is commented out: it crashes if compiled with g++.
    // I think the reason is that fonts must not be destructed after TTF_Quit,
    // Surfaces/textures must not be destructed after IMG_Quit.
    // But these things are destructed by their own dtors, and may be declared globally
    // I could try fixing this one day, but it's not urgent and would require overhead and thus slightly slower and more complex code
    // It *is* safe to call all these Quits if initialization failed or never happened
    // 
    //TTF_Quit(); 
    //IMG_Quit();
    ////SDL_DestroyRenderer (sdlRenderer_); //not needed, but OK //It was noticeably causing delay, so I commented it out
    //SDL_DestroyWindow(sdlWindow_); //not needed, but OK
    //SDL_Quit ();
}

void SSDL_Display::setCanvas(int w, int h)
{
    canvas_ = SDL_CreateTexture(sdlRenderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h); 
    SDL_Rect full = { 0, 0, w, h };
    SDL_SetRenderTarget(sdlRenderer_, nullptr);
    SDL_RenderSetViewport(sdlRenderer_, &full); //This line makes the viewport really be the new size.
    SDL_SetTextureBlendMode(canvas_, SDL_BLENDMODE_BLEND); //Make sure we can do alpha blending on this canvas
    SDL_SetRenderTarget(sdlRenderer_, canvas_);  //This tells SSDL that everything we draw will go to canvas_, not the screen
}

void SSDL_Display::renderPresent()
{
    SDL_SetRenderTarget(sdlRenderer_, nullptr);  // Draw to the screen
    SDL_RenderCopy(sdlRenderer_, canvas_, nullptr, nullptr); //What we draw is the canvas //SDL3 calls this SDL_RenderTexture
    SDL_RenderPresent(sdlRenderer_);             // OK, send it to the screen
    SDL_SetRenderTarget(sdlRenderer_, canvas_);  // Now go back to drawing on the canvas.
}

//See if *ch is one of the delimiters
//Delimiters are required to be ASCII
//ch isn't, but if it's not, it's not a delimiter
bool isADelimiter(char* ch, const char* delimiters)
{
    if (*ch & 0x80)   //if that particular bit is nonzero, it's not ASCII
        return false; //so it's not a delimiter
    else              //else see if it's in our delimiters list
        return std::strchr(delimiters, static_cast<char>(*ch));
}

//How many bytes are in this UTF-8 character?
int advanceUtf8(char* ptr)
{
    if ((*ptr & 0b10000000) == 0b00000000) return 1; //If the leftmost bit is 0, it's ASCII: 1 byte
    if ((*ptr & 0b11100000) == 0b11000000) return 2; //2 bytes long
    if ((*ptr & 0b11110000) == 0b11100000) return 3; //3 bytes
    if ((*ptr & 0b11111000) == 0b11110000) return 4; //4 bytes
    return 1;                                        //Not recognized! Just treat it as ASCII
}

char* myUTF8Strtok(char* str, const char* delimiters)
{
    static char* lastStringGiven = nullptr;
    if (str != nullptr) lastStringGiven = str; //It's always the last real string passed in (nullptr doesn't count)

    if (!lastStringGiven) return nullptr; //If first time ever called someone passed in nullptr (stupid, but still), give up
    if (*lastStringGiven == u8'\0') return nullptr; //If we're out of tokens, give up

    // Skip leading delimiters
    while (*lastStringGiven != '\0' && isADelimiter(lastStringGiven, delimiters))
    {
        lastStringGiven += advanceUtf8(lastStringGiven);
    }
    if (*lastStringGiven == '\0') return nullptr;

    char* end = lastStringGiven;

    //loop thru chars in lastStringGiven till we reach '\0' or a delimiter
    while (*end != '\0' && !isADelimiter(end, delimiters)) { end += advanceUtf8(end); }


    //if we are at the end of the string, 
    // then we should not make lastStringGiven = end+1. It shoudl just be end
    char* result = lastStringGiven;

    if (*end == '\0') //then we should NOT add 1 to lastStringGiven later
        lastStringGiven = end;
    else
    {
        char* next = end + advanceUtf8(end); // compute BEFORE overwriting *end
        *end = '\0';
        lastStringGiven = next;
    }

    return result;
}

void SSDL_Display::RenderText (const char* str, int x, int y, const TTF_Font* font, bool isCentered)
//It's possible that the str may contain \n or \r.  If so, it should be broken into lines
// and printed.  If centered, we need to remember the center point.  If not, we need to remember the starting point
//It's often a single char, but may be a UTF8 char of up to 4 bytes
{
    //This is all about splitting str by using strtok
    // Because std::string doesn't support this, I must do my own dynamic memory

    char* temp = new char [strlen(str)+1]; //strtok destroys its string, so I must make a copy
    strcpy (temp, str);

    char* nextChunk = myUTF8Strtok (temp, "\n\r");
    while (nextChunk)
    {
        RenderWord (nextChunk, x, y, font, isCentered);
        y += fontLineSkip ();
        nextChunk = myUTF8Strtok (nullptr, "\n\r");
    }
    delete [] temp;
}

void SSDL_Display::RenderWord (const char* str, int x, int y, const TTF_Font* font, bool isCentered)
{
    SDL_Surface* surfaceToPrintOn = TTF_RenderUTF8_Blended ((TTF_Font*) font, str, SSDL_GetRenderDrawColor ()); //TTF_RenderUTF8_Solid was clunky and ugly. This is beautiful
    //Why cast away the const? Because TTF_RenderTextSolid expects no const, though it should
    //Why use UTF8? Because a) I want to support UTF8 and b) ASCII is a subset of UTF8 so I can use it here too.
    if (! surfaceToPrintOn) throw SSDL_Exception ();

    SDL_Texture* textureToPrint;
    textureToPrint = SDL_CreateTextureFromSurface (sdlRenderer_, surfaceToPrintOn);

    if (! textureToPrint) throw SSDL_Exception ();

    int centerAdjustment = isCentered ? surfaceToPrintOn->w/2 : 0;
    SDL_Rect whereToPutIt = { x - centerAdjustment, y, surfaceToPrintOn->w, surfaceToPrintOn->h };

    if (SDL_RenderCopy (sdlRenderer_, textureToPrint, nullptr, &whereToPutIt) < 0)
        throw SSDL_Exception ();
    //If we used RenderCopyEx, we could also rotate the text, or mirror it vertically or horizontally
    //Another time perhaps
    SDL_DestroyTexture (textureToPrint); 
    SDL_FreeSurface (surfaceToPrintOn);
}

//inline
//SDL_Surface* SSDL_CopySurface(SDL_Surface* sdlSurface)
//{
//  return SDL_ConvertSurface(sdlSurface, sdlSurface->format, 0);
//}
//WSB: remove when sure not needed 2025-05-1


/*void SSDL_Display::scrollVertically (unsigned int pixels) //Not currently done; maybe nice in next version
  {
  SDL_Rect srcRect, destRec;
  srcRect.x = destRec.x = 0;
  srcRect.y = pixels;
  destRec.y= 0;
  srcRect.w = destRec.w = sdlSurface_->w; //If I need sdlSurface_, get it at last minute with SDL_GetWindowSurface
  srcRect.h = destRec.h = sdlSurface_->h - pixels;

  SDL_Surface* temp = SSDL_CopySurface (sdlSurface_);
  //SDL_FillRect (temp, &srcRect, SDL_MapRGB (sdlSurface_->format, 255, 255, 255));
  //I don't know if you can copy a surface onto itself...
  SDL_BlitSurface (temp, nullptr, sdlSurface_, nullptr);
  //SDL_BlitSurface (temp, &srcRect, sdlSurface_, &destRec); //I think it's not blitting -- I think the
  //then a bkgd-color rectangle at the bottom
  }
*/
void SSDL_RenderClear(const SSDL_Color& c)
{
    SSDL_Color old = SSDL_GetRenderDrawColor();
    SSDL_SetRenderDrawColor(c);
    SSDL_RenderClearAux();
    SSDL_SetRenderDrawColor(old);
}

void SSDL_Display::setWindowSize(int w, int h)
{
#ifdef __unix__
  SDL_Event e; while (SDL_PollEvent(&e));
#endif
  //In Unix sometimes if I don't do this, the
  //next call to SDL_GetWindowSize will get the old
  //dimensions, not the new. Go figure.

  SDL_SetWindowSize(SSDL_Display::Instance(), w, h);

#ifdef __unix__
  for (int i = 0; i < 40; ++i) //Must be 6 or more to be reliable
  {
    SSDL_RenderClear ();       //If I don't try to clear INSIDE THE LOOP! I may have the window
                               //resized but not updated, so I have background garbage in my window
                               //while we wait for the windows manager to get done with things
                           
    SDL_RenderPresent(SSDL_Display::Instance());
    SDL_Delay(30);             //10 works; 1 doesn't
  }
  //A single SDL_Delay(60); usually works
  //but sometimes does not
  //The current numbers 40x30, are there to make it work with OpenSuSe. Would like to have
  // a more rational long-term fix. Maybe SDL.org will have one by now.
#endif

  //Programs in the repository that fail on some unix installations unless I do this
  // are Ch2/[any but sound], ch4/search*, ch9/*, ch10/ticTacToeBoard, ch11/staircase

  //The earlier SDL_Event e;... line is also needed for ch11/spritefish* and possibly
  // for other programs with game loops
  //#elif __unix__
  //...
  //#endif

  SDL_DestroyTexture(canvas_); setCanvas(w, h);
  SDL_RenderSetViewport(sdlRenderer_, nullptr);  // Reset viewport to new window size
  SDL_RenderSetClipRect(sdlRenderer_, nullptr);  // Ensure no clipping is preventing full display
  SSDL_RenderPresent(); 
}
