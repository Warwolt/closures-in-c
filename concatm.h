/**
 * Concatenation Macros
 */
#define __CONCATENATE(a,b) a ## b // generates ab
#define CONCATENATE(a,b) __CONCATENATE(a,b) // expands args a, b
#define __CONCATENATE3(a,b,c) a ## b ## c // generates abc
#define CONCATENATE3(a,b,c) __CONCATENATE3(a,b,c) // expands args a, b, c
