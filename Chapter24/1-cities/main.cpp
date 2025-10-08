// Program that uses lambda functions to process cities by different criteria
//       -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <vector>
#include <ranges>       
#include "coordinate.h" 

//A City has a name, a population, and a location (latitude & longitude)
class City
{
public:
    City(const std::string& n, int p, const Coordinate& loc) :
        name_ (n), pop_ (p), location_ (loc)
    {
    }
    City            (const City&) = default;
    City& operator= (const City&) = default;
    const std::string& name    () const { return name_;       }
    int   pop                  () const { return pop_;        }
    const Coordinate&  location() const { return location_;  }
    void print(std::ostream& out) const { out << name_ << " (pop " << pop() << "): " << location_; }
private:
    std::string  name_;
    int          pop_;
    Coordinate   location_;
};

inline
std::ostream& operator<< (std::ostream& out, const City& foo)
{
    foo.print(out); return out;
}

inline 
double distance(const City& xCity, const City& yCity)
{
    return distance(xCity.location(), yCity.location());
}

int main()
{     
    // Some prominent party spots
    std::vector<City> cities =
    {
        //city name,       population, location
        {"London",         10'310'000, { 51,   -5}},
        {"Hamburg",         1'740'000, { 53,   10}},
        {"Paris",          10'840'000, { 49,    2}},
        {"Rome",            3'720'000, { 42,   12}},
        {"Rio de Janiero", 12'900'000, {-22,  -43}},
        {"Hong Kong",       7'314'000, { 20,  114}},
        {"Tokyo",          38'000'000, { 36,  140}},
        {"LA",              3'800'000, { 34, -118}},
        {"New York",        8'300'000, { 41,  -74}},
        {"Amsterdam",         921'000, { 52,    5}},
        {"Monte Carlo",        15'000, { 44,    7}},
        {"Shard End",          12'000, { 53,   -2}}
    };

    std::cout << "Cities with more than 10 million:\n";
    auto bigCities = cities | std::views::filter([](const City& c) { return c.pop() > 10'000'000; });
    for (const City& c: bigCities) std::cout << c << '\n';
    std::cout << "\n\n";

    const City SHARD_END = cities[11];
    std::cout << "Cities within 1000 km of Shard End:\n";
    auto nearCities = cities
                    | std::views::filter([&SHARD_END](const City& c)
                        { 
                            return distance(c, SHARD_END) < 1000;
                        });
    for (const City& c: nearCities) std::cout << c << '\n';
    std::cout << "\n\n";

    return 0;
}


