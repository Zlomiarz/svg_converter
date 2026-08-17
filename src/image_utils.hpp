#include "image.hpp"

Image<L> convertToGrayscale(const Image<RGB> &inputImage)
{
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<L> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const RGB &pixel = inputImage.getPixel(x, y);
            unsigned char grayValue = static_cast<unsigned char>(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
            outputImage.setPixel(x, y, L{grayValue});
        }
    }

    return outputImage;
}

Image<L> binarizeImage(const Image<L> &inputImage, unsigned char threshold)
{
    int width = inputImage.getWidth();
    int height = inputImage.getHeight();
    Image<L> outputImage(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const L &pixel = inputImage.getPixel(x, y);
            unsigned char binaryValue = (pixel.l >= threshold) ? 255 : 0;
            outputImage.setPixel(x, y, L{binaryValue});
        }
    }

    return outputImage;
}