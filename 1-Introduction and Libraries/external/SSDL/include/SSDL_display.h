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

#ifndef SSDL_DISPLAY_H
#define SSDL_DISPLAY_H

#include <sstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "SSDL_color.h"
#include "SSDL_exception.h"
#include "SSDL_font.h"

class SSDL_Image;

class SSDL_Display
{
public:
    inline static constexpr int DEFAULT_WIDTH = 640, DEFAULT_HEIGHT = 480;

    SSDL_Display                  (const SSDL_Display&) = delete;
    const SSDL_Display& operator= (const SSDL_Display&) = delete;

    static SSDL_Display& Instance ()
    {
        static SSDL_Display myInstance;
        return myInstance;
    }       

    //RenderWord probably renders a single char, but it could be up to 4 bytes long if it's really utf8
    // it could render a word, or a line
    void RenderWord        (const char* str, int x, int y, const TTF_Font* font, bool isCentered = false);
    void RenderText        (const char* str, int x, int y, const TTF_Font* font, bool isCentered = false);

    bool isTimeToQuit      () const { return isTimeToQuit_; }
    void declareTimeToQuit ()       { isTimeToQuit_ = true; }

    operator SDL_Renderer* () const { return sdlRenderer_;  }
    operator SDL_Window*   () const { return sdlWindow_;    }

    void renderPresent();

    void setWindowSize (int w, int h);

    //fontLineSkip is how far to go down to next line
    //advance is how far to go right to next char
    //Why not move them to SSDL_Font? Keeping SSDL_Font minimal means I can pass TTF_Font* not
    // SSDL_Font between SSDL functions. Efficient and simple.
    int  advance(char32_t ch) const
    {
        int result;
        //Why TTF_GlyphMetrics32? So we can take a Unicode code point (character) not just a regular ASCII
        if (TTF_GlyphMetrics32 ((TTF_Font*) SSDL_GetCurrentFont(), ch,  nullptr, nullptr, nullptr, nullptr, &result))
            throw SSDL_Exception ();
        return result;
    }
    int fontLineSkip () const
    {
        return TTF_FontLineSkip((TTF_Font*) SSDL_GetCurrentFont());
    }

    const SSDL_Color& background    () const { return background_;  }
    void setBackground (const SSDL_Color& c) { background_ = c;     }

    TTF_Font*     currentFont   () const { return currentFont_; }
    void          setCurrentFont(TTF_Font* newFont) { currentFont_ = newFont; }
    const char*   systemFontPath() const { return systemFontPath_.c_str();}
private:
    SDL_Window*   sdlWindow_;
    SDL_Renderer* sdlRenderer_;
    TTF_Font*     currentFont_;
    std::string   systemFontPath_;
    SSDL_Color    background_;

    SDL_Texture* canvas_ = nullptr;
    void setCanvas(int width, int height);
    //2025-04-14 We draw on this canvas texture and post it on screen on SSDL_RenderPresent. Why not just
    // write on the screen? Because SDL makes no guarantees not to forget what's on it
    // when you SDL_RenderPresent. Which means you have to draw everything again when
    // you update the screen. Which means you have to *start* learning with a game loop. Too hard for beginners.
    // SDL2 will let you get away without this, but it wasn't guaranteed. SDL3 will not

    bool          isTimeToQuit_ = false;

    SSDL_Display  ();       
    ~SSDL_Display ();
};

//Text output
inline
void SSDL_RenderTextAux(const char* str, int x, int y, bool isCentered, const TTF_Font* font)
{
    SSDL_Display::Instance().RenderText (str, x, y, font, isCentered);
}

template <typename T>
inline
void SSDL_RenderTextJustified(const T& thing, int x, int y, bool isCentered, const TTF_Font* font)
{
    std::ostringstream os; os << thing; SSDL_RenderTextAux (os.str().c_str(), x, y, isCentered, font); 
}

inline
void SSDL_RenderTextJustified(const char* thing, int x, int y, bool isCentered, const TTF_Font* font)
{
    SSDL_RenderTextAux(thing, x, y, isCentered, font);
}

inline
void SSDL_RenderTextJustified(const std::string& thing, int x, int y, bool isCentered, const TTF_Font* font)
{
    SSDL_RenderTextAux(thing.c_str(), x, y, isCentered, font); 
}

//For UTF-8 text
inline
void SSDL_RenderTextJustified(const char8_t* thing, int x, int y, bool isCentered, const TTF_Font* font)
{
    SSDL_RenderTextAux((const char*) thing, x, y, isCentered, font);
}

template <typename T>
inline
void SSDL_RenderText(const T& thing, int x, int y, const TTF_Font* font = SSDL_GetCurrentFont())
{
    SSDL_RenderTextJustified(thing, x, y, false, font); //false means "not centered"
}

