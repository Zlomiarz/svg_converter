#pragma once
#include "image.hpp"

template <typename P>
Image<L> convertToGrayscale(const Image<P> &inputImage)
{
    if constexpr (std::is_same_v<P, L>)
    {
        return inputImage; // Already grayscale
    }
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<L> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const P &pixel = inputImage.getPixel(x, y);
            unsigned char grayValue = pixel.getGrayscaleValue();
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

inline Image<RGBA> prepare_transparent_image(const Image<RGB> &inputImage)
{
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<RGBA> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const RGB &pixel = inputImage.getPixel(x, y);
            unsigned char alphaValue = 255 - (pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
            outputImage.setPixel(x, y, RGBA{pixel.r, pixel.g, pixel.b, alphaValue});
        }
    }

    return outputImage;
}

inline Image<RGBA> prepare_transparent_image(const Image<L> &inputImage)
{
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<RGBA> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const L &pixel = inputImage.getPixel(x, y);
            unsigned char alphaValue = 255 - pixel.l;
            outputImage.setPixel(x, y, RGBA{pixel.l, pixel.l, pixel.l, alphaValue});
        }
    }

    return outputImage;
}

template <typename P>
void add_borders(const Image<P> &image)
{
    width = image.getWidth();
    height = image.getHeight();
    P black;
    for (unsigned x = 0; x < width; ++x)
    {
        image.setPixel(x, 0, black);          // Top border
        image.setPixel(x, height - 1, black); // Bottom border
    }
    for (unsigned y = 0; y < height; ++y)
    {
        image.setPixel(0, y, black);         // Left border
        image.setPixel(width - 1, y, black); // Right border
    }
}
