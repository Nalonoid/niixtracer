#include <iostream>
#include <vector>

#include "image.hpp"

Image::Image(unsigned width, unsigned height) :
    _width(width), _height(height)
{
    /* Initialize an image of width*height pixels.
     * Each pixel of the image is initialized to (192, 0, 255) */
    _pixels.resize(width, std::vector<Pixel>(height, Pixel(192, 0, 255)));
}

unsigned Image::height() const
{
    return _height;
}

unsigned Image::width() const
{
    return _width;
}

const std::vector<std::vector<Pixel>>& Image::pixels() const
{
    return _pixels;
}

/* This second getter is useful to allow these statements :
 * img.pixels() = img2.pixels();
 * img.pixels() = std::vector<std::vector<Pixel>>
                    (10, std::vector<Pixel>(5, Colors::BLUE)); */
std::vector<std::vector<Pixel>>& Image::pixels()
{
    return _pixels;
}

std::vector<Pixel>& Image::operator[](int index)
{
    return _pixels[index];
}

const std::vector<Pixel>& Image::operator[](int index) const
{
    return _pixels[index];
}

void Image::paint(Color c)
{
    /* Equivalent to a clear but reallocates the memory to ensure that capacity
     * equals zero and that old pixels are no longer accessible */
    std::vector<std::vector<Pixel>>
            (_width, std::vector<Pixel>(_height, c)).swap(_pixels);
}
