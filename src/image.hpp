#pragma once

#include <vector>

struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct RGBA
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct L
{
    unsigned char l;
};

template <typename T>
class Image
{
public:
    Image(unsigned width, unsigned height) : width(width), height(height), pixels(width * height) {}

    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }

    const T &getPixel(unsigned x, unsigned y) const { return pixels[y * width + x]; }
    void setPixel(unsigned x, unsigned y, const T &pixel) { pixels[y * width + x] = pixel; }

private:
    unsigned width;
    unsigned height;
    std::vector<T> pixels;
};
