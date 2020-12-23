#include "circle.h"
#include "polygon.h"
#include "point.h"
#include <cmath>

#ifndef HIERARCHY_TRIANGLE_H
#define HIERARCHY_TRIANGLE_H

class Triangle : public Polygon {
public:
    Triangle() = delete;

    Triangle(Point x, Point y, Point z);

    Triangle(const Triangle& origin);

    Triangle& operator=(const Triangle& origin);

    ~Triangle() = default;


    Circle circumscribedCircle();

    Circle inscribedCircle();
};

#endif //HIERARCHY_TRIANGLE_H

