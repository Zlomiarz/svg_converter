#pragma once

#include <fstream>
#include <filesystem>
#include "region.hpp"
#include "shape.hpp"

class SvgWriter
{
public:
    SvgWriter(const std::filesystem::path &filename, unsigned width, unsigned height);
    ~SvgWriter();

    void add_path(const std::vector<std::pair<Point, Point>> &lines, const std::string &color, int stroke_width);
    void add_path(const Shape &shape, const std::string &color, int stroke_width);

private:
    std::ofstream svg_file;
    void write_svg_header(unsigned width, unsigned height);
    void write_svg_footer();
};