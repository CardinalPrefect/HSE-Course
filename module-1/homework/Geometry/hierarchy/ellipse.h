#include <utility>
#include <cmath>
#include "mymath.h"
#include "shape.h"

#ifndef HEIRARCHY_ELLIPSE_H
#define HEIRARCHY_ELLIPSE_H

class Ellipse : public Shape {
public:
    Ellipse(Point focus1, Point focus2, double focusSum);

    Ellipse(const Ellipse& origin);

    Ellipse& operator=(const Ellipse& origin);

    ~Ellipse() = default;

    double area() const override;

    double perimeter() const override;

    bool operator==(const Shape& another) const override;

    bool isCongruentTo(const Shape& another) const override;

    bool isSimilarTo(const Shape& another) const override;

    bool containsPoint(Point point) const override;

    void rotate(Point center, double angle) override;

    void reflex(Point center) override;

    void reflex(Line axis) override;

    void scale(Point center, double coefficient) override;

    std::pair<Point, Point> getFocuses();
    double eccentricity() const;
    Point center() const;

protected:
    std::pair<Point, Point> focuses;
    double focusSum;
    double a;
    double b;
    double c;
};

#endif //HEIRARCHY_ELLIPSE_H
