#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#define TRUE (1)
#define FALSE (0)
#define ERROR (-1)
#define SUCCESS (0)

#ifndef NULL
#define NULL (0)
#endif

#define EXIT_ON_ERROR(code) \
    fprintf(stderr, "Error: %s has failed\n", code); \
    exit(1);

#define IS_EOF (!ferror(stdin) && !ferror(stdout) && feof(stdin))

int in_range(int x, int a, int b);

#endif
