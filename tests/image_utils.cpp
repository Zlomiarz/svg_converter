#include <gtest/gtest.h>

#include "image_utils.hpp"

TEST(ImageUtilsTest, convertToGrayscale)
{
    Image<RGB> inputImage(2, 2);
    inputImage.setPixel(0, 0, RGB{255, 0, 0});     // Red
    inputImage.setPixel(1, 0, RGB{0, 255, 0});     // Green
    inputImage.setPixel(0, 1, RGB{0, 0, 255});     // Blue
    inputImage.setPixel(1, 1, RGB{255, 255, 255}); // White

    Image<L> outputImage = convertToGrayscale(inputImage);

    EXPECT_EQ(outputImage.getWidth(), 2);
    EXPECT_EQ(outputImage.getHeight(), 2);

    EXPECT_EQ(outputImage.getPixel(0, 0).l, static_cast<unsigned char>(0.299 * 255 + 0.587 * 0 + 0.114 * 0)); // Red to grayscale
    EXPECT_EQ(outputImage.getPixel(1, 0).l, static_cast<unsigned char>(0.299 * 0 + 0.587 * 255 + 0.114 * 0)); // Green to grayscale
    EXPECT_EQ(outputImage.getPixel(0, 1).l, static_cast<unsigned char>(0.299 * 0 + 0.587 * 0 + 0.114 * 255)); // Blue to grayscale
    EXPECT_EQ(outputImage.getPixel(1, 1).l, static_cast<unsigned char>(255));                                 // White to grayscale
}

TEST(ImageUtilsTest, binarizeImage)
{
    Image<L> inputImage(2, 2);
    inputImage.setPixel(0, 0, L{100});
    inputImage.setPixel(1, 0, L{200});
    inputImage.setPixel(0, 1, L{50});
    inputImage.setPixel(1, 1, L{150});

    unsigned char threshold = 150;
    Image<L> outputImage = binarizeImage(inputImage, threshold);

    EXPECT_EQ(outputImage.getWidth(), 2);
    EXPECT_EQ(outputImage.getHeight(), 2);

    EXPECT_EQ(outputImage.getPixel(0, 0).l, static_cast<unsigned char>(0));   // Below threshold
    EXPECT_EQ(outputImage.getPixel(1, 0).l, static_cast<unsigned char>(255)); // Above threshold
    EXPECT_EQ(outputImage.getPixel(0, 1).l, static_cast<unsigned char>(0));   // Below threshold
    EXPECT_EQ(outputImage.getPixel(1, 1).l, static_cast<unsigned char>(255)); // Above threshold
}