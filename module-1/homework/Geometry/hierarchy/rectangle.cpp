#include "rectangle.h"

Rectangle::Rectangle(Point point1, Point point2, double attitude) {
    points.push_back(point1);
    points.push_back(Point(point1.x, point2.y));
    points.push_back(point2);
    points.push_back(Point(point2.x, point1.y));
}

std::pair<Line, Line> Rectangle::diagonals() {
    return std::pair<Line, Line>(Line(points[0], points[2]), Line(points[1], points[3]));
}

Point Rectangle::center() {
    return Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2);
}

Rectangle::Rectangle(const Rectangle& origin) {
    points = origin.points;
}

Rectangle& Rectangle::operator=(const Rectangle& origin) {
    points = origin.points;

    return *this;
}
