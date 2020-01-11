/**
 *
 */
#include <string.h>
#include <stdlib.h>

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
 * @brief
 *
 * detailed description.
 *
 * @param return_type
 * @param closure_name
 * @param args
 */
#define DEFINE_CLOSURE_TYPE(return_type, closure_name, args...) \
struct closure_name \
{ \
    size_t num_args; \
    void **env; \
    return_type(*block)(void **env, args); \
}

/**
 * @brief
 *
 * detailed description.
 *
 * @param block_name
 * @param const_name
 * @param param0
 */
#define DEFINE_CLOSURE_CONSTRUCTOR_1(block_name, const_name, param0) \
CLOSURE(const_name) const_name(param0 arg0) \
{ \
    CLOSURE(const_name) c; \
    /* Create environment and capture arguments */ \
    c.num_args = 1; \
    c.env = malloc(sizeof(void *) * 1); \
    c.env[0] = malloc(sizeof(arg0)); \
    memcpy(c.env[0], &arg0, sizeof(arg0)); \
    /* Set function block */ \
    c.block = block_name; \
    return c; \
}

/**
 * @brief
 *
 * detailed description.
 *
 * @param block_name
 * @param const_name
 * @param param0
 */
#define DEFINE_CLOSURE_CONSTRUCTOR_2(block_name, const_name, param0, param1) \
CLOSURE(const_name) const_name(param0 arg0, param1 arg1) \
{ \
    CLOSURE(const_name) c; \
    /* Create environment and capture arguments */ \
    c.num_args = 2; \
    c.env = malloc(sizeof(void *) * 2); \
    c.env[0] = malloc(sizeof(arg0)); \
    memcpy(c.env[0], &arg0, sizeof(arg0)); \
    c.env[1] = malloc(sizeof(arg1)); \
    memcpy(c.env[1], &arg1, sizeof(arg1)); \
    /* Set function block */ \
    c.block = block_name; \
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
 * @param c  closure to free
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
 * @param block_name
 * @param args
 */
#define CLOSURE_BLOCK(return_type, block_name, args...) \
    return_type block_name(void **env, args)

/**
 * @brief Macros for accesing captured arguments
 */
#define ENV_ARG_0(type, name) type name = *(type *)(env[0])
#define ENV_ARG_1(type, name) type name = *(type *)(env[1])
