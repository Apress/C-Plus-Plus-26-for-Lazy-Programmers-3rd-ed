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

//This is SSDL_exception.cpp. It contains SSDL_terminate, which ends the program if something's wrong

#include <SDL.h>
#include "SSDL_exception.h"

//Note about VS:
// When using the debugger, it doesn't go to SSDL_Terminate but just breaks the program at the exception
// Continue just assumes the exception is resolved
// You can make it run without debugging with Ctrl-F5 and it will then call the terminate function as appropriate

void SSDL_terminate () 
{
	auto ce = std::current_exception();
	if (ce)                                     //if an exception is being thrown
	{
		try
		{
			std::rethrow_exception(ce);
		}
		catch (const SSDL_Exception& e)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SSDL Error", e.what(), nullptr);
		}
	}

	std::abort ();
}
