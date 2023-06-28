#include <stdio.h>
#include "point.h"

ssize_t save_point_array(const char *pathname, size_t n, const point *a)
{
    FILE *f = fopen(pathname, "w");

    if (f == NULL) {
        fprintf(stderr, "failed to open file.");
        return -1;
    }

    int num_written = fwrite(a, sizeof(point), n, f);
    fclose(f);
    return num_written;

}

ssize_t load_point_array(const char *pathname, size_t n, point *a)
{
    FILE *f = fopen(pathname, "r");

    if (f == NULL) {
        fprintf(stderr, "failed to open file.");
        return -1;
    }

    int num_read = fread(a, sizeof(point), n, f);
    fclose(f);
    return num_read;
}
