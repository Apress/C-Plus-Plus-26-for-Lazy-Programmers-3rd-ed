// Program to get length of a UTF-8 string
//       -- from _C++26 for Lazy Programmers_

#include <iostream> 
#include <cstring>
#include <cassert>
#ifdef _WIN32
#include <windows.h>
#endif

//How many bytes are in the next UTF-8 character in str?
int lengthOfUTF8Char(const char* str);
int strlenUTF8    (const char* str);

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); 
#endif

    const char* str = reinterpret_cast<const char*>(u8"α א 字");
    std::cout << "strlen thinks the length of \"" << str
              << "\" is " << strlen(str)
              << " but it's really " << strlenUTF8(str) << ".\n";

    return 0;
}

int strlenUTF8(const char* str)
{
    int result = 0;
    while (*str)
    {
        ++result; //we'll add 1 for each UTF-8 char
        str += lengthOfUTF8Char(str);
                  //but move str ahead by each UTF-8 char's width
    }
    return result;
}

int lengthOfUTF8Char(const char* str)
{
    if ((*str & 0b10000000) == 0b00000000) return 1; //If leftmost bit is 0,
    //   it's ASCII: 1 byte
    if ((*str & 0b11100000) == 0b11000000) return 2; //2 bytes long
    if ((*str & 0b11110000) == 0b11100000) return 3; //3 bytes
    if ((*str & 0b11111000) == 0b11110000) return 4; //4 bytes
    return 1;                         //Not recognized! Just treat it as ASCII
}
