#pragma once
#include <cmath>

class Point
{
public:
    Point() : x(0.0), y(0.0) {}
    Point(double x_, double y_) : x(x_), y(y_) {}
    double x, y;

    bool operator==(const Point &other) const
    {
        return std::abs(x - other.x) < std::numeric_limits<double>::epsilon() && std::abs(y - other.y) < std::numeric_limits<double>::epsilon();
    }
    bool operator!=(const Point &other) const
    {
        return !(*this == other);
    }

    friend Point operator+(const Point &p1, const Point &p2)
    {
        return Point(p1.x + p2.x, p1.y + p2.y);
    }
    friend Point operator-(const Point &p1, const Point &p2)
    {
        return Point(p1.x - p2.x, p1.y - p2.y);
    }
    double lenght()
    {
        return std::sqrt(x * x + y * y);
    }
    void normalize()
    {
        auto l = lenght();
        if (l > 0)
        {
            x /= l;
            y /= l;
        }
    }
};