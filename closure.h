/**
 *
 */
#include <string.h>
#include <stdlib.h>

/**
 * @brief Concatenation macros
 */
#define _CONCATENATE3(a,b,c) a ## b ## c // generates abc
#define CONCATENATE3(a,b,c) _CONCATENATE3(a,b,c) // expands args a, b, c
#define PREFIX_WITH_UNDERSCORE(a,b) _CONCATENATE3(_,a,b) // generates _ab
#define JOIN_WITH_UNDERSCORE(a,b) _CONCATENATE3(a,_,b) // generates a_b
#define BLOCK_NAME(name) PREFIX_WITH_UNDERSCORE(name,block)

/**
 * @brief Closure type macro
 *
 * Used for declaring variables, e.g. CLOSURE(my_closure) mc;
 */
#define CLOSURE(name) struct name

/**
 * @brief Macro for calling closure via pointer
 *
 * @param closure  closure to call
 * @param args     function arguments
 */
#define CALL_CLOSURE(closure, args...) closure->block(closure->env, args)

/**
 * @brief Macro for calling local closure
 *
 * @param closure  closure to call
 * @param args     function arguments
 */
#define CALL_LOCAL_CLOSURE(closure, args...) closure.block(closure.env, args)

/**
 * @brief Declares the closure block and defines the closure struct.
 *
 * Detailed description.
 *
 * @param return_type
 * @param closure_name
 * @param args
 */
#define DEFINE_CLOSURE_TYPE(return_type, closure_name, args...) \
return_type BLOCK_NAME(closure_name)(void **env, args); \
struct closure_name \
{ \
    size_t num_args; \
    void **env; \
    return_type(*block)(void **env, args); \
}

/**
 * @brief Defines a function returning a closure
 *
 * detailed description.
 *
 * @param closure_name
 * @param param0
 */
#define DEFINE_CLOSURE_CONSTRUCTOR_1(closure_name, param0) \
CLOSURE(closure_name) closure_name(param0 arg0) \
{ \
    CLOSURE(closure_name) c; \
    /* Create environment and capture arguments */ \
    c.num_args = 1; \
    c.env = malloc(sizeof(void *) * 1); \
    c.env[0] = malloc(sizeof(arg0)); \
    memcpy(c.env[0], &arg0, sizeof(arg0)); \
    /* Set function block */ \
    c.block = BLOCK_NAME(closure_name); \
    return c; \
}

/**
 * @brief Free resources allocated for a local closure
 * @param c  closure to free
 */
#define FREE_LOCAL_CLOSURE(c) \
for (size_t i = 0; i < c.num_args; i++) \
{ \
    free(c.env[i]); \
} \
free(c.env)

/**
 * @brief Free resources allocated for a closure
 * @param c  pointer to closure to free
 */
#define FREE_CLOSURE(c) \
for (size_t i = 0; i < c->num_args; i++) \
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
#define ENV_ARG_0(type, name) type name = *(type *)(env[0])
#define ENV_ARG_1(type, name) type name = *(type *)(env[1])
