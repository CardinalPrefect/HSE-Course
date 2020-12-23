#include "rectangle.h"
#include "circle.h"
#include <utility>

#ifndef HIERARCHY_SQUARE_H
#define HIERARCHY_SQUARE_H

class Square : protected Rectangle {
public:
    Square() = delete;

    Square(std::pair<Point, Point> points);

    Square(const Square& origin);

    Square& operator=(const Square& origin);

    ~Square() = default;

    Circle circumscribedCircle();

    Circle inscribedCircle();
};

#endif //HIERARCHY_SQUARE_H
