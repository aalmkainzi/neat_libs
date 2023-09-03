#ifndef NEAT_SORT_H
#define NEAT_SORT_H

#include <stdlib.h>
#include <string.h>

#define ADD_SORT_TYPE(type, cmp_function) type*: cmp = (cmp_func) cmp_function

#ifdef ADDITIONAL_SORT_TYPES

#define SORT_TYPES \
ADD_SORT_TYPE(unsigned char,      uchar_cmp), \
ADD_SORT_TYPE(unsigned short,     ushort_cmp), \
ADD_SORT_TYPE(unsigned int,       uint_cmp), \
ADD_SORT_TYPE(unsigned long,      ulong_cmp), \
ADD_SORT_TYPE(unsigned long long, ullong_cmp), \
ADD_SORT_TYPE(char,               char_cmp), \
ADD_SORT_TYPE(short,              short_cmp), \
ADD_SORT_TYPE(int,                int_cmp), \
ADD_SORT_TYPE(long,               long_cmp), \
ADD_SORT_TYPE(long long,          llong_cmp), \
ADD_SORT_TYPE(char*,              str_cmp), \
ADDITIONAL_SORT_TYPES

#else

#define SORT_TYPES \
ADD_SORT_TYPE(unsigned char,      uchar_cmp), \
ADD_SORT_TYPE(unsigned short,     ushort_cmp), \
ADD_SORT_TYPE(unsigned int,       uint_cmp), \
ADD_SORT_TYPE(unsigned long,      ulong_cmp), \
ADD_SORT_TYPE(unsigned long long, ullong_cmp), \
ADD_SORT_TYPE(char,               char_cmp), \
ADD_SORT_TYPE(short,              short_cmp), \
ADD_SORT_TYPE(int,                int_cmp), \
ADD_SORT_TYPE(long,               long_cmp), \
ADD_SORT_TYPE(long long,          llong_cmp), \
ADD_SORT_TYPE(char*,              str_cmp) \

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


#define declare_number_cmp_func(type) int type##_cmp(const type *a, const type *b)
#define define_number_cmp_func(type) declare_number_cmp_func { return (*a > *b) - (*b > *a); }

// these typedefs are for multi word types, for easy cmp function creation
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef long long          llong;
typedef unsigned long long ullong;

// compare functions to pass to qsort
#ifdef NEAT_SORT_IMPLEMENTATION
define_number_cmp_func(char);
define_number_cmp_func(uchar);
define_number_cmp_func(short);
define_number_cmp_func(ushort);
define_number_cmp_func(int);
define_number_cmp_func(uint);
define_number_cmp_func(long);
define_number_cmp_func(ulong);
define_number_cmp_func(llong);
define_number_cmp_func(ullong);
define_number_cmp_func(float);
define_number_cmp_func(double);

int str_cmp(const char **s1, const char **s2)
{
    return strcmp(*s1, *s2);
}

#else
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
int str_cmp(const char **s1, const char **s2);
#endif

#endif
