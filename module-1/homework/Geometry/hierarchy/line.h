#include "point.h"

#ifndef HIERARCHY_LINE_H
#define HIERARCHY_LINE_H

struct Line {
    Line(Point p1, Point p2);

    friend bool operator==(const Line& l1, const Line& l2);
    friend bool operator!=(const Line& l1, const Line& l2);

    Point p1;
    Point p2;
};

#endif //HIERARCHY_LINE_H
