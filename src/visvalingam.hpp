#pragma once

#include <algorithm>
#include <list>

#include "point.hpp"
#include "path.hpp"

class Visvalingam
{
    struct Node
    {
        Point p;
        double area = 0.0;
        Node(Point _p) : p(_p) {}
    };
    std::list<Node> points;

public:
    Visvalingam(Path &path, double threshold)
    {
        for (auto &p : path.points)
        {
            points.emplace_back(Node(p));
        }
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            calculate_area(it);
        }
        while (true)
        {
            if (points.size() <= 3)
                break;
            auto it = find_minimal_area();
            std::cout << "minimal area" << it->area << std::endl;
            if (it->area > threshold)
                break;
            it = points.erase(it);
            if (it == points.end())
            {
                calculate_area(points.begin());
                calculate_area(--points.end());
            }
            else
            {
                calculate_area(it);
                calculate_area(--it);
            }
        }
    }

    std::list<Node>::iterator find_minimal_area()
    {
        return std::min_element(points.begin(), points.end(), [](auto p1, auto p2)
                                { return p1.area < p2.area; });
    }

    Point get_prev_point(std::list<Node>::iterator it)
    {
        if (it == points.begin())
        {
            return points.back().p;
        }
        return (--it)->p;
    }

    Point get_next_point(std::list<Node>::iterator it)
    {
        if (++it == points.end())
        {
            return points.begin()->p;
        }
        return it->p;
    }

    void calculate_area(const std::list<Node>::iterator &it)
    {
        auto prev_point = get_prev_point(it);
        auto next_point = get_next_point(it);
        it->area = calculate_area(prev_point, it->p, next_point);
    }

    double calculate_area(const Point p1, const Point p2, const Point p3)
    {
        return std::abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0);
    }

    Path get_simplified_path()
    {
        Path path;
        path.points.resize(path.points.size());
        for (auto &p : points)
        {
            path.points.push_back(p.p);
        }
        return path;
    }
};