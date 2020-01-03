#ifndef UTILS_H
#define UTILS_H

#define EXIT_ON_ERROR(code) \
    fprintf(stderr, "Error: %s has failed\n", code); \
    exit(1);

int in_range(int x, int a, int b);

#endif
