#include "utils.h"

/**
 * Checks if a <= x < b
 * @param x
 * @param a
 * @param b
 * @return a <= x < b
 */
int in_range(int x, int a, int b) {
    return x >= a && x < b;
}

/**
 * Checks if a <= x < b
 * @param x
 * @param a
 * @param b
 * @return a <= x < b
 */
int float_in_range(float x, float a, float b) {
    return x >= a && x <= b;
}

/**
 * Returns length of number in some base
 * @param x Number
 * @param base Base
 * @return
 */
int num_length(int x, int base) {
    int result = 0;

    while (x > 0) {
        result++;
        x /= base;
    }

    return result;
}

/**
 * Prints a certain string for several times
 * @param s
 * @param times
 */
void print_repeated(char* s, int times) {
    int i;

    for (i = 0; i < times; i++) {
        printf("%s", s);
    }
}
