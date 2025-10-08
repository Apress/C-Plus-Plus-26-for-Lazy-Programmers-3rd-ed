// Driver program to test the String class
//        -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <cassert>  // for assert, a function which crashes 
                    //   if the condition you give is false
                    // used for debugging
#include "string.h"

using namespace std;

int main()
{
                    // using consts to ensure const functions are right
    const String EMPTY;   
    const String ABC("abc");

    // Testing default ctor, conversion ctor from char*, ==, !=, !
    assert(EMPTY == ""); assert(! EMPTY); assert(! (EMPTY != ""));
    assert(ABC != "");                     assert(! (ABC == ""));
    assert(String(nullptr) == EMPTY);

    // Testing c_str, size ...
    assert(strcmp(ABC.c_str(), "abc") == 0);
    assert(ABC.size() == 3);

    // Test >, >=, <, <=, !=,  
    // We're doing lots of automatic calls to conversion ctor
    //   from const char*, so that's tested too
    assert(ABC <  "abd");  assert(! (ABC >= "abd"));  
    assert(ABC <= "abd");  assert(! (ABC >  "abd"));
    assert(ABC >  "abb");  assert(! (ABC <= "abb"));
    assert(ABC >= "abb");  assert(! (ABC <  "abb"));
    assert(ABC <= ABC);    assert(ABC >= ABC);
    
    // Test []
    String xyz = "xyz";
    assert(xyz[1] == 'y'); xyz[1] = 'Y';
    assert(xyz[1] == 'Y'); xyz[1] = 'y';
    assert(ABC[1] == 'b'); //const version

    // Test =, ()
    xyz = "xYz"; assert(xyz == "xYz");

    // Test copy ctor
    assert(ABC == String(ABC));

    // Test + (and thereby +=) 
    String ABCDEF = ABC+"def";
    assert(ABCDEF == "abcdef");

    // Test << and >>
    String input;
    cout << "Enter a string:\t"; cin >> input;
    cout << "You entered:\t" << input << '\n';

    cout << "If no errors were reported, "
         << "class String seems to be working!\n";

    return 0;
}

