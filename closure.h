/**
 * @author: Rasmus K. Källqvist
 * @date:   2020-01-12
 * @brief:  Header for defining and using closures in C.
 *
 * See README.md for instructions on how to use this file.
 * Example usage:
 *
 * DEFINE_CLOSURE_TYPE(int, add, int);
 * DEFINE_CLOSURE_ENVIRONMENT_1(add, int);
 * DEFINE_CLOSURE_BLOCK(int, add, int b)
 * {
 *     ENV_ARG(0, int, a);
 *     return a + b;
 * }
 *
 * int main()
 * {
 *     CLOSURE(add) c = add(5);
 *     printf("c(10) = %d\n", CALL_LOCAL_CLOSURE(c, 10));
 *     FREE_LOCAL_CLOSURE(c);
 *     return 0;
 * }
 */

#ifndef CLOSURE_H
#define CLOSURE_H

#include <string.h>
#include <stdlib.h>


/* Concatenation macros ------------------------------------------------------*/
/* Concatenate */
#define _CONCATENATE(a,b) a ## b // generates ab
#define CONCATENATE(a,b) _CONCATENATE(a,b) // expands args a, b
/* Concatenate 3 */
#define _CONCATENATE3(a,b,c) a ## b ## c // generates abc
#define CONCATENATE3(a,b,c) _CONCATENATE3(a,b,c) // expands args a, b, c


/* Closure definition macros -------------------------------------------------*/
/**
 * @brief Inserts the closure type name
 *
 * Used for refering to the closure type name, e.g. when declaring the return
 * type of a function or declaring variables of the closure type.
 *
 * Creates e.g. 'struct _add' from the closure name 'add'
 *
 * @param closure_name  Name of the closure to refer to
 */
#define CLOSURE(closure_name) struct CONCATENATE(_, closure_name)

/**
 * @brief Inserts a block type name
 *
 * Takes the name of a defined closure type and defines a text literal
 * to be used for referencing the associated block type of that closure.
 *
 * Creates e.g. '_add_block' from the closure name 'add'
 *
 * @param closure_name  Name of the closure to associate the block with
 */
#define BLOCK(closure_name) CONCATENATE3(_,closure_name,_block)

/**
 * @brief Defines a closure type
 *
 * Defines a closure type consisting of an execution environment with captured
 * arguments and a block type with the defined closure behaviour.
 *
 * The execution environment consists of an array of void pointers, pointing to
 * arguments that are copied onto the heap when the capture function is called.
 * The captured variables are accessed using the ENV_ARG() macro.
 *
 * The block is just a pointer to some function implemented with the same
 * signature as the block type, and should be declared with the associated
 * DEFINE_CLOSURE_BLOCK() macro.
 *
 * @param return_type   The return type of the closure function
 * @param closure_name  Name used to refer to the closure type
 * @param params        Comma separated list of parameters for closure
 */
#define DEFINE_CLOSURE_TYPE(return_type, closure_name, params...) \
return_type BLOCK(closure_name)(void **env, params); \
CLOSURE(closure_name) \
{ \
    size_t num_params; \
    void **env; \
    return_type(*block)(void **env, params); \
}

/**
 * @brief Creates the closure environment
 *
 * Creates an environment on the heap with room for storing arguments passed
 * to the function that instantiates a given closure.
 *
 * @param capacity  Number of arguments that should be capturable
 */
#define CREATE_ENVIRONMENT(capacity) \
c.num_params = capacity; \
c.env = malloc(sizeof(void *) * c.num_params)

/**
 * @brief Macro for argument capturing code
 *
 * Allocates space for a copy of the given argument and assigns the address of
 * that to the N:th pointer in the environment array, and then performs a
 * (shallow) copy of the argument onto that allocated space.
 *
 * @param num  Which N:th argument to capture
 * @param arg  Argument to capture by copying onto the heap
 */
#define CAPTURE_ARGUMENT(num, arg) \
c.env[num] = malloc(sizeof(arg)); \
memcpy(c.env[num], &arg, sizeof(arg))

/**
 * @brief Defines a function that captures closure arguments
 *
 * Defines the function that instantiates the closure type with an environment
 * consisting of one captured argument.
 *
 * @param closure_name  Name used to refer to the closure type
 * @param param0        Type name of argument to capture
 *
 * @return An instance of the closure struct with its environment and block
 */
