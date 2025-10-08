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

#ifndef SSDL_STREAM_H
#define SSDL_STREAM_H

#include <iostream>
#include <cassert>
#include <cstring> //for memmove
#include <expected>
#include "SSDL_font.h"
#include "SSDL_io.h"
#include "SSDL_image.h"

//SDL has a cursor, but that's for mouse.
//I need one for my stream IO.
//I might make it as a square, based on font size, with a line at the bottom.
//It could be drawn to cursorX, cursorY location
//Backspace should move it back, leaving behind an empty box (use SSDL_Display background color)
//When we put text there, make sure to erase it!

//scrolling will be triggered when cursorY() gets to within fontLineSkip*2 pixels of the bottom of the screen.
//It will capture the screen, 0 to maxX, fontLineSkip to maxY;
// paste it onto the screen at position 0, 0
// past an empty rectangle of SSDL_Display's background color at position 0, maxY-fontLineSkip

//I'm pretty sure this is all do-able


class SSDL_StreamIOSystem
{
public:
	static SSDL_StreamIOSystem& Instance ()
	{
		static SSDL_StreamIOSystem myStreamIOSystem;
		return myStreamIOSystem;
	}
	SSDL_StreamIOSystem(const SSDL_StreamIOSystem&) = delete;
	const SSDL_StreamIOSystem& operator= (const SSDL_StreamIOSystem&) = delete;

	TTF_Font* currentFont () const { return SSDL_GetCurrentFont (); }

	int  cursorX() const { return cursorX_; }
	int  cursorY() const { return cursorY_; }
	int& cursorX()       { return cursorX_; }
	int& cursorY()	     { return cursorY_; }
	void setCursor (int x, int y) { cursorX_ = x; cursorY_ = y; }
	void eraseCharAtCursor (char32_t c); //should not be called after changing font... odd things'll happen
private:
	SSDL_StreamIOSystem () : cursorX_(0), cursorY_(0) { }

	int cursorX_, cursorY_;
};

/////////////////  SSDL_BufferOut //////////////////////

inline void SSDL_SetCursor (int x, int y) { SSDL_StreamIOSystem::Instance().setCursor (x, y); }
bool isCompleteUTF8(const char* bytes, int length, int& correctLength);

template<class _Elem, class _Traits = std::char_traits <_Elem> >
class SSDL_BufferOut: public std::basic_streambuf<_Elem, _Traits>
{
public:
	typedef _Elem                           char_type;
	typedef _Traits                         traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::off_type off_type;
	typedef typename traits_type::pos_type pos_type;

	SSDL_BufferOut()  { } 
	void printChar (const char* ch, int len); 
	//I could speed things up by using xsputn to print entire char arrays rather than printing char by char with printChar
	// except that printChar advances the cursor. It does this by measuring each character. xsputn would have to do this too
	// so we wouldn't save time -- it would still be O(length of string). So don't bother

protected:
	int_type overflow (int_type c = traits_type::eof ());
};

//Give it p, which is a pointer to a UTF8 character (which can be 1-4 bytes)
//THis is by ChatGPT so completely overhaul it if it works!
std::expected<char32_t, std::string> decode_utf8(const char* p, int len);

template <class charType, class TraitsType>
void SSDL_BufferOut<charType, TraitsType>::printChar (const char* ch, int len)
{
	//fontLineSkip and advance vary except in fixed-width font
	//This may blow away my hope of saving time by sending entire strings -- or not.  Not sure
	// it'll be O(N) to find the advance, but I don't know how long it takes to render a string
	auto whatIsThis = decode_utf8(ch, len);
	assert(whatIsThis.has_value());

	int fontLineSkip = SSDL_Display::Instance().fontLineSkip();
	int myAdvance    = SSDL_Display::Instance().advance(whatIsThis.value());  
	int debugX = SSDL_StreamIOSystem::Instance().cursorX();
	int debugY = SSDL_StreamIOSystem::Instance().cursorY();
	switch (*ch)
	{
	case '\n':
	case '\r':
		SSDL_StreamIOSystem::Instance().cursorY() += fontLineSkip;
		SSDL_StreamIOSystem::Instance().cursorX() =   0;
		break;
	case '\t':
		SSDL_StreamIOSystem::Instance().cursorX() = (((SSDL_StreamIOSystem::Instance().cursorX()/myAdvance)/8)+1)*8*myAdvance; //8 spaces per tab, by convention
		break; 
	default:
		//SSDL_RenderDrawRect(debugX, debugY, myAdvance, fontLineSkip);
		SSDL_Display::Instance().RenderText(ch, SSDL_StreamIOSystem::Instance().cursorX(), SSDL_StreamIOSystem::Instance().cursorY(), SSDL_GetCurrentFont(), false);
		SSDL_StreamIOSystem::Instance().cursorX() += myAdvance;
	}
}

