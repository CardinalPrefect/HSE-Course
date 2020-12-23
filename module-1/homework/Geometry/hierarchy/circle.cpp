#include "circle.h"

Circle::Circle(Point center, double radius) :
        Ellipse(center, center, 2 * radius) {}

Circle::Circle(const Circle& origin) : Ellipse(origin) {
}

Circle& Circle::operator=(const Circle& origin) {
    focuses = origin.focuses;
    focusSum = origin.focusSum;
    return *this;
}

double Circle::radius() {
    return a;
}
