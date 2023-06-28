#ifndef POINT_H
#define POINT_H

#include <stddef.h>
// for size_t, ssize_t

typedef struct point {
  int x, y;
} point;

ssize_t save_point_array(const char *pathname, size_t n, const point *a);

ssize_t load_point_array(const char *pathname, size_t n, point *a);

#endif  // #ifndef POINT_H
