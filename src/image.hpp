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

class PngWrapper;

template <typename T>
class Image
{
    friend class PngWrapper;

public:
    Image(unsigned _width, unsigned _height) : width(_width), height(_height), pixels(_width * _height) {}

    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }

    const T &getPixel(unsigned x, unsigned y) const { return pixels[y * width + x]; }
    void setPixel(unsigned x, unsigned y, const T &pixel) { pixels[y * width + x] = pixel; }

protected:
    T *getDataPointer() { return pixels.data(); }

private:
    unsigned width;
    unsigned height;
    std::vector<T> pixels;
};
