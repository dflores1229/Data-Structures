#include "Swap.h"

/**
 * Swap the values of integers a and b (in-place)
 */
void swap(int & a, int & b) {
    int temp = a;
    a = b;
    b = temp;
}
