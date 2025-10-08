// Program to print some points with std::format
//       -- from _C++26 for Lazy Programmers_

#include <format>
#include <iostream>
#include <exception>

struct Point2D 
{ 
    int x_ = 0, y_ = 0;
};

bool operator== (const Point2D& a, const Point2D& b)
{
    return a.x_ == b.x_ && a.y_ == b.y_;
}

template <> //a template specialization
struct std::formatter<Point2D> : public std::formatter<std::string>
{
    auto format(const Point2D& point, std::format_context& context) const
    {
        std::string str = std::format("({}, {})", point.x_, point.y_);
        return std::formatter<std::string>::format(str, context);
    }
};

int main(int argc, char** argv)
{
    //Let's take a 2 step trip: {1,1} forward, same amount back
    Point2D step1 = { 1, 1 }, step2 = { -1, -1 };
    std::cout << std::format("Taking two steps: {} and {}.\n", step1, step2);
    
    Point2D whereWeWent = { step1.x_ + step2.x_, step1.y_ + step2.y_ };
    if (whereWeWent != Point2D())
        throw std::out_of_range(
            std::format("How can {} and {} not add to {}?", 
            step1, step2, Point2D()));

    return 0;
}
