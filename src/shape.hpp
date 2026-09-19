#pragma once

#include <iostream>

#include "point.hpp"
#include "path.hpp"
#include "visvalingam.hpp"

class Shape
{
public:
    Shape(std::list<std::pair<Point, Point>> lines)
    {
        process_lines(lines);
        connect_collinear();
        run_visvalingam();
    }

    void process_lines(std::list<std::pair<Point, Point>> &lines)
    {
        auto start = lines.front();
        lines.pop_front();
        paths.emplace_back();
        paths.back().points.push_back(start.first);
        paths.back().points.push_back(start.second);
        while (lines.size() > 0)
        {
            for (auto it = lines.begin(); it != lines.end(); ++it)
            {
                if (it->first == paths.back().points.back())
                {
                    paths.back().points.push_back(it->second);
                    lines.erase(it);
                    break;
                }
                if (it->second == paths.back().points.back())
                {
                    paths.back().points.push_back(it->first);
                    lines.erase(it);
                    break;
                }
            }
            if (!lines.empty() && start.first == paths.back().points.back())
            {
                start = lines.front();
                lines.pop_front();
                paths.emplace_back();
                paths.back().points.push_back(start.first);
                paths.back().points.push_back(start.second);
            }
        }
    }

    void connect_collinear()
    {
        for (auto &p : paths)
        {
            p.connect_collinear();
        }
    }

    void run_visvalingam()
    {
        for (size_t i = 0; i < paths.size(); ++i)
        {
            auto l1 = paths[i].points.size();
            Visvalingam v(paths[i], 10);
            paths[i] = v.get_simplified_path();
            auto l2 = paths[i].points.size();
            std::cout << "visivigam removed " << l1 - l2 << " points" << std::endl;
        }
    }

    std::vector<Path> paths;
};