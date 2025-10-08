// Program to read in and print back out numbers 
//               -- from _C++26 for Lazy Programmers_

#include <span>
#include "SSDL.h"

constexpr double KELVIN_FREEZING_POINT = 273.0; //close enough
constexpr size_t MAX_NUMS = 10;

void convertToKelvin1(double temps[], size_t arraySize); //uses a parameter for array size
void convertToKelvin2(std::span<double> temps);          //uses a span
void convertToKelvin3(std::span<double> temps);          //uses a span and range-based for

double lowestTemp(const double temps[MAX_NUMS]);          //returns lowest entry in temperatures
double minimum(std::span<const double> elements);         //returns lowest entry in elements

int main (int argc, char** argv)
{
    double temperatures[] = { 32.6, 32.6, 32.7, 32.7, 32.7,
                              32.7, 32.7, 32.7, 32.7, 32.7 };

    //convertToKelvin1(temperatures, MAX_NUMS);
    //convertToKelvin2(temperatures);
    convertToKelvin3(temperatures);

    sout << "In Kelvin, the temperatures are ";
    for (int i = 0; i < MAX_NUMS; ++i)
        sout << temperatures[i] << ' ';
    sout << '\n';

    sout << "Min temp is " << minimum(temperatures) << ".\n";

    sout << "\nHit any key to end.\n";

    SSDL_WaitKey ();

    return 0;
} 

void convertToKelvin1(double temps[], size_t arraySize)
{
    for (int i = 0; i < arraySize; ++i)
        temps[i] += KELVIN_FREEZING_POINT;
}

void convertToKelvin2(std::span<double> temps)
{
    for (int i = 0; i < temps.size(); ++i)
        temps[i] += KELVIN_FREEZING_POINT;
}

void convertToKelvin3(std::span<double> temps)
{
    for (double& element : temps)
        element += KELVIN_FREEZING_POINT;
}

double lowestTemp(const double temps[MAX_NUMS])
// returns lowest entry in temperatures
{
    double result = temps[0];

    for (int i = 0; i < MAX_NUMS; ++i)
        if (temps[i] < result)
            result = temps[i];

    return result;
}

double minimum (std::span<const double> myArray)
// returns lowest entry in elements
// I’m renaming it "minimum" because it’ll be generally useful 
//    -- we don’t have to use it only on temperatures
{
    double result = myArray[0];

    for (double element : myArray)
        if (element < result)
            result = element;

    return result;
}


