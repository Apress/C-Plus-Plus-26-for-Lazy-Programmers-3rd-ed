// Program to make an image monochrome... 
//   get a histogram...
//   ...and do histogram equalization to make it more viewable
//          -- from _C++26 for Lazy Programmers_

#include <ranges>
#include <algorithm> 
#include <numeric> 
#include "SSDL.h"

int average(int, int, int);                // Averages 3 ints -- from Chapter 7
SSDL_Color greyscale(const SSDL_Color& c); // Gets a greyscale color for a given r, g, b -- adapted from Chapter 7

void showHistogram(std::span<int> histogram, int x = 0, int y = 0, int w = 100, int h = 100);
SSDL_ProcessibleImage 
     crop(const SSDL_ProcessibleImage& original, int x = 0, int y = 0, int w = 0, int h = 0);

void histogramEqualize(      SSDL_ProcessibleImage& outImg,     
                   const SSDL_ProcessibleImage& inImg, const std::vector<int>& inHist);

int main(int argc, char** argv)
{
    SSDL_SetWindowSize(800, 450);
    SSDL_RenderClear(WHITE);
    SSDL_SetRenderDrawColor(BLACK);

    //The original
    SSDL_ProcessibleImage original = SSDL_LoadProcessibleImage("media/fuji.bmp");

    //A monochrome version
    SSDL_ProcessibleImage monochrome = original;
    auto newPixelsView = monochrome.getPixels() | std::views::transform(greyscale);
    auto newPixels     = std::ranges::to<std::vector<SSDL_Color>>(newPixelsView);
    monochrome.setPixels(newPixels);

    //A cropped version
    SSDL_ProcessibleImage cropped = crop(original, 0, 47, 130, 40);
    std::vector<int> histogram3(256, 0); //256 elements, each initialized to 0
    std::ranges::for_each(cropped.getPixels(), 
        [&histogram3](const SSDL_Color& c) {++histogram3[average(c.r, c.g, c.b)]; });

    //Cropped, and histogram-equalized
    SSDL_ProcessibleImage histogramEq;
    histogramEqualize(histogramEq, cropped, histogram3);

    //Calculate its histogram
    std::vector<int> histogram4(256,0);
    std::ranges::for_each(histogramEq.getPixels(), 
        [&histogram4](const SSDL_Color& c) {++histogram4[average(c.r, c.g, c.b)]; });

    SSDL_RenderImage(original,     0,  0); 
    SSDL_RenderText ("original",         0, 150); 
    SSDL_RenderImage(monochrome, 200,  0);
    SSDL_RenderText ("monochrome",     200, 150); 
    SSDL_RenderImage(cropped,    400, 47); 
    SSDL_RenderText ("cropped",        400, 150); 
    showHistogram   (histogram3, 405, 175);
    SSDL_RenderImage(histogramEq,600, 47); 
    SSDL_RenderText ("histogram-eq'd", 600, 150); 
    showHistogram   (histogram4, 605, 175);

    SSDL_WaitKey();    // Wait for user to hit any key

    return 0;
}

int average(int a, int b, int c) {    return (a + b + c) / 3; }

SSDL_Color greyscale(const SSDL_Color& c) // Gets a greyscale color for a given r, g, b
{
    int rgbAverage = average(c.r, c.g, c.b);

    SSDL_Color result
        = SSDL_CreateColor(rgbAverage, rgbAverage, rgbAverage);

    return result;
}

//Crops original starting at (imgX, imgY), to a size (w, h)
SSDL_ProcessibleImage crop(const SSDL_ProcessibleImage& original, 
                           int imgX, int imgY, int w, int h)
{
    auto originalPixels = original.getPixels();
    int  originalWidth  = original.width();

    auto newPixelsView = std::views::iota(0, original.size())
        | std::views::filter([imgX, imgY, w, h, originalWidth](int i) 
            {
                int x = i % originalWidth, y = i / originalWidth;
                return (imgX <= x && x < imgX + w &&
                        imgY <= y && y < imgY + h);
            })
        | std::views::transform([originalPixels](int i)
            {
                return originalPixels[i];
            });

    auto newPixels = std::ranges::to<std::vector<SSDL_Color>>(newPixelsView);
    SSDL_ProcessibleImage result;
    result.setPixels(newPixels, w, h);
    return result;
}

void showHistogram(std::span<int> histogram, int x, int y, int w, int h)
{
    int max = std::ranges::max(histogram);
    int height = std::max(1, h / 256);              //height of the rect for a single value
    int allHeight = 256 * height;
    SSDL_RenderDrawRect(x, y - 1, w, allHeight + 2);//make border a little outsize so we can see top and bottom rows
    for (int i = 0; i < 256; ++i)
        SSDL_RenderFillRect(x, y + i * height, static_cast<int>((histogram[i] / double(max)) * w), height);
}


void histogramEqualize(      SSDL_ProcessibleImage& outImg,   
                       const SSDL_ProcessibleImage& inImg, const std::vector<int>& inHist)
{
    outImg = inImg;

    //Do histogram equalization
    int size = outImg.size();
    double sum = 0.0f;
    std::vector<double> intensityMap = std::ranges::to<std::vector<double>>(
        inHist
        | std::views::transform([size](int x) { return double(x) / size; })
        | std::views::transform([&sum](double x) { return sum += x; })
        | std::views::transform([size](double x) { return static_cast<int>(255 * x); })
    );

    auto newColors = std::ranges::to<std::vector<SSDL_Color>>(
        outImg.getPixels()
        | std::views::transform([intensityMap](const SSDL_Color& c)
            {
                int oldIntensity = average(c.r, c.g, c.b);
                if (oldIntensity == 0) oldIntensity = 1; //prevent divide by zero
                double scale = intensityMap[oldIntensity] / oldIntensity;
                SSDL_Color cNew;
                cNew.r = static_cast<int>(std::clamp(c.r * scale, 0.0, 255.0));
                cNew.g = static_cast<int>(std::clamp(c.g * scale, 0.0, 255.0));
                cNew.b = static_cast<int>(std::clamp(c.b * scale, 0.0, 255.0));
                cNew.a = 255;
                return cNew;
            }));

    outImg.setPixels(newColors);
}
