/* Compile:
 *
 *     gcc -O2 -Wall -g circle.c -o circle -lm
 *
 * -g records "debugging information" such as source code line numbers so
 * valgrind (and other debugging tools) can report line numbers.
 *
 * -lm requests the math library because that's where sin and cos come from.
 *
 * Run under valgrind:
 *
 *     valgrind --leak-check=yes ./circle
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct cartesian {
  double x, y;
} cartesian;

typedef struct polar {
  double r, theta;
  /* r should be >= 0 */
} polar;

/* Convert polar coordinates to cartesian coordinates.  The answer is put into a
 * malloc'd record and its pointer returned.  Someone else will have to free it
 * eventually!  (Hint hint.)
 */
cartesian *polar_to_cartesian(polar *p) {
  cartesian *q = malloc(sizeof(cartesian));

  q->x = p->r * cos(p->theta);
  q->y = p->r * sin(p->theta);

  return q;
}

/* Print out a record of cartesian coordinates.
 */
void print_cartesian(cartesian *c) {
  printf("x = %f, y = %f\n", c->x, c->y);
}

/* Compute n points evenly spread out on the circle of radius r centred at the
 * origin.  The points are in polar coordinates.  The points are put into a
 * malloc'd array and its pointer returned.  Someone else will have to free it
 * eventually!  (Hint hint.)
 */
polar *circle_samples(double r, int n)
{
  polar *points = malloc(sizeof(polar));
  int i;

  for (i = 0; i < n; i++) {
    points[i].r = r;
    points[i].theta = 2 * M_PI * i / n;
  }

  return points;
}

int main(void)
{
  polar *polars;
  cartesian *cart;
  int n, i;

  n = 5;
  /* Get n points on the unit circle.  Print their cartesian coordinates. */
  polars = circle_samples(1, n);
  for (i = 0; i < n; i++) {
    cart = polar_to_cartesian(&polars[i]);
    print_cartesian(cart);
    free(cart);
  }
  free(polars);

  return 0;
}



// int x = 2018; (4 bytes)
// char c = x;   (1 byte)

// x =>  2018 (10) -> 0000000 0000000 0000111 11100010 (2)
// c =>  226 (10)  -> 11100010 (2)

//  11   10   9   8   7   6   5   4   3   2   1   0
// ------------------------------------------------
// 2048 1024 512 256 128  64  32  16  8   4   2   1
//
//       1    1   1   1   1   1   0   0   0   1   0



// 2018 - 1024 = 994 - 512 = 482 - 256 = 226 - 128 = 98 - 64 = 34 - 32 = 2 - 2 = 0


