// Test List::iterator and some other List functions
//    -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <cassert>
#include <string>
#include "list.h"

using namespace std;

int main ()
{
    // Test begin, end, and iterator's ctor, !=, ++, *, -> 
    List<string>S1; S1.push_front("phrase"); S1.push_front("word"); S1.push_front("three");

    cout << "These words are capitalized: ";
    for (List<string>::iterator i = S1.begin(); i != S1.end(); ++i)
    {
        (*i)[0] = toupper((*i)[0]);     // Verifies that you can change *i
        cout << string(*i) << ' ';
    }
    cout << '\n';

    // Let's compute the average length and verify it's OK
    unsigned int sum1 = 0;
    for (List<string>::iterator i = S1.begin(); i != S1.end(); ++i)
        sum1 += (unsigned int) i->size();
    assert(sum1 / S1.size() == 5);

    cout << "If that's right and there were no crashes, "
         << "looks like the list is working.\n";

    return 0;
}
