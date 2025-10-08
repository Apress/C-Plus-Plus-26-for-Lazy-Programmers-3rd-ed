// Program that uses string_view
//        -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <string>
#include <string_view>

bool hasDoubleLetters(std::string_view str);

int main()
{
    const std::string word = "bookkeeper";

    if (! hasDoubleLetters(word))
        std::cout << "hasDoubleLetters isn't working!\n";
    if (! hasDoubleLetters("bookkeeper"))
        std::cout << "hasDoubleLetters isn't working!\n";
    return 0;
}

bool hasDoubleLetters(std::string_view str)
{
    for (int i = 0; i < str.size()-1; ++i)
        if (str[i] == str[i+1]) // Two in a row! Return true
            return true; 

    return false;               // No doubles found
}
