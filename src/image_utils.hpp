#pragma once
#include "image.hpp"

#include <expected>

template <typename P>
Image<L> convertToGrayscale(const Image<P> &inputImage)
{
    if constexpr (std::is_same_v<P, L>)
    {
        return inputImage; // Already grayscale
    }
    unsigned width = inputImage.getWidth();
    unsigned height = inputImage.getHeight();
    Image<L> outputImage(width, height);

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
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
    unsigned width = image.getWidth();
    unsigned height = image.getHeight();

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            const L &pixel = image.getPixel(x, y);
            unsigned char binaryValue = (pixel.l >= threshold) ? 255 : 0;
            image.setPixel(x, y, L{binaryValue});
        }
    }
}

inline void invertImage(Image<L> &image)
{
    unsigned width = image.getWidth();
    unsigned height = image.getHeight();

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            const L &pixel = image.getPixel(x, y);
            unsigned char invertedValue = 255 - pixel.l;
            image.setPixel(x, y, L{invertedValue});
        }
    }
}

inline Image<RGBA> prepare_transparent_image(const Image<RGBA> &inputImage)
{
    unsigned width = inputImage.getWidth();
    unsigned height = inputImage.getHeight();
    Image<RGBA> outputImage(width, height);

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            const RGBA &pixel = inputImage.getPixel(x, y);
            unsigned char alphaValue = 255 - static_cast<unsigned char>(pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
            outputImage.setPixel(x, y, RGBA{pixel.r, pixel.g, pixel.b, alphaValue});
        }
    }

    return outputImage;
}

Image<RGBA> prepare_transparent_image(const Image<RGB> &inputImage);
Image<RGBA> prepare_transparent_image(const Image<L> &inputImage);

template <typename P>
void add_borders(Image<P> &image)
{
    auto width = image.getWidth();
    auto height = image.getHeight();
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

std::expected<std::pair<unsigned, unsigned>, bool> find_pixel(const Image<L> &image, unsigned char target_value);
