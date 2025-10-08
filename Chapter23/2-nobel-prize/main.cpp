//Program to find out which Simpson is mostly likely to get the Nobel
//        -- from _C++26 for Lazy Programmers_

#include <string>
#include <tuple>
#include <iostream>

auto smartestSimpson()
{
    static std::tuple<std::string, int, int> Simpsons[] =
    {
        {"Homer",39,92}, {"Marge",36,105}, {"Bart",10,95}, {"Lisa",8,135}, 
        {"Maggie",1,100}
    };

    auto smartest = std::make_tuple(std::string(), 0, 0); 

    for (const auto& s : Simpsons)
    {
        if (std::get<2>(smartest) < std::get<2>(s))
            smartest = s;
    }

    return smartest;
}

int main()
{
    auto [name, _, __] = smartestSimpson(); 
                    //2 _'s because at present most compilers aren't C++26 compliant on this

    std::cout << "The Simpson most likely to win the Nobel is " << name << ".\n";

    return 0;
}


