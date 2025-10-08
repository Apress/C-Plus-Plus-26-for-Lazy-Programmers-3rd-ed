// Utterly useless program that uses a function template
//      -- from _C++26 for Lazy Programmers_

#include <iostream>

template<typename T, typename U>
void mySwap (T& arg1, U& arg2) noexcept
{
    T temp = arg2; arg2 = arg1; arg1 = temp;
}

template<typename T>
void repeatPrint(const T& thing, int howMany)
{
    for (int i = 0; i < howMany; ++i)
        std::cout << thing << ' ';
}

int main()
{
    int    i = 10  , j = 20  ;
    double m =  0.5, n =  1.5;
        
    mySwap(i, j);
    mySwap(m, n);
    mySwap(i, n); // You'll get warnings abt loss of data 
                  //   from mixing ints and doubles, but it'll work 

    repeatPrint(5, 7);

    return 0;
}