template <typename T>
inline
void SSDL_RenderTextCentered(const T& thing, int x, int y, const TTF_Font* font = SSDL_GetCurrentFont())
{
    SSDL_RenderTextJustified(thing, x, y, true, font); //true means "centered"
}

//Rendering images. I keep this here not in SSDL_Image.h because it uses SSDL_Display
inline void SSDL_RenderImage (SDL_Texture* image, const SDL_Rect& src, const SDL_Rect& dst)
{
  if (SDL_RenderCopy (SSDL_Display::Instance(), image, &src, &dst) != 0) throw SSDL_Exception ();
}

inline void SSDL_RenderImageEx (SDL_Texture* image, const SDL_Rect& src, const SDL_Rect& dst, double angleInDegrees = 0.0,
				SDL_RendererFlip flipValue = SDL_FLIP_NONE)
{
  SDL_RenderCopyEx (SSDL_Display::Instance(), image, &src, &dst, angleInDegrees, nullptr, flipValue);
}

//Make things drawn appear on the screen right now
inline void SSDL_RenderPresent()                       { SSDL_Display::Instance().renderPresent();                      } 

//Clearing the screen
inline void  SSDL_RenderClearAux() { if (SDL_RenderClear(SSDL_Display::Instance()) < 0) throw SSDL_Exception(); }
void SSDL_RenderClear(const SSDL_Color& c = SSDL_Display::Instance().background());

//Window position
inline void SSDL_GetWindowPosition(int& x, int& y)     {     SDL_GetWindowPosition (SSDL_Display::Instance(), &x, &y);  }
inline auto SSDL_GetWindowPosition()                   { int x, y;  SDL_GetWindowPosition(SSDL_Display::Instance(), &x, &y); return std::tuple(x, y); }
inline void SSDL_GetWindowSize (int& x, int& y)        { SDL_GetWindowSize     (SSDL_Display::Instance(), &x, &y);      }

//...size
inline void        SSDL_SetWindowSize(int w, int h)    { SSDL_Display::Instance().setWindowSize(w, h);                  }
inline int         SSDL_GetWindowWidth ()              { int w, h; SSDL_GetWindowSize (w, h); return w;                 }
inline int         SSDL_GetWindowHeight()              { int w, h; SSDL_GetWindowSize (w, h); return h;                 }
inline auto        SSDL_GetWindowSize()                { int w, h; SSDL_GetWindowSize (w, h); return std::tuple (w, h); }

//...title
inline const char* SSDL_GetWindowTitle ()              { return SDL_GetWindowTitle (SSDL_Display::Instance());          }
inline void        SSDL_SetWindowTitle(const char* t)  { SDL_SetWindowTitle   (SSDL_Display::Instance(), t);            }
inline void        SSDL_SetWindowPosition (int x,int y){ SDL_SetWindowPosition(SSDL_Display::Instance(), x,y);SSDL_RenderClear (); SSDL_RenderPresent(); }//2021-06-12: I am unsure if the last 2 function calls here are needed, but that could vary by distribution.
inline void        SSDL_RestoreWindow  ()              { SDL_RestoreWindow    (SSDL_Display::Instance());     SSDL_RenderClear (); SSDL_RenderPresent(); }
inline void        SSDL_MinimizeWindow ()              { SDL_MinimizeWindow   (SSDL_Display::Instance());     SSDL_RenderClear (); SSDL_RenderPresent(); }
inline void        SSDL_MaximizeWindow ()              { SDL_MaximizeWindow   (SSDL_Display::Instance());     SSDL_RenderClear (); SSDL_RenderPresent(); }

//...current font
inline TTF_Font*   SSDL_GetCurrentFont()               { return SSDL_Display::Instance().currentFont();                 }
inline void        SSDL_SetFont(TTF_Font* newFont)     { SSDL_Display::Instance().setCurrentFont(newFont);              }

//...quit status
inline bool SSDL_IsQuit      () { return SSDL_Display::Instance().isTimeToQuit(); }
inline void SSDL_DeclareQuit () { SSDL_Display::Instance().declareTimeToQuit();   }

//...color
inline void  SSDL_SetRenderDrawColor(const SSDL_Color& c)  { SDL_SetRenderDrawColor(SSDL_Display::Instance(), c.r, c.g, c.b, c.a);      }
inline SSDL_Color SSDL_GetRenderDrawColor() { SSDL_Color c; SDL_GetRenderDrawColor(SSDL_Display::Instance(), &c.r, &c.g, &c.b, &c.a); return c; }
inline void  SSDL_SetRenderEraseColor(const SSDL_Color& c) { SSDL_Display::Instance().setBackground(c);                                 }
inline const SSDL_Color& SSDL_GetRenderEraseColor()        { return SSDL_Display::Instance().background();                              }
#endif
