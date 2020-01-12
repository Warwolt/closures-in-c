/**
 * A tiny language experiment, defining closures in C.
 * The reference python-code that is implemented:
 *
 * def add(a):
 *     return (lambda b: a + b)
 *
 * def main():
 *     c = add(5)
 *     print('c(10) =', c(10))
 */

#include <stdio.h>
#include "closure.h"

/* Type definitions */
DEFINE_CLOSURE_TYPE(int, add, int);

/* Function definitions */
DEFINE_CLOSURE_ENVIRONMENT_1(add, int);
DEFINE_CLOSURE_BLOCK(int, add, int b)
{
    ENV_ARG(0, int, a);
    return a + b;
}

int main()
{
    CLOSURE(add) c = add(5);
    printf("c(10) = %d\n", CALL_LOCAL_CLOSURE(c, 10));
    FREE_LOCAL_CLOSURE(c);
    return 0;
}
