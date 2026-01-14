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

//This is SSDL stream I/O, culminating in sin (which reads from kbd, prints chars to output, and reads in text)
// and sout (which prints to screen)

#include "SSDL_display.h"
#include "SSDL_stream.h"
#include "SSDL_io.h"

using namespace std;

static SSDL_BufferOut <char> ssdlBufferOut;
SSDL_ostream sout (&ssdlBufferOut);

static SSDL_BufferIn <char> ssdlBufferIn;
istream ssin (&ssdlBufferIn);


//This is by ChatGPT so completely overhaul it if it works!
// Returns pointer to start of previous UTF-8 character, given a pointer p
// to any position after the character (e.g., end of buffer).
const char* utf8_prev_char(const char* start, const char* p)
{
	if (p <= start) return start;

	--p;
	// While we are in the middle of a UTF-8 multibyte sequence
	while (p > start && ((*p & 0b11000000) == 0b10000000)) --p;

	return p;
}

//Give it p, which is a pointer to a UTF8 character (which can be 1-4 bytes)
//THis is by ChatGPT so completely overhaul it if it works!
std::expected<char32_t, std::string> decode_utf8(const char* p, int len)
{
	unsigned char firstChar = p[0];

	if      (firstChar < 0x80)                          return firstChar;
	else if ((firstChar >> 5) == 0b110 && len > 1)  	return ((firstChar & 0x1F) << 6) | (p[1] & 0x3F);
	else if ((firstChar >> 4) == 0b1110 && len > 2)  	return ((firstChar & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
	else if ((firstChar >> 3) == 0b11110 && len > 3)	return ((firstChar & 0x07) << 18) | ((p[1] & 0x3F) << 12) | ((p[2] & 0x3F) << 6) | (p[3] & 0x3F);
	return std::unexpected("Not a valid UTF-8 character"); //used to return U'\uFFFD'; // Replacement char on error
}

bool isCompleteUTF8(const char* bytes, int length, int& correctLength)
{
	if (! length) return false;
	unsigned char firstChar = static_cast<unsigned char>(bytes[0]);

	correctLength = 1; //Given the first bits of first char of sequence, how long should
	                       // this UTF-8 char be, in bytes?
	if      ((firstChar & 0b1000'0000) == 0x00) correctLength = 1; //If byte starts with 10, it's ASCII (1 byte)
	else if ((firstChar & 0b1110'0000) == 0xC0)	correctLength = 2; //If byte starts with 1100, it's 2-byte UTF-8. 
	                                                               //The 3rd 1 shown here ensures firstChar's 3rd byte is 0
	else if ((firstChar & 0b1111'0000) == 0xE0)	correctLength = 3; //If byte starts with 1110, it's 3-byte
	else if ((firstChar & 0b1111'1000) == 0xF0)	correctLength = 4; //If byte starts with 11110, it's 4-byte
	else throw std::runtime_error("Invalid UTF-8 lead byte");

	return length >= correctLength;
}


//If we're using variable-width font, how many pixels to backspace is a guess
//Could fix this but a) we barely use this and b) it would require remembering what was typed. So: no
void SSDL_StreamIOSystem::eraseCharAtCursor (char32_t c) //should not be called after changing font... odd things'll happen
{
	SDL_Surface* result = SDL_CreateRGBSurface (0, 1, 1, 4, 0, 0, 0, 1);

	//erase that last char by replacing with background color
	SDL_Rect newRect;
	newRect.x = cursorX(); newRect.y = cursorY(); 
	newRect.w = SSDL_Display::Instance().advance(c); 
	newRect.h = SSDL_Display::Instance().fontLineSkip();
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor (SSDL_Display::Instance(), &r, &g, &b, &a); //save current draw 
	SSDL_Color background_ = SSDL_Display::Instance().background ();
	SDL_SetRenderDrawColor (SSDL_Display::Instance(), background_.r, background_.g, background_.b, background_.a); 
	SDL_RenderFillRect     (SSDL_Display::Instance(), &newRect);
	SDL_SetRenderDrawColor (SSDL_Display::Instance(), r, g, b, a); //Restore prev draw color

	SDL_FreeSurface (result);
}

/*
This code is not currently used, but I'll leave it in, in case it turns up useful later.

bool isPrintable(SDL_Scancode scancode, SDL_Keymod mod) 
{
	if (SDL_SCANCODE_A <= scancode && scancode <= SDL_SCANCODE_0) //if it's alphanumeric...
		return true;
	if (SDL_SCANCODE_SPACE <= scancode && scancode <= SDL_SCANCODE_SLASH) //or from a bunch of punctuation keys...
		return true;
	if (isNumLockOn(mod) && isNumLockKey(scancode))
		return true;
	return false;
}

char myToupper(char c) 
{
	if (isalpha(c)) return toupper(c);
	switch (c)
	{
	case '1': return '!';
	case '`': return '~';
	case '2': return '@';
	case '3': return '#';
	case '4': return '$';
	case '5': return '%';
	case '6': return '^';
	case '7': return '&';
	case '8': return '*';
	case '9': return '(';
	case '0': return ')';
	case '-': return '_';
	case '=': return '+';
	case '[': return '{';
	case ']': return '}';
	case '\\': return '|';
	case ';': return ':';
	case '\'': return '\"';
	case ',': return '<';
	case '.': return '>';
	case '/': return '?';
	default: return c;
	}
}
*/
