/**
 * Header file defining a set of macros for defining and using closuers.
 *
 * ((detailed description)).
 *
 * Captured arguments are done by shallow copies onto the stack. Because the
 * internals of any data types passed by pointer are unknown to the macros,
 * deep copies of closure arguments cannot be done at the moment.
 *
 * A potential solution to this is to pass a function pointer that knows how
 * to perform the deep copy.
 */

#ifndef CLOSURE_H
#define CLOSURE_H

#include <string.h>
#include <stdlib.h>

/* Concatenation macros */
#define _CONCATENATE3(a,b,c) a ## b ## c // generates abc
#define CONCATENATE3(a,b,c) _CONCATENATE3(a,b,c) // expands args a, b, c
#define PREFIX_WITH_UNDERSCORE(a,b) _CONCATENATE3(_,a,b) // generates _ab
#define JOIN_WITH_UNDERSCORE(a,b) _CONCATENATE3(a,_,b) // generates a_b

/**
 * @brief Defines a block name
 *
 * Takes the name of a defined closure type and defines a text literal
 * to be used for referencing the associated block type of that closure.
 *
 * Creates e.g. '_add_block' from the closure name 'add'
 *
 * @param closure_name  Name of the closure to associate the block with
 */
#define BLOCK_NAME(closure_name) PREFIX_WITH_UNDERSCORE(closure_name,_block)

/**
 * @brief Defines a closure type
 *
 * Defines a closure type consisting of an execution environment with captured
 * arguments and a block type with the defined closure behaviour.
 *
 * The execution environment is implemented with a list of void pointers that
 * will point to the captured arguments to the closure, stored on the heap.
 * The captured variables are accessed using the ENV_ARG() macro.
 *
 * The block is just a function pointer to some function implemented with the
 * same signature as the block type, and should be declared with the associated
 * DEFINE_CLOSURE_BLOCK() macro.
 *
 * @param return_type   The return type of the closure function
 * @param closure_name  Name used to refer to the closure type
 * @param params        Comma separated list of parameters for closure
 */
#define DEFINE_CLOSURE_TYPE(return_type, closure_name, params...) \
return_type BLOCK_NAME(closure_name)(void **env, params); \
struct closure_name \
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
 * Used to make closure environment macros more concise. More description here.
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
    c.block = BLOCK_NAME(closure_name); \
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
    c.block = BLOCK_NAME(closure_name); \
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
    c.block = BLOCK_NAME(closure_name); \
    return c; \
}

/**
 * @brief Closure type name
 *
 * Used for declaring variables, e.g. CLOSURE(my_closure) mc;
 */
#define CLOSURE(name) struct name

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
 * @param c  closure to free
 */
#define FREE_LOCAL_CLOSURE(c) \
for (size_t i = 0; i < c.num_params; i++) \
{ \
    free(c.env[i]); \
} \
free(c.env)

/**
 * @brief Free resources allocated for a closure
 * @param c  pointer to closure to free
 */
#define FREE_CLOSURE(c) \
for (size_t i = 0; i < c->num_params; i++) \
{ \
    free(c->env[i]); \
} \
free(c->env)

/**
 * @brief Macro for declaring a closure block signature
 *
 * detailed description.
 *
 * @param return_type
 * @param closure_name  name of closure type this block belongs to
 * @param args
 */
#define DEFINE_CLOSURE_BLOCK(return_type, closure_name, args...) \
    return_type BLOCK_NAME(closure_name)(void **env, args)

/**
 * @brief Macros for accessing captured arguments in environment
 */
#define ENV_ARG(num, type, name) type name = *(type *)(env[num])

#endif /* CLOSURE_H */
