#include"square.h"

Square::Square(std::pair<Point, Point> point) {
    points.push_back(point.first);
    points.push_back(Point(point.first.x, point.second.y));
    points.push_back(point.second);
    points.push_back(Point(point.second.x, point.first.y));
}

Square::Square(const Square& origin) {
    points = origin.points;
}

Square& Square::operator=(const Square& origin) {
    points = origin.points;
    return *this;
}

Circle Square::inscribedCircle() {
    Point p_r = Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2);
    double r = eucl(p_r, points[0]);
    return Circle(p_r, r);
}

Circle Square::circumscribedCircle() {
    Point p_r = Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2);
    double r = eucl(points[0], points[2]) / 2;
    return Circle(p_r, r);
}