template <class _ELem, class _Traits>
typename SSDL_BufferOut<_ELem, _Traits>::int_type
SSDL_BufferOut<_ELem, _Traits>::overflow(typename SSDL_BufferOut<_ELem, _Traits>::int_type ch) 
{
	static char bytes[5] = { 0 };
	static int which = 0;

	if (ch == EOF) return ch;

	if (which == 3) throw SSDL_Exception("You can't have more than 4 bytes in a UTF8 character!");
	bytes[which++] = (char)ch;

	int correctLength;
	if (isCompleteUTF8(bytes, which, correctLength)) 
	{
		printChar(bytes, correctLength);
		which = 0; memset(bytes, 0, 5);
	}

	return ch;
}

/////////////////// SSDL_BufferIn //////////////////////

template <class charT, class traits = std::char_traits<charT> >
class SSDL_BufferIn: public std::basic_streambuf<charT, traits>
{
public:
	inline static constexpr int BUFFER_SIZE = 256;

	typedef charT                          char_type;
	typedef traits                         traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::off_type off_type;
	typedef typename traits_type::pos_type pos_type;

	SSDL_BufferIn ()
	{
		this->setg (buffer_, buffer_, buffer_);
		//This tells C++ where the read buffer starts, where it ends, and where the next char to read is
		// We offset it so there is an area to prepend things?  Don't know yet
	}
	protected:
	int numCharsInBuffer () const { return int(this->egptr()-this->gptr()); }//it's the end position minus the next-char position
	char buffer_[BUFFER_SIZE]; //OK, but what if it grows too high? //should it be an int array?  setg says no

	int_type underflow ();
	int_type pbackfail (int_type c);
private:
	int_type  getMoreChars ();
	void egbump (int n)           //I wonder why this isn't built in?  moves end ptr by n units.
	{
		this->setg (this->eback(), this->gptr(), this->egptr()+n);
	}

	void handleBackspace(bool& charRemoved );	            //used by getMoreChars ()
	void addACharacter  (char c, bool& quit);				//used by getMoreChars ()
};

//As I understand it, underflow is here for when the istream class wants another character, it can get it
// from the source I provide (the buffer, if there is anything in it; if not, from the device that gives us chars)
template <class charType, class TraitsType>
typename SSDL_BufferIn<charType, TraitsType>::int_type SSDL_BufferIn<charType, TraitsType>::underflow ()
{
	if (numCharsInBuffer() > 0)						    //If we have a char in the buffer to provide...
		return TraitsType::to_int_type (*(this->gptr()));	//  ... return it!  Job complete
	else if (getMoreChars ()>0)						    //Can we get one from the device?
		return TraitsType::to_int_type (*(this->gptr()));	//If so, we did it; return the next char available
	else											        //NO way to get a character!  Oh, no!  Return failure
		return TraitsType::eof ();
}

template <class charType, class TraitsType>
void SSDL_BufferIn<charType, TraitsType>::handleBackspace(bool& charRemoved)	//process a backspace in my input
{
	charRemoved = false;

	if (this->egptr() != this->eback() && SSDL_StreamIOSystem::Instance().cursorX() > 0) //last condition:  can we be assured it's right?  Cd we be
	//off by a few pixels?
	{
		char prevChar = *((this->egptr()) - 1);
		switch (prevChar)
		{
		case '\n':
		case '\r':
		case -1: //eof
			break;
		default:
			SSDL_StreamIOSystem::Instance().cursorX() -= SSDL_Display::Instance().advance(prevChar);

			SSDL_StreamIOSystem::Instance().eraseCharAtCursor(prevChar); //erase the char backspaced over

			assert(SSDL_StreamIOSystem::Instance().cursorX() >= 0); //we shd've had at least 1 char on line, plus my new blank space!
			egbump(-1); //move gptr back 1 space, so we're ignoring where we were
			charRemoved = true;

			break;
		}
	}
}

/////////////////// Declare ssin and sout //////////////////////

