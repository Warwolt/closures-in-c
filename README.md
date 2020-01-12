# C Closures
Single header implementation of closures in C

## Introduction to closures
A closure is an anonymous function that has access to values in the scope of
the function that creates it. It can be thought of has a function that is
called in two steps, the first call binds some of the parameters to values
and returns a new function, and the second call binds the remaining
parameters and produces a result.

Closures can applied anywhere you would normally use a callback function,
such as event-driven applications like a GUI, but where you might want to
have some state in the function prior to passing it to the caller.

## Closure implementation in C
Because C is a statically typed language, and functions have types, before
a closure can be instantiated it must first be defined as a type. Here, the
closures consist of an 'environment' and a 'block'. The environment contains
any captured arguments, and the block is a function that defines the
behaviour of the closure.

The closure itself is a struct with the environment as a list of void
pointers and the block as a function pointer. The block function takes the
void pointer list and some other types as parameters. To call the closure,
the block function is passed the environment and any other arguments.

A capture function takes the first set of arguments and captures them by
copying them onto the heap, and assigns them to a closure struct along with
the block function, and then returns the instantiated closure to the caller.

## Defining and using a closure with closure.h
A closure type is defined with this header using the `DEFINE_CLOSURE_TYPE()`
macro, that creates the block type (a function signature) and the struct
that stores the block and the environment.

The capture function will have the name of the closure, e.g. `add()`, and is
defined along with the closure environment with the group of macros
`DEFINE_CLOSURE_ENVIRONMENT_N` (N is some whole number).

The behaviour of the closure is defined with a block function, that is
declared with the `DEFINE_CLOSURE_BLOCK()` macro, where captured arguments
can be accessed in the function body using the `ENV_ARG()` macro.

Instantiated closures are stored in variables of type `CLOSURE(name)`, and
can be called using the `CALL_CLOSURE()` macro. When a closure instance is no
longer in use, it should be claned up with the `FREE_CLOSURE()` macro.

## Example usage
```C
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
```
