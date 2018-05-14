#include <iostream>
#include <vector>

#include "image.hpp"

Image::Image() {}

Image::Image(unsigned width, unsigned height) :
    _width(width), _height(height)
{
    init();
}

unsigned Image::height() const
{
    return _height;
}

unsigned Image::width() const
{
    return _width;
}

unsigned& Image::height()
{
    return _height;
}

unsigned& Image::width()
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

double Image::aspect_ratio() const
{
    return (double)_width/(double)_height;
}

std::vector<Pixel>& Image::operator[](int index)
{
    return _pixels[index];
}

const std::vector<Pixel>& Image::operator[](int index) const
{
    return _pixels[index];
}

void Image::init()
{
    /* Initialize an image of width*height pixels.
     * Each pixel of the image is initialized to (192, 0, 255) */
    _pixels.resize(_width, std::vector<Pixel>(_height, Pixel(192, 0, 255)));
}

void Image::paint(Color c)
{
    /* Equivalent to a clear but reallocates the memory to ensure that capacity
     * equals zero and that old pixels are no longer accessible */
    std::vector<std::vector<Pixel>>
            (_width, std::vector<Pixel>(_height, c)).swap(_pixels);
}