//Why class SSDL_ostrream? Because C++26 apparently still can't print const char8_t* with std::ostream
// This is a Bad Thing, but there it is
// Although I want sout to be consistent with std::cout, 
// I can't expect beginners to type sout << reinterpret_cast<const char*> u8"thing"
// ...in Chapter 1 (!)
// ...before we've introduced casting much less reinterpret_cast
// So I'll make sout friendlier. It works!

class SSDL_ostream : public std::ostream
{
public:
	using std::ostream::basic_ostream;
};

inline
SSDL_ostream& operator<< (SSDL_ostream& out, const char8_t* foo)
{
	out << reinterpret_cast<const char*> (foo); return out;
}

extern SSDL_ostream sout;
extern std::istream ssin;

inline bool isNumLockOn (SDL_Keymod mod) { return (mod & KMOD_NUM) != 0; }

///////////////////// More SSDL_BufferIn ////////////////////////////

//We got a character through ssin... display it and send it where it should go.
template <class charType, class TraitsType>
void  SSDL_BufferIn<charType, TraitsType>::addACharacter(char c, bool& OKWereDone)
{
	if (this->gptr() >= this->eback() + BUFFER_SIZE - 1) //no room!
	{
		OKWereDone = true;	//What will this look like?  Test with tiny buffer size.
		return;
	}

	sout << c;
	*(this->egptr()) = c;
	egbump(1); //move end pointer one step further
}

template <class charType, class TraitsType>
typename SSDL_BufferIn<charType, TraitsType>::int_type  SSDL_BufferIn<charType, TraitsType>::getMoreChars ()
{
	SSDL_RenderPresent();	//We're about to wait for user input... so let's see what's already there

	//As it is, unless you backspace, you _never_ have gptr going back...and it should!  Can I be guaranteed
	// that every time istream calls underflow, it ate all the chars SSDL_BufferIn provided?  Yes!  So I will
	//move everything back.

	assert (this->eback () == buffer_); //if not, why not?
	if (this->gptr() != this->eback())
	memmove (this->eback(), this->gptr(), this->egptr()-this->gptr()); // I think this'll move things back!

	bool OKWereDone = false;
	int  numCharsRead = 0;

	while (! OKWereDone) //Looks a lot like SSDL_WaitEvent...
	{
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0)
		{
			SDL_Keymod   mod		= SDL_GetModState();
			bool	     isNumLock	= isNumLockOn(mod);
			SDL_Scancode scancode;

			//If a quit message came, let's get out.
			SSDL_CheckForQuitMessages(event);
			if (SSDL_Display::Instance().isTimeToQuit())
			{
				OKWereDone = true;  break;
			}

			//Now let's process two kinds of messages.  SDL_KEYDOWN for when we get a backspace or a return
			//SDL_TEXTINPUT for when we get useful, printable text.

			switch (event.type)
			{
			case SDL_KEYDOWN:
				scancode = event.key.keysym.scancode;
				if (scancode == SDL_SCANCODE_BACKSPACE || (!isNumLock && scancode == SDL_SCANCODE_KP_BACKSPACE))
				{
					bool charRemoved;
					handleBackspace(charRemoved);
					if (charRemoved) --numCharsRead;
					SSDL_RenderPresent();	//Let user see what's typed 
				}
				if (scancode == SDL_SCANCODE_RETURN || scancode == SDL_SCANCODE_RETURN2)
				{
					OKWereDone = true;

					addACharacter(event.key.keysym.sym, OKWereDone);
					++numCharsRead;
					SSDL_RenderPresent();	//Let user see what's typed 
				}
				break;
			case SDL_TEXTINPUT:
				addACharacter(event.text.text[0], OKWereDone);
				++numCharsRead;
				SSDL_RenderPresent();	//Let user see what's typed 
			}
		}
	}

	SSDL_RenderPresent();	//Do we need to do it again just to be sure? Wouldn't hurt
	return numCharsRead;
}

template <class charType, class TraitsType>
typename SSDL_BufferIn<charType, TraitsType>::int_type SSDL_BufferIn<charType, TraitsType>::pbackfail (int_type c)
{
	if (this->gptr() == this->eback ()) //if there is no further back we can go -- we're at start of buffer
		return TraitsType::eof (); //then false
	else
	{
		this->gbump(-1); //back up one space
		//and if there's no EOF where we just went, put our char there
		if (! TraitsType::eq_int_type (c, TraitsType::eof ()))
		{
			*(this->gptr()) = TraitsType::to_char_type (c);
			return c;
		}
		else return TraitsType::not_eof(c);
	}
}
#endif //SSDL_STREAM_H
