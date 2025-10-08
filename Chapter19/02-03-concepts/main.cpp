// Utterly useless program that uses
//   a function template with a concept
//        -- from _C++26 for Lazy Programmers_

#include <concepts>
#include <string>
#include <span>
#include <stdexcept>

template<std::integral T> bool isEven(T n) { return n % 2 == 0; }

template<typename T, typename U>
    requires std::swappable_with<T&, U&>
void mySwap(T& t, U& u) noexcept
{
    T temp = t; t = u; u = temp;
}

template<typename T>
    requires std::copyable<T>&& std::totally_ordered<T>
T minimum(std::span<const T> sp)
{
    if (sp.empty()) throw std::out_of_range("Empty span in minimum");

    T result = sp[0];
    for (const T& e : sp)
        if (e < result)
            result = e;

    return result;
}

class A
{
public:
    A() {};
    A& operator=(const A&) = default;
};

class B
{
public:
    B() {};
    auto operator<=>(const B&) const = default;
    B& operator=(const B&) = delete;
};

int main()
{
    isEven(2);
    // isEven(2.0);  //won't compile -- can't use floats

    int x = 2, y = 3;
    mySwap(x, y);
    //mySwap(2, 3); // won't compile -- can't swap literals

    //A a; B b;
    //mySwap(a,b);  // won't compile -- A and B aren't swappable

    double ds[] = { 1.0, 2.0, 3.0 };
    int    is[] = { 1, 2, 3 };
    std::string ss[] = { "one", "two", "three" };
    A      as[] = { A(), A(), A() };
    B      bs[] = { B(), B(), B() };

    minimum<double>(ds); minimum<int>(is); minimum<std::string>(ss);
    //minimum<A>(as); //won't compile -- A doesn't support <
    //minimum<B>(bs); //won't compile -- B doesn't support =

    return 0;
}


