// Program to print several factorials using a coroutine
//      -- from _C++26 for Lazy Programmers_

#include <iostream>
#ifdef _MSC_VER
#include <experimental/generator>
#else
#include <generator>
#endif

#ifdef _MSC_VER
std::experimental::generator<int> factorial()
#else
std::generator<int> factorial()
#endif
{
    int whichOne = 0;                 // start with 0!
    int result = 1;                   // 0! is 1

    while (true)
    {
        co_yield result;
        ++whichOne;                   // go on to next one
        result *= whichOne;           // and calculate next result
    }
}

int main()
{
    std::cout << "The first 8 factorials:\n";

    for (int i : factorial())
    {
        static int which = 0;

        std::cout << which << ": " << i << '\n';

        ++which;                  // go on to next
        if (which > 8) break;     // stop at 8
    }

    std::cout << '\n';

    return 0;
}
