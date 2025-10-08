// Example with a Vector of int
//      -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <cassert>
#include "vector.h"

using namespace std;

//  Used to test move ctor/=
Vector makeVectorToTestMoveCtor(int e) 
{ 
    Vector V; V.push_back(e); return V; 
}

int main()
{
    Vector V;

    for (int i = 1; i < 11; ++i) V.push_back (i);

    cout << "Can you count to 10?  The Count will be so proud!\n";

    try
    {
        for (unsigned int i = 0; i < V.size(); ++i) cout << V[i] << ' ';
        cout << '\n';
    }
    catch (const std::out_of_range& e)
    {
        cout << "This shouldn't happen: " << e.what() << ".\n";
        //what() returns whatever message we gave the constructor
        //   when we created the out_of_range exception
    }

    // Extra code to test other functions, including move functions
    Vector W(V);  assert(W==V);
    Vector Y;     assert(Y!=V); // what if sizes differ?
    V[0] = 10;    assert(W!=V); assert(V[0]==10);
    V = W;        assert(W==V);

    V = makeVectorToTestMoveCtor(5); 
    // test by tracing in debugger, that it really calls it

    return 0;
}

