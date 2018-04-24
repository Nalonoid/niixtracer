#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <vector>

#include "color.hpp"

struct Pixel
{
    Color color;

    Pixel(Color c) :
        color(c) {}

    Pixel(double red, double green, double blue) :
        color(Color(red, green, blue)) {}

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
    unsigned height() const;
    unsigned width()  const;

    unsigned& height();
    unsigned& width();

    const std::vector<std::vector<Pixel>>& pixels() const;
    std::vector<std::vector<Pixel>>& pixels();

    double aspect_ratio() const;

    // Operators
    std::vector<Pixel>& operator[](int index);
    const std::vector<Pixel>& operator[](int index) const;

    // Functions
    void paint(Color c);

private:
    unsigned _width;
    unsigned _height;
    std::vector<std::vector<Pixel>> _pixels;
};

#endif
