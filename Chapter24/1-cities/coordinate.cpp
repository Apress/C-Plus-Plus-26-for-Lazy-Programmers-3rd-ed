// Function to get distance between two Coordinates
//       -- from _C++26 for Lazy Programmers_

#include <cmath>
#include "coordinate.h"

// Function to square things should've been in cmath in my opinion
template <typename T> T sqr(const T& t) { return t * t; }

// Uses the "Haversine" formula to get distances between 2 Coordinates
// Trust me, it works
double distance(const Coordinate& x, const Coordinate& y)
{
    constexpr double EARTH_RADIUS = 6371; //km

    double a = sqr(sin((x.latdInRad() - y.latdInRad()) / 2.0))
                   + cos(x.latdInRad()) * cos(y.latdInRad()) *
                         sqr(sin((x.longtdInRad() - y.longtdInRad()) / 2.0));

    return EARTH_RADIUS * 2 * asin(sqrt(a));
}
