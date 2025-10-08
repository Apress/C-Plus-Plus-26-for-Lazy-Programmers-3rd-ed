//  Program to find max of a variable-length parameter list
//        -- from _C++26 for Lazy Programmers_

#include <cassert>

using namespace std;

template<typename T>
double myMax(T value) { return value;  }

template<typename T, typename... OtherArgs>
double myMax(T first, OtherArgs... otherArgs)
{
    double otherArgsMax = myMax(otherArgs...);
    if (first < otherArgsMax) return otherArgsMax; else return first;
}

int main()
{
    assert(myMax(32.0, 100, 0, 56.0) == 100.0);

    return 0;
}
