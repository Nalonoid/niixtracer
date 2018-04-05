#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <vector>

#include "color.hpp"

struct Pixel
{
    Color color;

    Pixel(Color c) :
        color(c) {}

    Pixel(int red, int green, int blue) :
        color(Color(red, green, blue)) {}

    Pixel() :
        color(Colors::BLACK) {}
};

class Image
{
public:
    Image(int width, int height);

    // Getters
    int height() const;
    int width() const;
    const std::vector<std::vector<Pixel>>& pixels() const;
    std::vector<std::vector<Pixel>>& pixels();
    double aspect_ratio() const;

    // Operators
    std::vector<Pixel>& operator[](int index);
    const std::vector<Pixel>& operator[](int index) const;

    // Functions
    void paint(Color c);

private:
    int _width;
    int _height;
    std::vector<std::vector<Pixel>> _pixels;
};

#endif
