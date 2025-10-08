// What would happen if we call srand every time we call rand? Nothing good!
//        -- from _C++26 for Lazy Programmers_

#include <ctime>    // for time function
#include <cstdlib>  // for srand, rand
#include "SSDL.h"
 
int main (int argc, char** argv)
{
    for (int i = 0; i < 100; ++i)   	// Print a bunch of random #'s
    {
        srand(static_cast<unsigned int> (time(nullptr)));  // WRONG!
        sout << rand() % 10 << ' ';            
    }

    SSDL_WaitKey ();
    return 0;
}