#define DEFINE_CLOSURE_ENVIRONMENT_1(closure_name, param0) \
CLOSURE(closure_name) closure_name(param0 arg0) \
{ \
    /* Instantiate closure struct */ \
    CLOSURE(closure_name) c; \
    /* Create environment and capture arguments */ \
    CREATE_ENVIRONMENT(1); \
    CAPTURE_ARGUMENT(0, arg0); \
    /* Set callback function then return closure */ \
    c.block = BLOCK(closure_name); \
    return c; \
}

/**
 * @brief 2 argument version of DEFINE_CLOSURE_ENVIRONMENT_1()
 */
#define DEFINE_CLOSURE_ENVIRONMENT_2(closure_name, param0, param1) \
CLOSURE(closure_name) closure_name(param0 arg0, param1 arg1) \
{ \
    /* Instantiate closure struct */ \
    CLOSURE(closure_name) c; \
    /* Create environment and capture arguments */ \
    CREATE_ENVIRONMENT(2); \
    CAPTURE_ARGUMENT(0, arg0); \
    CAPTURE_ARGUMENT(1, arg1); \
    /* Set callback function then return closure */ \
    c.block = BLOCK(closure_name); \
    return c; \
}

/**
 * @brief 3 argument version of DEFINE_CLOSURE_ENVIRONMENT_1()
 */
#define DEFINE_CLOSURE_ENVIRONMENT_3(closure_name, param0, param1, param2) \
CLOSURE(closure_name) closure_name(param0 arg0, param1 arg1, param2 arg2) \
{ \
    /* Instantiate closure struct */ \
    CLOSURE(closure_name) c; \
    /* Create environment and capture arguments */ \
    CREATE_ENVIRONMENT(3); \
    CAPTURE_ARGUMENT(0, arg0); \
    CAPTURE_ARGUMENT(1, arg1); \
    CAPTURE_ARGUMENT(2, arg2); \
    /* Set callback function then return closure */ \
    c.block = BLOCK(closure_name); \
    return c; \
}

/**
 * @brief Declare the signature of a closure block
 *
 * Macro for inserting the signature of a closure block, used when refering to
 * the block type associated with a given closure, and when defining a function
 * of the block type that should be used as the closure behaviour.
 *
 * @param return_type   Type that the block return
 * @param closure_name  Name of closure type this block belongs to
 * @param params        Parameter list for the block
 */
#define DEFINE_CLOSURE_BLOCK(return_type, closure_name, params...) \
    return_type BLOCK(closure_name)(void **env, params)

/**
 * @brief Macro for accessing captured arguments in closure environment
 *
 * e.g. ENV_ARG(0, int, a); // retreives 1:th arg to an int variable 'a'
 *      ENV_ARG(1, float, b); // retreives 2:nd arg to a float variable 'b'
 *
 * @param num   Which N:th captured argument to access
 * @param type  Type of the arg (should be same as given in env declaration)
 * @param name  Name of variable being declared
 */
#define ENV_ARG(num, type, name) type name = *(type *)(env[num])


/* Closure usage macros ------------------------------------------------------*/
/**
 * @brief Macro for calling closure via pointer
 *
 * @param closure  Pointer to the closure to call
 * @param args     Non-captured function arguments to pass to closure
 */
#define CALL_CLOSURE(closure, args...) closure->block(closure->env, args)

/**
 * @brief Macro for calling local closure
 *
 * @param closure  Closure to call
 * @param args     Non-captured function arguments to pass to closure
 */
#define CALL_LOCAL_CLOSURE(closure, args...) closure.block(closure.env, args)

/**
 * @brief Free resources allocated for a local closure
 * @param c  Closure to free
 */
#define FREE_LOCAL_CLOSURE(c) \
for (size_t i = 0; i < c.num_params; i++) \
{ \
    free(c.env[i]); \
} \
free(c.env)

/**
 * @brief Free resources allocated for a closure
 * @param c  Pointer to closure to free
 */
#define FREE_CLOSURE(c) \
for (size_t i = 0; i < c->num_params; i++) \
{ \
    free(c->env[i]); \
} \
free(c->env)

#endif /* CLOSURE_H */
