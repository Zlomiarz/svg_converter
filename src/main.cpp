#include <cstdlib>
#include <iostream>
#include <chrono>
#include "image.hpp"
#include "image_utils.hpp"
#include "png_wrapper.hpp"
#include "skeletonize.hpp"
#include "region.hpp"
#include "shape.hpp"
#include "svg_writer.hpp"
#include "visvalingam.hpp"

void generate_svg(Image<L> &image, const std::string &outpath)
{
    binarizeImage(image, 128);
    invertImage(image);
    skeletonizeImage(image);
    invertImage(image);
    add_borders(image);
    auto coords = find_pixel(image, 255);
    std::cout << "Generating SVG..." << std::endl;
    SvgWriter svg_writer(outpath + ".svg", image.getWidth(), image.getHeight());
    while (coords.has_value())
    {
        Region region(Point(coords->first, coords->second), image);
        auto lines = region.get_lines();
        Shape shape(lines);
        svg_writer.add_path(shape, "black", 1);
        coords = find_pixel(image, 255);
    }
}

int main(int, char **)
{
    /*if (argc < 2)
    {
        return EXIT_FAILURE;
    }*/
    auto inpath = "cat.png";
    auto outpath = "output";
    auto start = std::chrono::high_resolution_clock::now();
    PngWrapper pngwrapper;
    auto result = pngwrapper.read_png(inpath);
    std::visit([&pngwrapper, &outpath](auto &&image)
               {
                    auto transparent = prepare_transparent_image(image);
                    pngwrapper.write_png(outpath+std::string(".png"), transparent);
                    auto grayscale = convertToGrayscale(image);
                    generate_svg(grayscale, outpath); },
               result.value());

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    return EXIT_SUCCESS;
}