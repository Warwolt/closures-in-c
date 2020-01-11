/**
 *
 */

#include <stdio.h>
#include "closure.h"

/* Type definitions */
DEFINE_CLOSURE_TYPE(int, add, int);
DEFINE_CLOSURE_TYPE(int, add3, int);

/* Function declarations */
void closure_client(CLOSURE(add3) *c);
CLOSURE_BLOCK(int, add_block, int b);
CLOSURE_BLOCK(int, add3_block, int c);

/* Function definitions */
DEFINE_CLOSURE_CONSTRUCTOR_1(add_block, add, int);
CLOSURE_BLOCK(int, add_block, int b)
{
    ENV_ARG_0(int, a);
    return a + b;
}

DEFINE_CLOSURE_CONSTRUCTOR_2(add3_block, add3, int, float);
CLOSURE_BLOCK(int, add3_block, int c)
{
    ENV_ARG_0(int, a);
    ENV_ARG_1(float, b);
    return a + b + c;
}

int main()
{
    CLOSURE(add3) c = add3(5, 7);
    closure_client(&c);
    return 0;
}

void closure_client(CLOSURE(add3) *c)
{
    printf("c(10) = %d\n", CALL_CLOSURE(c, 10));
    FREE_CLOSURE(c);
}
