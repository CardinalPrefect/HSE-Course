#include "ellipse.h"

Ellipse::Ellipse(Point focus1, Point focus2, double focusSum) : focuses(std::make_pair(focus1, focus2)),
                                                                       focusSum(focusSum) {
    a = focusSum / 2;
    c = eucl(focuses.first, focuses.second) / 2;
    b = std::sqrt(a * a - c * c);
}

Ellipse::Ellipse(const Ellipse& origin) : focuses(origin.focuses), focusSum(origin.focusSum), a(origin.a), b(origin.b),
                                          c(origin.c) {}

Ellipse& Ellipse::operator=(const Ellipse& origin) {
    focuses = origin.focuses;
    focusSum = origin.focusSum;
    return *this;
}

std::pair<Point, Point> Ellipse::getFocuses() {
    return focuses;
}

double Ellipse::eccentricity() const {
    return c / a;
}

Point Ellipse::center() const {
    return {(focuses.first.x + focuses.second.x) / 2, (focuses.first.y + focuses.second.y) / 2};
}

double Ellipse::area() const {
    return PI * a * b;
}

double Ellipse::perimeter() const {
    return 4 * (PI * a * b + (a - b)) / (a + b);
}

bool Ellipse::operator==(const Shape& another) const {
    const auto* other_ell = dynamic_cast<const Ellipse* >(&another);

    if (other_ell) {
        return focuses.first == other_ell->focuses.first && focuses.second == other_ell->focuses.second &&
               focusSum == other_ell->focusSum;
    }

    return false;
}

void Ellipse::rotate(Point center, double angle) {
    double x = focuses.first.x;
    double y = focuses.first.y;
    focuses.first.x =
            ((x - center.x) * cos(angle) - (y - center.y) * sin(angle)) + center.x;
    focuses.first.y =
            ((x - center.x) * sin(angle) + (y - center.y) * cos(angle)) + center.y;

    x = focuses.second.x;
    y = focuses.second.y;

    focuses.second.x =
            ((x - center.x) * cos(angle) - (y - center.y) * sin(angle)) + center.x;
    focuses.second.y =
            ((x - center.x) * sin(angle) + (y - center.y) * cos(angle)) + center.y;
}

void Ellipse::reflex(Point center) {
    focuses.first.x = 2 * center.x - focuses.first.x;
    focuses.first.y = 2 * center.y - focuses.first.y;
    focuses.second.x = 2 * center.x - focuses.second.x;
    focuses.second.y = 2 * center.y - focuses.second.y;
}

void Ellipse::reflex(Line axis) {
    Point p1 = axis.p1;
    Point p2 = axis.p2;
    Point p3 = focuses.first;
    double xx = p2.y - p1.y;
    double yy = p1.x - p2.x;
    double k = (p1.x * p2.y - p2.x * p1.y + p1.y * p3.x - p2.y * p3.x + p2.x * p3.y - p1.x * p3.y) / (xx * (p2.y - p1.y) * yy * (p1.x - p2.x));
    double px = p3.x + xx * k;
    double py = p3.y + yy * k;
    focuses.first.x = 2 * px - focuses.first.x;
    focuses.first.y = 2 * py - focuses.first.y;

    p3 = focuses.second;
    k = (p1.x * p2.y - p2.x * p1.y + p1.y * p3.x - p2.y * p3.x + p2.x * p3.y - p1.x * p3.y) / (xx * (p2.y - p1.y) + yy * (p1.x - p2.x));
    px = p3.x + xx * k;
    py = p3.y + yy * k;
    focuses.second.x = 2 * px - focuses.second.x;
    focuses.second.y = 2 * py - focuses.second.y;


//    Point _a = axis.p1;
//    Point _b = axis.p2;
//    Point _c = focuses.first;
//    double v_x = _b.y - _a.y;
//    double v_y = _a.x - _b.x;

//    double k = (_a.x * _b.y - _b.x * _a.y + _a.y * _c.x - _b.y * _c.x + _b.x * _c.y - _a.x * _c.y) /
//               (v_x * (_b.y - _a.y) + v_y * (_a.x - _b.x));
//    double p_x = _c.x + v_x * k;
//    double p_y = _c.y + v_y * k;
//    focuses.first.x = 2 * p_x - focuses.first.x;
//    focuses.first.y = 2 * p_y - focuses.first.y;

//    _c = focuses.second;
//    k = (_a.x * _b.y - _b.x * _a.y + _a.y * _c.x - _b.y * _c.x + _b.x * _c.y - _a.x * _c.y) /
//        (v_x * (_b.y - _a.y) + v_y * (_a.x - _b.x));
//    p_x = _c.x + v_x * k;
//    p_y = _c.y + v_y * k;
//    focuses.second.x = 2 * p_x - focuses.second.x;
//    focuses.second.y = 2 * p_y - focuses.second.y;
}

void Ellipse::scale(Point center, double coefficient) {
    focuses.first.x = coefficient * (focuses.first.x - center.x) + center.x;
    focuses.first.y = coefficient * (focuses.first.y - center.y) + center.y;
    focuses.second.x = coefficient * (focuses.second.x - center.x) + center.x;
    focuses.second.y = coefficient * (focuses.second.y - center.y) + center.y;
}

bool Ellipse::containsPoint(Point point) const {
    return eucl(focuses.first, point) + eucl(focuses.second, point) <= focusSum;
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    const auto* ellipse_another = dynamic_cast<const Ellipse*>(&another);
    return ellipse_another && (a / ellipse_another->a == b / ellipse_another->b);
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    const auto* ellipse_another = dynamic_cast<const Ellipse*>(&another);

    if (ellipse_another) {
        double k = a / ellipse_another->a;
        return k == b / ellipse_another->b && k == 1;
    }

    return false;
}
