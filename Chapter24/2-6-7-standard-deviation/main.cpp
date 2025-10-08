// Program that uses calculates standard deviation
//       -- from _C++26 for Lazy Programmers_

#include <iostream>
#include <vector>
#include <ranges> 
#include <algorithm> //for fold_left
#include <expected>
#include <fstream>
#include <cmath>      //for sqrt

double sqr(double x) { return x * x; }

std::expected<std::ifstream, std::string> openFile(const char* filename);
std::expected<std::vector<double>, std::string> loadFile(std::ifstream in);

int main()
{     
    auto numbers = openFile("input.txt").and_then(loadFile);
    if (!numbers)
        std::cout << numbers.error() << '\n';
    else
    {
        std::vector<double>& ns = numbers.value();
        double sum = std::ranges::fold_left(ns, 0.0, std::plus{});
        double avg = sum / ns.size();

        double innerSummation =
            std::ranges::fold_left
            (numbers.value()
                | std::views::transform([avg](double x) {return sqr(x - avg); }),
                0.0,
                std::plus{});

        double standardDeviation = sqrt(innerSummation / numbers.value().size());

        std::cout << "Standard deviation should be 2 and is " << standardDeviation << ".\n";
    }

    return 0;
}

std::expected<std::ifstream, std::string> openFile(const char* filename)
{
    std::ifstream infile(filename);
    if (!infile) return std::unexpected("Can't open file");
    else return std::move(infile);
}

std::expected<std::vector<double>, std::string> loadFile(std::ifstream in)
{
    std::vector<double> result;
    while (in)
    {
        int x=0; in >> x;
        if (!in) break; //reached end of file; discard failed read
        result.push_back(x);
    }
    if (result.empty()) return std::unexpected("Empty file");

    return result;
}