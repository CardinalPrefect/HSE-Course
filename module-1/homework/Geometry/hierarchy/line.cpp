#include "line.h"

Line::Line(Point p1, Point p2) : p1(p1), p2(p2) {}

bool operator==(const Line& l1, const Line& l2) {
    return (l1.p1 == l2.p1) and (l1.p2 == l2.p2);
}

bool operator!=(const Line& l1, const Line& l2) {
    return (l1.p1 != l2.p1) or (l1.p2 != l2.p2);
}
