#include <gtest/gtest.h>

#include "image.hpp"

TEST(ImageTest, instantiateImage)
{
    Image<RGB> image(10, 10);
    EXPECT_EQ(image.getWidth(), 10);
    EXPECT_EQ(image.getHeight(), 10);

    RGB pixel{255, 0, 0};
    image.setPixel(5, 5, pixel);
    const RGB &retrievedPixel = image.getPixel(5, 5);
    EXPECT_EQ(retrievedPixel.r, 255);
    EXPECT_EQ(retrievedPixel.g, 0);
    EXPECT_EQ(retrievedPixel.b, 0);
}