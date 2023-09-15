/*
    This is single-header library that provides an easy-to-use API
    for sorting and searching arrays.
    
    To use this library, do this in *one* C file:
        #define NEAT_SORT_IMPLEMENTATION
        #include "neat_sort.h"
    
    DOCUMENTATION
        
        By default this library has sorting/searching support for
        strings and all the main numeric types. User types can
        easily be added.
        
        Extending with additonal types:
            #define SORTABLE_TYPES ADD_SORTABLE(S, s_cmp)
            #include "neat_sort.h"
        
        s_cmp must be a function with this prototype:
            
            int s_cmp(const S *a, const S *b);
            
        And it should return:
            if a > b => a positive number
            if a < b => a negative number
            if a = b => 0
        
        Basically a normal qsort compare function.
        
        You can add as many sortable types as you want:
            #define SORTABLE_TYPES \
            ADD_SORTABLE(S, s_cmp), \
            ADD_SORTABLE(S2, s2_cmp), \
            ADD_SORTABLE(S3, s3_cmp)
            
            #include "neat_sort.h"
        
        make sure to only put commas *between* the ADD_SORTABLE calls.
        
        You can also define SORTABLE_TYPES2 and SORTABLE_TYPES3.
        
        From here on out I'll refer to a sortable type by S.
        
        Functions (actually macros):
            
            SORT:
                void SORT(S[]);
                  Sorts the local stack array.
                 
            SORT_PTR:
                void SORT_PTR(S*, size_t n);
                  Sorts the array pointer of length n.
                 
            SORT_DESC:
                void SORT_DESC(S[]);
                  Descendingly sorts the local stack array.
                
            SORT_DESC_PTR:
                void SORT_DESC_PTR(S*, size_t n);
                  Descendingly sorts the array pointer of length n.
                  
            BSEARCH:
                S *BSEARCH(S[], S k);
                  Does binary search to find k in the local array.
                  Returns a pointer to it if found, NULL otherwise.
                  
            BSEARCH_PTR:
                S *BSEARCH_PTR(S*, size_t n, S k);
                  Does binary search to find k in the array pointer
                  of length n.
                  Returns a pointer to it if found, NULL otherwise.
                  
            SEARCH:
                S *SEARCH(S[], S k);
                  Does linear search to find k in the local array.
                  Returns a pointer to it if found, NULL otherwise.
                  
            SEARCH_PTR:
                S *SEARCH(S*, size_t n, S k);
                  Does linear search to find k in the array pointer
                  of length n.
                  Returns a pointer to it if found, NULL otherwise.
                  
            GET_CMP:
                int(*)(S*,S*) GET_CMP(S);
                  Returns the compare function associated with the
                  type.
*/

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

#if defined(SORTABLE_TYPES) && defined(SORTABLE_TYPES2) && defined(SORTABLE_TYPES3)
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES, \
    SORTABLE_TYPES2, \
    SORTABLE_TYPES3

#elif defined(SORTABLE_TYPES) && defined(SORTABLE_TYPES2)
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES, \
    SORTABLE_TYPES2
    
#elif defined(SORTABLE_TYPES) && defined(SORTABLE_TYPES3)
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES, \
    SORTABLE_TYPES3
    
#elif defined(SORTABLE_TYPES)
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES
    
#elif defined(SORTABLE_TYPES2) && defined(SORTABLE_TYPES3) // by now SORTABLE_TYPES is out of the picture, user didnt define it.
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES2, \
    SORTABLE_TYPES3
    
#elif defined(SORTABLE_TYPES2)
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES2
    
#elif defined(SORTABLE_TYPES3)
    
    #define NEAT_USER_SORTABLE_TYPES \
    , \
    SORTABLE_TYPES3
    
#else // user didn't define any sortable type
    
    #define NEAT_USER_SORTABLE_TYPES
    
#endif // User's SORTABLE_TYPEs

#define ALL_SORTABLE_TYPES \
DEFAULT_SORTABLE_TYPES \
NEAT_USER_SORTABLE_TYPES

typedef int (*cmp_func)(const void*, const void*);

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

#define GET_CMP(type) ((cmp_func) _Generic((typeof(type)){0}, ALL_SORTABLE_TYPES))

#define SORT_PTR(arr, n) qsort(arr, n, sizeof(*arr), GET_CMP(*arr))

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

#define BSEARCH_PTR(arr, n, key)  (typeof(*arr)*) bsearch (&(typeof(*arr)[]){key}[0], arr, n, sizeof(*arr), GET_CMP(*arr)) \

#define BSEARCH(arr, key) BSEARCH_PTR(arr, ARR_LEN(arr), key)

#define SEARCH_PTR(arr, n, key) (typeof(*arr)*) neat_search(&(typeof(*arr)[]){key}[0], arr, n, sizeof(*arr), GET_CMP(*arr))

#define SEARCH(arr, key) SEARCH_PTR(arr, ARR_LEN(arr), key)

#define declare_number_cmp_func(type) int neat_##type##_cmp (const type *a, const type *b)
#define define_number_cmp_func(type) declare_number_cmp_func(type) { return (*a > *b) - (*b > *a); }

#endif // NEAT_SORT_H

#ifdef NEAT_SORT_IMPLEMENTATION
// default compare functions to pass to qsort/bsearch
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