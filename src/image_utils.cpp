#include "image_utils.hpp"

Image<RGBA> prepare_transparent_image(const Image<L> &inputImage)
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

Image<RGBA> prepare_transparent_image(const Image<RGB> &inputImage)
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

std::expected<std::pair<unsigned, unsigned>, bool> find_pixel(const Image<L> &image, unsigned char target_value)
{
    for (unsigned y = 0; y < image.getHeight(); ++y)
    {
        for (unsigned x = 0; x < image.getWidth(); ++x)
        {
            if (image.getPixel(x, y).l == target_value)
            {
                return std::make_pair(x, y);
            }
        }
    }
    return std::unexpected(false); // Return unexpected if not found
}
