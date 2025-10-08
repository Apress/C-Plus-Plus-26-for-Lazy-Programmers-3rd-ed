// class Coordinate, latitude and longitude for global positioning
//       -- from _C++26 for Lazy Programmers_

#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
#include <numbers> //for pi
#include <cmath>   //for sqrt, sin, etc.

constexpr inline long double deg2Radians(long double deg) 
{ 
    return deg * std::numbers::pi / 180;
}

class Coordinate
{
public:
    constexpr Coordinate() = default;
    constexpr Coordinate(int latIn, int longIn) : latd_(latIn), longtd_(longIn) {}

    constexpr Coordinate& operator=(const Coordinate&) = default;

    constexpr long double latdInDeg  () const { return latd_;               }
    constexpr long double longtdInDeg() const { return longtd_;             }
    constexpr long double latdInRad  () const { return deg2Radians(latd_);  }
    constexpr long double longtdInRad() const { return deg2Radians(longtd_);}

    void print(std::ostream& out) const //Not constexpr! Can't be: does I/O
    {
        out << latd_ << " deg " << longtd_ << " deg";
    }
private:
    long double latd_ = 0.0, longtd_ = 0.0;
};

                                        //Also not constexpr!
inline std::ostream& operator<< (std::ostream& out, const Coordinate& foo)
{
    foo.print(out); return out;
}

// Function to square things should've been in cmath in my opinion
template <typename T> constexpr T sqr(const T& t) { return t * t; }

// Uses the "Haversine" formula to get distances between 2 Coordinates
// Trust me, it works
#if __cplusplus >= 202302L // If we're doing C++26 or later, so distance 
constexpr 
#endif
double distance(const Coordinate& x, const Coordinate& y)
{
    constexpr double EARTH_RADIUS = 6371; //km

    double a = sqr(sin((x.latdInRad() - y.latdInRad()) / 2.0))
                   + cos(x.latdInRad()) * cos(y.latdInRad()) *
                   sqr(sin((x.longtdInRad() - y.longtdInRad()) / 2.0));

    return EARTH_RADIUS * 2 * asin(sqrt(a));
}

constexpr inline Coordinate GREENWICH = {51, 0};
#endif //COORDINATE_H
