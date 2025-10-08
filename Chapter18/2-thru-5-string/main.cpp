// Driver program to test move constructor and move =
//      -- from _C++26 for Lazy Programmers_

#include <iostream>
#include "string.h"

using namespace std;

int main()
{
    // Test at
    String xyz = "xyz";
    try
    {
        xyz.at(3) = 'a'; //no, that's out of range...
        cout << "This line will never be reached.\n";
    }
    catch (OutOfRangeException&)
    {
        cout << "Out of range error caught!\n";
    }

    String str1 = "after", str2 = "noon";
    String newString;
    newString = str1 + str2;
        // To test that this is really working,
        //   put a breakpoint in String::copy and see if
        //   it gets called on return from +, or by =
        //   (it shouldn't)

    return 0;
}

