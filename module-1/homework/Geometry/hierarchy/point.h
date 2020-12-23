#ifndef HIERARCHY_POINT_H
#define HIERARCHY_POINT_H

struct Point {
    Point(double x, double y);

    friend bool operator==(const Point& p1, const Point& p2);
    friend bool operator!=(const Point& p1, const Point& p2);
    friend bool operator<(const Point& lhs, const Point& rhs);

    double x;
    double y;
};

#endif //HIERARCHY_POINT_H
