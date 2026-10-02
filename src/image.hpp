#pragma once

#include <vector>

struct RGB
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    unsigned char getGrayscaleValue() const
    {
        return static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
};

struct RGBA
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;
    unsigned char getGrayscaleValue() const
    {
        return static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
};

struct L
{
    unsigned char l = 0;
    unsigned char getGrayscaleValue() const
    {
        return l;
    }
};

class PngWrapper;
class JpegWrapper;

template <typename T>
class Image
{
    friend class PngWrapper;
    friend class JpegWrapper;

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
