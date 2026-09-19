#pragma once

#include <vector>
#include "point.hpp"

class Path
{
public:
    Path() = default;
    std::vector<Point> points;

    void connect_collinear()
    {
        std::vector<Point> p;
        Point start = points[0];
        Point end = points[1];
        Point d1 = start - end;
        d1.normalize();
        p.push_back(start);
        for (size_t i = 2; i < points.size(); i++)
        {
            Point d2 = end - points[i];
            d2.normalize();
            if (d1 == d2)
            {
                end = points[i];
            }
            else
            {
                p.push_back(end);
                start = end;
                end = points[i];
                d1 = start - end;
                d1.normalize();
            }
        }
        points.push_back(end);
        points = std::move(p);
    }
};