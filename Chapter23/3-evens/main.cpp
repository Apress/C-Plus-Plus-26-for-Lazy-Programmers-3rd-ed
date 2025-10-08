//Program to do some processing of number sequences
//    -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <ranges>
#include <list>
#include <algorithm>

bool isEven(int x) { return !(x % 2); }
int square(int x) { return x * x; }

int main()
{
    std::list<int> nums = { 1,2,3,4,5,6,7,8,9 };

    std::list<int>::const_iterator 
        where1 = std::find_if(nums.begin(), nums.end(), isEven);
    if (where1 != nums.end())
        std::cout << "Found an even: " << *where1 << '\n';

    auto where2 = std::ranges::find_if(nums, isEven);
    if (where2 != nums.end()) std::cout << "Found an even: " << *where2 << '\n';

    std::ranges::filter_view<std::ranges::ref_view<std::list<int>>, bool (*)(int)>
        evens1 = std::views::filter(nums, isEven);
    std::cout << "Evens: ";
    for (auto i : evens1) std::cout << i << ' ';
    std::cout << '\n';

    auto evens2 = nums | std::views::filter(isEven);
    std::cout << "Evens: ";
    for (auto i : evens2) std::cout << i << ' ';
    std::cout << '\n';

    auto evenSquares1 = std::views::take(std::views::transform(std::views::filter(nums, isEven), square), 3);
    std::cout << "The first 3 squares of evens are: ";
    for (auto i : evenSquares1) std::cout << i << ' ';
    std::cout << '\n';

    auto evenSquares2 = nums | std::views::filter(isEven) | std::views::transform(square) | std::views::take(3);
    std::cout << "The first 3 squares of evens are: ";
    for (auto i : evenSquares2) std::cout << i << ' ';
    std::cout << '\n';

    std::erase_if(nums, isEven);

    std::cout << "The odds are: ";
    for (auto i : nums) std::cout << i << ' ';
    std::cout << '\n';
}


