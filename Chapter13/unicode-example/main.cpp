// Hello, world! program
//       -- from _C++26 for Lazy Programmers_
// It prints a UTF-8 string. 

#include <iostream> 
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

int main ( )
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); 
#endif

    cout << reinterpret_cast<const char*>(u8"Ελλάδα 中国 ኢትዮጵያ\n");

    return 0;
}
