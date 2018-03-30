#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <vector>

#include "color.inl"

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
    Image(unsigned width, unsigned height);

    // Getters
    unsigned height() const;
    unsigned width() const;
    const std::vector<std::vector<Pixel>>& pixels() const;
    std::vector<std::vector<Pixel>>& pixels();

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
