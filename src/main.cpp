#include <cstdlib>
#include <iostream>
#include <chrono>
#include "image.hpp"
#include "image_utils.hpp"
#include "png_wrapper.hpp"
#include "skeletonize.hpp"

int main(int argc, char *argv[])
{
    /*if (argc < 2)
    {
        return EXIT_FAILURE;
    }*/
    auto inpath = "cat.png";
    auto outpath = "output.png";
    auto start = std::chrono::high_resolution_clock::now();
    PngWrapper pngwrapper;
    auto result = pngwrapper.read_png(inpath);
    std::visit([&pngwrapper, &outpath](auto &&image)
               {
                   auto transparent = prepare_transparent_image(image);
                   pngwrapper.write_png(outpath, transparent);
                   auto grayscale = convertToGrayscale(image);
                   binarizeImage(grayscale, 128);
                   invertImage(grayscale);
                   skeletonizeImage(grayscale);
                   invertImage(grayscale);
                   pngwrapper.write_png("output2.png", grayscale); },
               result.value());

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    return EXIT_SUCCESS;
}