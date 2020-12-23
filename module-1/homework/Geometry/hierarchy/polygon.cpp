#include "polygon.h"


Polygon::Polygon(std::vector<Point>& points) : points(points) { }

Polygon::Polygon(const Polygon& origin) {
    points = origin.points;
}

Polygon& Polygon::operator=(const Polygon& origin) {
    points = origin.points;
    return *this;
}

Polygon::~Polygon() {
    points.clear();
}

size_t Polygon::verticesCount() const {
    return points.size();
}

double Polygon::perimeter() const {
    double perimeter = 0;

    perimeter += eucl(points.back(), points.front());

    for (size_t i = 0; i < verticesCount() - 1; ++i) {
        perimeter += eucl(points[i], points[i + 1]);
    }

    return perimeter;
}

double Polygon::area() const {
    double sum = 0;
    size_t size = verticesCount();

    for (size_t i = 0; i < size; ++i)
        sum += points[i].x * points[(i + 1) % size].y - points[(i + 1) % size].x * points[i].y;

    sum = std::abs(sum);
    sum /= 2;

    return sum;
}

bool Polygon::operator==(const Shape& another) const {
    const auto* other_pol = dynamic_cast<const Polygon*>(&another);

    if (other_pol) {
        if (verticesCount() != other_pol->verticesCount())
            return false;

        auto points1 = points;
        auto points2 = other_pol->points;
        std::sort(points1.begin(), points1.end());
        std::sort(points2.begin(), points2.end());

        for (size_t i = 0; i < verticesCount(); ++i)
            if (eucl(points1[i], points2[i]) > EPS)
                return false;

        return true;
    }

    return false;
}

bool Polygon::isCongruentTo(const Shape& another) const {
    const auto& other_pol = dynamic_cast<const Polygon&>(another);

    double k = eucl(points.front(), points.back()) / eucl(other_pol.points.front(), other_pol.points.back());

    return k == 1 && isSimilarTo(another);
}

bool Polygon::isSimilarTo(const Shape& another) const {
    const auto& other_pol = dynamic_cast<const Polygon&>(another);

    if (verticesCount() != other_pol.verticesCount())
        return false;

    double k = eucl(points.front(), points.back()) / eucl(other_pol.points.front(), other_pol.points.back());

    for (size_t i = 0; i < verticesCount() - 1; ++i) {
        double nk = eucl(points[i], points[i + 1]) / eucl(other_pol.points[i], other_pol.points[i + 1]);

        if (nk != k)
            return false;
    }

    return true;
}

bool Polygon::containsPoint(Point point) const {
    bool flag;
    double sFullTriangle;
    double sFirstPartTriangle;
    double sSecondPartTriangle;
    double sThirdPartTriangle;

    for (size_t i = 0; i < verticesCount(); ++i) {
        for (size_t j = 0; j < verticesCount() - 2; ++j) {
            size_t i1 = (j + i) % verticesCount();
            size_t i2 = (j + i + 1) % verticesCount();

            sFullTriangle = std::abs(points[i1].x * (points[i2].y - points[i].y) +
                            points[i2].x * (points[i].y - points[i1].y) +
                            points[i].x * (points[i1].y - points[i2].y));
            sFirstPartTriangle = std::abs(points[i1].x * (points[i2].y - point.y) +
                                     points[i2].x * (point.y - points[i1].y) +
                                     point.x * (points[i1].y - points[i2].y));
            sSecondPartTriangle = std::abs(points[i].x * (points[i2].y - point.y) +
                                      points[i2].x * (point.y - points[i].y) +
                                      point.x * (points[i].y - points[i2].y));
            sThirdPartTriangle = std::abs(points[i1].x * (points[i].y - point.y) +
                                     points[i].x * (point.y - points[i1].y) +
                                     point.x * (points[i1].y - points[i].y));

            if (sFullTriangle == sFirstPartTriangle + sSecondPartTriangle + sThirdPartTriangle) {
                flag = true;
                break;
            }
        }

        if (!flag)
            break;
    }

    return flag;
}

void Polygon::rotate(Point center, double angle) {
    angle = angle / 180 * PI;
    for (size_t i = 0; i < verticesCount(); ++i) {
        double x = points[i].x - center.x;
        double y = points[i].y - center.y;
        points[i].x = (x * cos(angle) - y * sin(angle)) + center.x;
        points[i].y = (x * sin(angle) + y * cos(angle)) + center.y;
    }
}

void Polygon::reflex(Point center) {
    for (size_t i = 0; i < verticesCount(); ++i) {
        points[i].x = 2 * center.x - points[i].x;
        points[i].y = 2 * center.y - points[i].y;
    }
}

void Polygon::reflex(Line axis) {
    for (size_t i = 0; i < verticesCount(); ++i) {
        Point a = axis.p1;
        Point b = axis.p2;
        Point c = points[i];
        double v_x = b.y - a.y;
        double v_y = a.x - b.x;
        double k = (a.x * b.y - b.x * a.y + a.y * c.x - b.y * c.x + b.x * c.y - a.x * c.y) /
                   (v_x * (b.y - a.y) + v_y * (a.x - b.x));
        double p_x = c.x + v_x * k;
        double p_y = c.y + v_y * k;
        points[i].x = 2 * p_x - points[i].x;
        points[i].y = 2 * p_y - points[i].y;
    }
}

void Polygon::scale(Point center, double coefficient) {
    for (size_t i = 0; i < verticesCount(); ++i) {
        points[i].x = coefficient * (points[i].x - center.x) + center.x;
        points[i].y = coefficient * (points[i].y - center.y) + center.y;
    }
}

std::vector<Point> Polygon::getVertices() const {
    return points;
}

bool Polygon::isConvex() const {
    Point vector_a_buf{
            points.front().x - points.back().x,
            points.front().y - points.back().y
    };

    Point vector_b_buf{
            points[0].x - points[1].x,
            points[0].y - points[1].y
    };

    bool sign = vector_a_buf.x * vector_b_buf.y - vector_a_buf.y * vector_b_buf.x >= 0;

    for (size_t i = 1; i < verticesCount(); ++i) {
        Point vector_a{
                points[i].x - points[i - 1].x,
                points[i].y - points[i - 1].y
        };

        Point vector_b{
                points[i].x - points[(i + 1) % verticesCount()].x,
                points[i].y - points[(i + 1) % verticesCount()].y
        };

        if (sign != vector_a.x * vector_b.y - vector_a.y * vector_b.x >= 0)
            return false;
    }

    return true;
}
