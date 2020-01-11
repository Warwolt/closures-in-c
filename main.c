/**
 *
 */

#include <stdio.h>
#include "closure.h"

/* Type definitions */
DEFINE_CLOSURE_TYPE(int, add, int);

/* Function definitions */
DEFINE_CLOSURE_CONSTRUCTOR_1(add, int);
DEFINE_CLOSURE_BLOCK(int, add, int b)
{
    ENV_ARG_0(int, a);
    return a + b;
}

int main()
{
    CLOSURE(add) c = add(5);
    printf("c(10) = %d\n", CALL_LOCAL_CLOSURE(c, 10));
    FREE_LOCAL_CLOSURE(c);
    return 0;
}
