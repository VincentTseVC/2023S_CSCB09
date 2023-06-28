#ifndef _CIRCLE_H
#define _CIRCLE_H

typedef struct circle {
    double radius;
    double cx, cy;
} circle;

double circle_area(const circle *c);

#endif

