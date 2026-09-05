#pragma once

#include "image.hpp"
#include <list>

class Point
{
public:
    Point(double x_, double y_) : x(x_), y(y_) {}
    double x, y;
};

class Region
{
    Image<L> &image;
    std::vector<unsigned> grid;
    std::vector<std::pair<Point, Point>> lines;

    void build_region(const Point &coords)
    {
        std::list<Point> pixels;
        pixels.push_back(coords);
        while (!pixels.empty())
        {
            auto current = pixels.back();
            pixels.pop_back();
            double x = current.x;
            double y = current.y;

            L pixel = image.getPixel(x, y);
            if (pixel.l == 0)
                continue;

            set_cell(current, 1);
            pixel.l = 0;
            image.setPixel(x, y, pixel);

            add_next_pixel(pixels, {x + 1, y});
            add_next_pixel(pixels, {x - 1, y});
            add_next_pixel(pixels, {x, y + 1});
            add_next_pixel(pixels, {x, y - 1});
        }
    }

    void add_next_pixel(std::list<Point> &pixels, const Point &coords)
    {
        unsigned x = coords.x;
        unsigned y = coords.y;
        if (x >= image.getWidth() || y >= image.getHeight())
            return;
        if (get_cell(coords) == 0)
        {
            pixels.push_back(coords);
        }
    }

    unsigned get_cell(const Point &coords) const
    {
        unsigned x = coords.x;
        unsigned y = coords.y;
        if (x >= image.getWidth() || y >= image.getHeight())
            return 0;
        return grid[y * image.getWidth() + x];
    }

    void set_cell(const Point &coords, unsigned value)
    {
        unsigned x = coords.x;
        unsigned y = coords.y;
        if (x >= image.getWidth() || y >= image.getHeight())
            return;
        grid[y * image.getWidth() + x] = value;
    }

    void convert_grid_to_lines()
    {
        for (unsigned y = 0; y < image.getHeight() - 1; ++y)
        {
            unsigned x = 0;
            while (x < image.getWidth() - 1)
            {
                int v = 0;
                if (get_cell(Point(x, y)) == 1)
                    v |= 1;
                if (get_cell(Point(x + 1, y)) == 1)
                    v |= 2;
                if (get_cell(Point(x + 1, y + 1)) == 1)
                    v |= 4;
                if (get_cell(Point(x, y + 1)) == 1)
                    v |= 8;

                switch (v)
                {
                case 0:
                case 15:
                default:
                    break;
                case 1:
                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 0.5, y)));
                    break;
                case 2:
                    lines.push_back(std::make_pair(Point(x + 0.5, y), Point(x + 1, y + 0.5)));
                    break;
                case 3:

                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 1, y + 0.5)));
                    break;
                case 4:

                    lines.push_back(std::make_pair(Point(x + 0.5, y + 1), Point(x + 1, y + 0.5)));
                    break;
                case 5:
                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 0.5, y)));
                    lines.push_back(std::make_pair(Point(x + 0.5, y + 1), Point(x + 1, y + 0.5)));
                    break;
                case 6:
                    lines.push_back(std::make_pair(Point(x + 0.5, y), Point(x + 0.5, y + 1)));
                    break;
                case 7:
                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 0.5, y + 1)));
                    break;
                case 8:
                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 0.5, y + 1)));
                    break;
                case 9:
                    lines.push_back(std::make_pair(Point(x + 0.5, y), Point(x + 0.5, y + 1)));
                    break;
                case 10:
                    lines.push_back(std::make_pair(Point(x + 0.5, y), Point(x + 1, y + 0.5)));
                    lines.push_back(std::make_pair(Point(x + 0.5, y + 1), Point(x, y + 0.5)));
                    break;
                case 11:
                    lines.push_back(std::make_pair(Point(x + 0.5, y + 1), Point(x + 1, y + 0.5)));
                    break;
                case 12:
                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 1, y + 0.5)));
                    break;
                case 13:
                    lines.push_back(std::make_pair(Point(x + 0.5, y), Point(x + 1, y + 0.5)));
                    break;
                case 14:
                    lines.push_back(std::make_pair(Point(x, y + 0.5), Point(x + 0.5, y)));
                    break;
                }
            }
        }
    }

public:
    Region(const Point &coords_, Image<L> &image_) : image(image_)
    {
        grid.resize(image.getWidth() * image.getHeight(), 0);
        build_region(coords_);
    }
};