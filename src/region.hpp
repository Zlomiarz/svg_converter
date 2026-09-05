#pragma once

#include "image.hpp"
#include <list>

class Region
{
    Image<L> &image;
    std::vector<unsigned> grid;

    void build_region(const std::pair<unsigned, unsigned> &coords)
    {
        std::list<std::pair<unsigned, unsigned>> pixels;
        pixels.push_back(coords);
        while (!pixels.empty())
        {
            auto current = pixels.back();
            pixels.pop_back();
            unsigned x = current.first;
            unsigned y = current.second;

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

    void add_next_pixel(std::list<std::pair<unsigned, unsigned>> &pixels, const std::pair<unsigned, unsigned> &coords)
    {
        unsigned x = coords.first;
        unsigned y = coords.second;
        if (x >= image.getWidth() || y >= image.getHeight())
            return;
        if (get_cell(coords) == 0)
        {
            pixels.push_back(coords);
        }
    }

    unsigned get_cell(std::pair<unsigned, unsigned> coords) const
    {
        unsigned x = coords.first;
        unsigned y = coords.second;
        if (x >= image.getWidth() || y >= image.getHeight())
            return 0;
        return grid[y * image.getWidth() + x];
    }

    void set_cell(std::pair<unsigned, unsigned> coords, unsigned value)
    {
        unsigned x = coords.first;
        unsigned y = coords.second;
        if (x >= image.getWidth() || y >= image.getHeight())
            return;
        grid[y * image.getWidth() + x] = value;
    }

public:
    Region(std::pair<unsigned, unsigned> coords_, Image<L> &image_) : image(image_)
    {
        grid.resize(image.getWidth() * image.getHeight(), 0);
        build_region(coords_);
    }
};