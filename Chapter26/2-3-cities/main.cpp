// Program that tests differences between Coordinates
//       -- from _C++26 for Lazy Programmers_

#include <iostream>
#include "coordinate.h" 

int main()
{
    constexpr Coordinate QUITO    = {   0, -78 },
                         LIMA     = { -12, -77 },
                         SANTIAGO = { -33, -70 };

#if __cplusplus >= 202302L // If we're doing C++26 or later, so distance 
                           //    is constexpr and so can be in static_assert...
    static_assert(distance(LIMA, QUITO) < distance(LIMA, SANTIAGO),
                  "Oops! Lima is closer to Quito than to Santiago");
#endif

    std::cout << "The distance from " << LIMA
              << " to " << QUITO << " is "
              << distance(LIMA, QUITO) << " km.\n";

    return 0;
}


