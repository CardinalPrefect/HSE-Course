#include "polygon.h"
#include <utility>

#ifndef HEIRARCHY_RECTANGLE_H
#define HEIRARCHY_RECTANGLE_H

class Rectangle : public Polygon {
public:
    Rectangle(Point point1, Point point2, double attitude);

    Rectangle(const Rectangle& origin);

    Rectangle& operator=(const Rectangle& origin);

    ~Rectangle() = default;

    Point center();
    std::pair<Line, Line> diagonals();

protected:
    Rectangle() = default;
};

#endif //HEIRARCHY_RECTANGLE_H
