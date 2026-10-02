#include <cstdlib>
#include <iostream>
#include <chrono>

#include "CLI11.hpp"

#include "image.hpp"
#include "image_utils.hpp"
#include "image_reader.hpp"
#include "skeletonize.hpp"
#include "region.hpp"
#include "shape.hpp"
#include "svg_writer.hpp"
#include "visvalingam.hpp"

void generate_svg(Image<L> &image, const std::string &outpath, bool skeletonize)
{
    binarizeImage(image, 128);
    invertImage(image);
    if (skeletonize)
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

int main(int argc, char *argv[])
{
    CLI::App app{"Application that converts PNG/JPEG images into SVG coloring pages"};
    argv = app.ensure_utf8(argv);

    std::string inpath = "in.png";
    app.add_option("-f,--file", inpath, "Input file")->required();
    std::string outpath = "out";
    app.add_option("-o,--out", outpath, "Output path without file extension. PNG/SVG will be added autmaticaly")->required();
    bool generate_mask = false;
    app.add_flag("-m", generate_mask, "Generate transparent PNG mask");
    bool skeletonize = false;
    app.add_flag("-s", skeletonize, "Run skeltonization algorithm");

    CLI11_PARSE(app, argc, argv);

    auto start = std::chrono::high_resolution_clock::now();

    ImageReader reader;

    auto result = reader.read(inpath);

    if (!result)
    {

        return EXIT_FAILURE;
    }

    std::visit([&outpath, generate_mask, skeletonize](auto &&image)
               {
                    if (generate_mask){
                        PngWrapper pngwrapper;
                        auto transparent = prepare_transparent_image(image);
                        pngwrapper.write_png(outpath+std::string(".png"), transparent);
                    }
                    auto grayscale = convertToGrayscale(image);
                    generate_svg(grayscale, outpath, skeletonize); },
               result.value());

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    return EXIT_SUCCESS;
}