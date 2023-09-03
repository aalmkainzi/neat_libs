#ifndef NEAT_SORT_H
#define NEAT_SORT_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ADD_SORTABLE(type, cmp_function) type*: cmp = (cmp_func) cmp_function

#ifdef SORTABLE_TYPES

#define SORT_TYPES \
ADD_SORTABLE(uint8_t,  neat_uint8_t_cmp), \
ADD_SORTABLE(uint16_t, neat_uint16_t_cmp), \
ADD_SORTABLE(uint32_t, neat_uint32_t_cmp), \
ADD_SORTABLE(uint64_t, neat_uint64_t_cmp), \
ADD_SORTABLE(int8_t,   neat_int8_t_cmp), \
ADD_SORTABLE(int16_t,  neat_int16_t_cmp), \
ADD_SORTABLE(int32_t,  neat_int32_t_cmp), \
ADD_SORTABLE(int64_t,  neat_int64_t_cmp), \
ADD_SORTABLE(char*,    str_cmp), \
SORTABLE_TYPES

#else

#define SORT_TYPES \
ADD_SORTABLE(uint8_t,  neat_uint8_t_cmp), \
ADD_SORTABLE(uint16_t, neat_uint16_t_cmp), \
ADD_SORTABLE(uint32_t, neat_uint32_t_cmp), \
ADD_SORTABLE(uint64_t, neat_uint64_t_cmp), \
ADD_SORTABLE(int8_t,   neat_int8_t_cmp), \
ADD_SORTABLE(int16_t,  neat_int16_t_cmp), \
ADD_SORTABLE(int32_t,  neat_int32_t_cmp), \
ADD_SORTABLE(int64_t,  neat_int64_t_cmp), \
ADD_SORTABLE(char*,    str_cmp) \

#endif

typedef int (*cmp_func)(const void*, const void*);

#define SORT( arr ) do { \
cmp_func cmp; \
typeof(&arr[0]) arr_as_ptr = arr; \
_Generic(arr_as_ptr, \
    SORT_TYPES \
); \
qsort(arr, sizeof(arr) / sizeof(*arr), sizeof(*arr), cmp); \
} while(0)

#define SORT_PTR(arr, n) do { \
cmp_func cmp; \
_Generic(arr, \
    SORT_TYPES \
); \
qsort(arr, n, sizeof(*arr), cmp); \
} while(0)

#define declare_number_cmp_func(type) int neat_##type##_cmp (const type *a, const type *b)
#define define_number_cmp_func(type) declare_number_cmp_func(type) { return (*a > *b) - (*b > *a); }

// compare functions to pass to qsort
#ifdef NEAT_SORT_IMPLEMENTATION
define_number_cmp_func(int8_t);
define_number_cmp_func(uint8_t);
define_number_cmp_func(int16_t);
define_number_cmp_func(uint16_t);
define_number_cmp_func(int32_t);
define_number_cmp_func(uint32_t);
define_number_cmp_func(int64_t);
define_number_cmp_func(uint64_t);
define_number_cmp_func(float);
define_number_cmp_func(double);

int str_cmp(const char **s1, const char **s2)
{
    return strcmp(*s1, *s2);
}

#else
declare_number_cmp_func(int8_t);
declare_number_cmp_func(uint8_t);
declare_number_cmp_func(int16_t);
declare_number_cmp_func(uint16_t);
declare_number_cmp_func(int32_t);
declare_number_cmp_func(uint32_t);
declare_number_cmp_func(int64_t);
declare_number_cmp_func(uint64_t);
declare_number_cmp_func(float);
declare_number_cmp_func(double);
int str_cmp(const char **s1, const char **s2);
#endif

#endif
