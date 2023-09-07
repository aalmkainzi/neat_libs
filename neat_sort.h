#ifndef NEAT_SORT_H
#define NEAT_SORT_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ADD_SORTABLE(type, cmp_function) type: cmp_function

#define DEFAULT_SORTABLE_TYPES \
ADD_SORTABLE(uint8_t,  neat_uint8_t_cmp), \
ADD_SORTABLE(uint16_t, neat_uint16_t_cmp), \
ADD_SORTABLE(uint32_t, neat_uint32_t_cmp), \
ADD_SORTABLE(uint64_t, neat_uint64_t_cmp), \
ADD_SORTABLE(int8_t,   neat_int8_t_cmp), \
ADD_SORTABLE(int16_t,  neat_int16_t_cmp), \
ADD_SORTABLE(int32_t,  neat_int32_t_cmp), \
ADD_SORTABLE(int64_t,  neat_int64_t_cmp), \
ADD_SORTABLE(float,    neat_float_cmp), \
ADD_SORTABLE(double,   neat_double_cmp), \
ADD_SORTABLE(char*,    neat_str_cmp)

#ifdef SORTABLE_TYPES
    
    #define ALL_SORTABLE_TYPES \
    DEFAULT_SORTABLE_TYPES, \
    SORTABLE_TYPES
    
#else // SORTABLE_TYPES
    
    #define ALL_SORTABLE_TYPES \
    DEFAULT_SORTABLE_TYPES
    
#endif // SORTABLE_TYPES

typedef int (*cmp_func)(const void*, const void*);

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

#define GET_CMP_FOR(type) ((cmp_func) _Generic(type, ALL_SORTABLE_TYPES))

#define SORT_PTR(arr, n) qsort(arr, n, sizeof(*arr), GET_CMP_FOR(*arr))

#define SORT(arr) SORT_PTR(arr, ARR_LEN(arr))

#define REVERSE_ARRAY_PTR(arr, n) do { \
for(int neat_iter = 0; neat_iter < n/2; neat_iter++) { \
    typeof(*arr) neat_temp = arr[neat_iter]; \
    arr[neat_iter] = arr[n - neat_iter -1]; \
    arr[n - 1 - neat_iter] = neat_temp; \
} \
} while(0)

#define REVERSE_ARRAY(arr) REVERSE_ARRAY_PTR(arr, ARR_LEN(arr))

#define SORT_DESC_PTR(arr, n) do { \
SORT_PTR(arr, n); \
REVERSE_ARRAY_PTR(arr, n); \
} while(0)

#define SORT_DESC(arr) SORT_DESC_PTR(arr, ARR_LEN(arr))

#define BSEARCH_PTR(arr, n, key) bsearch (&(typeof(*arr)[]){key}[0], arr, n, sizeof(*arr), GET_CMP_FOR(*arr)) \

#define BSEARCH(arr, key) BSEARCH_PTR(arr, ARR_LEN(arr), key)

#define SEARCH_PTR(arr, n, key) neat_search(&(typeof(*arr)[]){key}[0], arr, n, sizeof(*arr), GET_CMP_FOR(*arr))

#define SEARCH(arr, key) SEARCH_PTR(arr, ARR_LEN(arr), key)

#define declare_number_cmp_func(type) int neat_##type##_cmp (const type *a, const type *b)
#define define_number_cmp_func(type) declare_number_cmp_func(type) { return (*a > *b) - (*b > *a); }

// default compare functions to pass to qsort/bsearch
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
    
    int neat_str_cmp(const char **s1, const char **s2)
    {
        return strcmp(*s1, *s2);
    }
    
    void *neat_search(const void *key, const void *base, size_t nmemb, size_t elm_size, cmp_func cmp)
    {
        const uint8_t *arr = (const uint8_t *) base;
        for(size_t i = 0 ; i < nmemb ; i++)
        {
            const uint8_t *elm = arr + i * elm_size;
            if(cmp(key, elm) == 0) return elm;
        }
        return NULL;
    }
    
#else // NEAT_SORT_IMPLEMENTATION
    
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
    int neat_str_cmp(const char **s1, const char **s2);
    
    void *neat_search(const void *key, const void *base, size_t nmemb, size_t elm_size, cmp_func cmp);
    
#endif // NEAT_SORT_IMPLEMENTATION

#endif // NEAT_SORT_H