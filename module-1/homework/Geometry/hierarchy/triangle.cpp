#include "triangle.h"

Triangle::Triangle(Point x, Point y, Point z) {
    points.push_back(x);
    points.push_back(y);
    points.push_back(z);
}

Triangle::Triangle(const Triangle& origin) {
    points = origin.points;
}

Triangle& Triangle::operator=(const Triangle& origin) {
    points = origin.points;
    return *this;
}

Circle Triangle::circumscribedCircle() {
    double a = eucl(points[0], points[1]);
    double b = eucl(points[1], points[2]);
    double c = eucl(points[2], points[0]);

    Point center = Point((a * points[0].x + b * points[1].x + c * points[2].x) / (a + b + c),
                         (a * points[0].y + b * points[1].y + c * points[2].y) / (a + b + c));
    double p = (a + b + c) / 2;
    double r = std::sqrt((p - a) * (p - b) * (p - c) / p);
    return Circle(center, r);
}

Circle Triangle::inscribedCircle() {
    double xy_1 = std::pow(points[0].x, 2) + std::pow(points[0].y, 2);
    double xy_2 = std::pow(points[1].x, 2) + std::pow(points[1].y, 2);
    double xy_3 = std::pow(points[2].x, 2) + std::pow(points[2].y, 2);
    double a = xy_1 * points[1].y + xy_2 * points[2].y + xy_3 * points[0].y - xy_3 * points[1].y - xy_2 * points[0].y -
               xy_1 * points[2].y;
    double b = points[0].x * xy_2 + points[1].x * xy_3 + points[2].x * xy_1 - xy_2 * points[2].x - xy_1 * points[1].x -
               xy_3 * points[0].x;
    double c = points[0].x * points[1].y + points[1].x * points[2].y + points[2].x * points[0].y -
               points[2].x * points[0].y - points[0].x * points[2].y;

    Point center = Point(a / (2 * c), b / (2 * c));
    double r = eucl(center, points[0]);

    return Circle(center, r);
}
