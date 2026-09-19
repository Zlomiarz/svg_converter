#include "svg_writer.hpp"

SvgWriter::SvgWriter(const std::filesystem::path &filename, unsigned width, unsigned height)
{
    svg_file.open(filename);
    if (!svg_file.is_open())
    {
        throw std::runtime_error("Could not open SVG file for writing.");
    }
    write_svg_header(width, height);
}

SvgWriter::~SvgWriter()
{
    write_svg_footer();
    svg_file.close();
}

void SvgWriter::write_svg_header(unsigned width, unsigned height)
{
    svg_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    svg_file << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" << width << "\" height=\"" << height << "\">\n";
}

void SvgWriter::add_path(const std::vector<std::pair<Point, Point>> &lines, const std::string &color, int stroke_width)
{
    svg_file << "<path d=\" ";
    for (const auto &pair : lines)
    {
        svg_file << "M " << pair.first.x << " " << pair.first.y << " L " << pair.second.x << " " << pair.second.y << " ";
    }
    svg_file << " Z\" fill=\"transparent\" stroke=\"" << color << "\" stroke-width=\"" << stroke_width << "\" />\n";
}

void SvgWriter::add_path(const Shape &shape, const std::string &color, int stroke_width)
{
    svg_file << "<path d=\" ";
    for (auto &path : shape.paths)
    {
        svg_file << " M" << path.points[0].x << " " << path.points[0].y;
        for (size_t i = 1; i < path.points.size(); ++i)
        {
            svg_file << " L" << path.points[i].x << " " << path.points[i].y;
        }
        svg_file << " L" << path.points[0].x << " " << path.points[0].y;
    }
    svg_file << " Z\" fill=\"transparent\" stroke=\"" << color << "\" stroke-width=\"" << stroke_width << "\" />\n";
}

void SvgWriter::write_svg_footer()
{
    svg_file << "</svg>\n";
}