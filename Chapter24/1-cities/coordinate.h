// class Coordinate, latitude and longitude for global positioning
//       -- from _C++26 for Lazy Programmers_

#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
#include <numbers> //for pi

inline long double deg2Radians(long double deg) 
{ 
    return deg * std::numbers::pi / 180;
}

class Coordinate
{
public:
    Coordinate() = default;
    Coordinate(int latdIn, int longtdIn) : latd_(latdIn), longtd_(longtdIn) {}

    Coordinate& operator=(const Coordinate&) = default;

    long double latdInDeg  () const { return latd_;               }
    long double longtdInDeg() const { return longtd_;             }
    long double latdInRad  () const { return deg2Radians(latd_);  }
    long double longtdInRad() const { return deg2Radians(longtd_);}

    void print(std::ostream& out) const 
    {
        out << latd_ << " deg " << longtd_ << " deg";
    }
private:
    long double latd_ = 0.0, longtd_ = 0.0;
};

inline std::ostream& operator<< (std::ostream& out, const Coordinate& foo)
{
    foo.print(out); return out;
}

double distance(const Coordinate& x, const Coordinate& y);
#endif //COORDINATE_H
