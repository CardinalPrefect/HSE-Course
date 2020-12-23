#include "point.h"

Point::Point(double x, double y) : x(x), y(y) {}

bool operator==(const Point& p1, const Point& p2) {
    return (p1.x == p2.x) and (p1.y == p2.y);
}

bool operator!=(const Point& p1, const Point& p2) {
    return (p1.x != p2.x) or (p1.y != p2.y);
}

bool operator<(const Point& lhs, const Point& rhs) {
    if (lhs.x < rhs.x)
        return true;

    if (lhs.x == rhs.x)
        return lhs.y < rhs.y;

    return false;
}
