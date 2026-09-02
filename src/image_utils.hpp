#pragma once
#include "image.hpp"

template <typename P>
Image<L> convertToGrayscale(const Image<P> &inputImage)
{
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<L> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const P &pixel = inputImage.getPixel(x, y);
            unsigned char grayValue = static_cast<unsigned char>(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
            outputImage.setPixel(x, y, L{grayValue});
        }
    }

    return outputImage;
}

inline void binarizeImage(Image<L> &image, unsigned char threshold)
{
    int width = image.getWidth();
    int height = image.getHeight();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const L &pixel = image.getPixel(x, y);
            unsigned char binaryValue = (pixel.l >= threshold) ? 255 : 0;
            image.setPixel(x, y, L{binaryValue});
        }
    }
}

inline void invertImage(Image<L> &image)
{
    int width = image.getWidth();
    int height = image.getHeight();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const L &pixel = image.getPixel(x, y);
            unsigned char invertedValue = 255 - pixel.l;
            image.setPixel(x, y, L{invertedValue});
        }
    }
}

inline Image<RGBA> prepare_transparent_image(const Image<RGBA> &inputImage)
{
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<RGBA> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const RGBA &pixel = inputImage.getPixel(x, y);
            unsigned char alphaValue = 255 - (pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
            outputImage.setPixel(x, y, RGBA{pixel.r, pixel.g, pixel.b, alphaValue});
        }
    }

    return outputImage;
}
