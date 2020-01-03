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
