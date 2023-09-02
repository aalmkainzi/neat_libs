#ifndef NEAT_SORT_H
#define NEAT_SORT_H

#include <stdlib.h>

#define ADD_SORT_TYPE(type, function) type*: cmp = (cmp_func) function

// Add your own types in the SORT_TYPES macro below. Use the following format:
// ADD_SORT_TYPE(type, cmp_function)
// cmp_function must be in this format (basically a qsort compare function):
// int cmp(const type*, const type*);

#define SORT_TYPES \
ADD_SORT_TYPE(uchar,              uchar_cmp), \
ADD_SORT_TYPE(unsigned short,     ushort_cmp), \
ADD_SORT_TYPE(unsigned int,       uint_cmp), \
ADD_SORT_TYPE(unsigned long,      ulong_cmp), \
ADD_SORT_TYPE(unsigned long long, ullong_cmp), \
ADD_SORT_TYPE(char,               char_cmp), \
ADD_SORT_TYPE(short,              short_cmp), \
ADD_SORT_TYPE(int,                int_cmp), \
ADD_SORT_TYPE(long,               long_cmp), \
ADD_SORT_TYPE(long long,          llong_cmp), \
ADD_SORT_TYPE(char*,              str_cmp)

typedef int (*cmp_func)(const void*, const void*);

// these typedefs are for multi word types, for easy cmp function creation
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef long long          llong;
typedef unsigned long long ullong;

#define declare_number_cmp_func(type) int type##_cmp(const type* a, const type* b);
#define define_number_cmp_func(type) int type##_cmp(const type* a, const type* b) { return *a - *b; }

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

// compare functions to pass to qsort
declare_number_cmp_func(char);
declare_number_cmp_func(uchar);
declare_number_cmp_func(short);
declare_number_cmp_func(ushort);
declare_number_cmp_func(int);
declare_number_cmp_func(uint);
declare_number_cmp_func(long);
declare_number_cmp_func(ulong);
declare_number_cmp_func(llong);
declare_number_cmp_func(ullong);
declare_number_cmp_func(float);
declare_number_cmp_func(double);
int str_cmp(const char** s1, const char** s2);

#endif
