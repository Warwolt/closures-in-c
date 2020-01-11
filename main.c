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
DEFINE_CLOSURE_ENVIRONMENT_3(add, int, int, int);
DEFINE_CLOSURE_BLOCK(int, add, int d)
{
    ENV_ARG(0, int, a);
    ENV_ARG(1, int, b);
    ENV_ARG(2, int, c);
    return a + b + c + d;
}

int main()
{
    CLOSURE(add) c = add(1, 5, 10);
    printf("c(15) = %d\n", CALL_LOCAL_CLOSURE(c, 15));
    FREE_LOCAL_CLOSURE(c);
    return 0;
}
