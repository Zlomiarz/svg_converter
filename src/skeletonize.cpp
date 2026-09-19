#include "skeletonize.hpp"

#include <algorithm>

inline int A(const std::vector<unsigned> &values)
{
    int count = 0;
    auto prev = values.back();
    for (size_t i = 1; i < values.size(); ++i)
    {
        if (prev == 0 && values[i] != 0)
        {
            count++;
        }
        prev = values[i];
    }
    return count;
}

inline long B(const std::vector<unsigned> &values)
{
    return std::count_if(values.begin(), values.end(), [](unsigned value)
                         { return value != 0; });
}

inline int firstPass(const Image<L> &inputImage, Image<L> &outputImage)
{
    std::vector<unsigned> values;
    values.reserve(9);
    int ret = 0;
    for (unsigned x = 1; x < inputImage.getWidth() - 1; ++x)
    {
        for (unsigned y = 1; y < inputImage.getHeight() - 1; ++y)
        {
            auto pixel = inputImage.getPixel(x, y);
            if (pixel.l != 0)
            {
                values.clear();
                values.push_back(inputImage.getPixel(x, y).l);         // P1
                values.push_back(inputImage.getPixel(x, y - 1).l);     // P2
                values.push_back(inputImage.getPixel(x + 1, y - 1).l); // P3
                values.push_back(inputImage.getPixel(x + 1, y).l);     // P4
                values.push_back(inputImage.getPixel(x + 1, y + 1).l); // P5
                values.push_back(inputImage.getPixel(x, y + 1).l);     // P6
                values.push_back(inputImage.getPixel(x - 1, y + 1).l); // P7
                values.push_back(inputImage.getPixel(x - 1, y).l);     // P8
                values.push_back(inputImage.getPixel(x - 1, y - 1).l); // P9
                if (A(values) == 1 and B(values) >= 2 and B(values) <= 6)
                {
                    if ((values[1] * values[3] * values[5] == 0) and (values[3] * values[5] * values[7] == 0))
                    {
                        pixel.l = 0;
                        ret++;
                    }
                }
            }
            outputImage.setPixel(x, y, pixel);
        }
    }
    return ret;
}

inline int secondPass(const Image<L> &inputImage, Image<L> &outputImage)
{
    std::vector<unsigned> values;
    values.reserve(9);
    int ret{0};
    for (unsigned x = 1; x < inputImage.getWidth() - 1; ++x)
    {
        for (unsigned y = 1; y < inputImage.getHeight() - 1; ++y)
        {
            auto pixel = inputImage.getPixel(x, y);
            if (pixel.l != 0)
            {
                values.clear();
                values.push_back(inputImage.getPixel(x, y).l);         // P1
                values.push_back(inputImage.getPixel(x, y - 1).l);     // P2
                values.push_back(inputImage.getPixel(x + 1, y - 1).l); // P3
                values.push_back(inputImage.getPixel(x + 1, y).l);     // P4
                values.push_back(inputImage.getPixel(x + 1, y + 1).l); // P5
                values.push_back(inputImage.getPixel(x, y + 1).l);     // P6
                values.push_back(inputImage.getPixel(x - 1, y + 1).l); // P7
                values.push_back(inputImage.getPixel(x - 1, y).l);     // P8
                values.push_back(inputImage.getPixel(x - 1, y - 1).l); // P9
                if (A(values) == 1 and B(values) >= 2 and B(values) <= 6)
                {
                    if ((values[1] * values[3] * values[7] == 0) and (values[1] * values[5] * values[7] == 0))
                    {
                        pixel.l = 0;
                        ret++;
                    }
                }
            }
            outputImage.setPixel(x, y, pixel);
        }
    }
    return ret;
}

void skeletonizeImage(Image<L> &image)
{
    auto width = image.getWidth();
    auto height = image.getHeight();
    Image<L> tmpImage(width, height);
    int result{0};
    do
    {
        result = firstPass(image, tmpImage);
        if (result > 0)
        {
            result = secondPass(tmpImage, image);
        }
        else
        {
            image = std::move(tmpImage);
        }
    } while (result > 0);
}