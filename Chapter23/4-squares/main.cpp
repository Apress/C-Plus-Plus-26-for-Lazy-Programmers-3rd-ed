//Program to generate some squares
//    -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <ranges>
#include <vector>

int square(int x) { return x * x; }

int main()
{
    auto lotsaIntegers     = std::views::iota(0, 1000000);
    auto firstFiftySquares = lotsaIntegers | std::views::transform(square) | std::views::take(50);
    std::vector<int> v     = firstFiftySquares | std::ranges::to<std::vector>();
    for (int i : firstFiftySquares) std::cout << i << ' ';
    std::cout << '\n';

    auto allPositiveIntegers    = std::views::iota(0); //from 0 to ∞! 
    auto firstFiftySquaresAgain = allPositiveIntegers | std::views::transform(square) | std::views::take(50);
    std::vector<int> vAgain     = firstFiftySquaresAgain | std::ranges::to<std::vector>();
    for (int i : vAgain) std::cout << i << ' ';
    std::cout << '\n';

    return 0;
}


