/*
    This is header-only library that provides an easy-to-use API
    for parsing from strings and converting to strings.
    
    To use this library, do this in *one* C file before including:
        #define NEAT_TOSTR_IMPLEMENTATION
    
    DOCUMENTATION
        
        By default this library has parsing/to_string support
        for all integer types, char, bool, and string.
        User types can easily be added.
        
        Extending 'to_string' with additonal types:
            #define STRINGABLE_TYPES ADD_STRINGABLE(T, T2str)
            #include "neat_tostr.h"
        
        T2str must be a function with this prototype:
            
            char *T2str(T*);
            
        And it should return a 'malloc'ed string.
        
        Extending 'parse' with additonal types:
            #define PARSABLE_TYPES ADD_PARSABLE(T, parseT)
            #include "neat_tostr.h"
        
        parseT must be a function with this prototype:
        
            T parseT(char *str, int *err);
        
        You can add as many stringable and parsable types 
        as you want:
            #define STRINGABLE_TYPES \
            ADD_STRINGABLE(A, a2str), \
            ADD_STRINGABLE(B, b2str), \
            ADD_STRINGABLE(C, c3str)
            
            #define PARSABLE_TYPES \
            ADD_PARSABLE(A, parse_a), \
            ADD_PARSABLE(B, parse_b), \
            ADD_PARSABLE(C, parse_c)
            
            #include "neat_tostr.h"
        
        make sure to only put commas *between* the ADD_ calls.
        
        You can also define STRINGABLE_TYPES2 and STRINGABLE_TYPES3.
        And PARSABLE_TYPES2 and PARSABLE_TYPES3.
        
        From here on out I'll refer to a stringable and parsable type by S.
        
        Functions (actually macros):
            
            to_string:
                char* to_string(S s);
                  Returns a 'malloc'ed string representation of s.
                 
            array_to_string:
                char* array_to_string(S* s, int n);
                  Returns a 'malloc'ed string from the array s 
                  of size n,
                
            print:
                void print(S s);
                  Prints to stdout the string representation of s.
                  
            fprint:
                void fprint(FILE* f, S s);
                  Prints to f the string representation of s.
                
            println:
                void println(S s);
                  Prints to stdout the string representation of s,
                  with '\n' after it.
                  
            fprintln:
                void fprintln(FILE* f, S s);
                  Prints to f the string representation of s,
                  with '\n' after it.
                
            print_array:
                void print_array(S* s, int n);
                  Prints to stdout the string representation of the
                  array s with length n.
                  
            fprint_array:
                void print_array(FILE* f, S* s, int n);
                  Prints to f the string representation of the
                  array s with length n.
                  
            println_array:
                void println_array(S* s, int n);
                  Prints to stdout the string representation of the
                  array s with length n, with '\n' after it.
                  
            fprintln_array:
                void fprintln_array(FILE* f, S* s, int n);
                  Prints to f the string representation of the
                  array s with length n, with '\n' after it.
                  
            parse:
                S parse(S, char *str);
                  Parses S from str and returns it.
                
                S parse(S, char *str, int *err);
                  Parses S from str and returns it.
                  Puts error code into err.
                  
        If you want the API to be prefixed,
        then define NEAT_TOSTR_PREFIX before including.
        All the functions (macros) will be prefixed with 'neat_'
*/

#ifndef NEAT_TOSTR_H
#define NEAT_TOSTR_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>

#define NEAT_ADD_STRINGABLE(type, tostr) \
type: tostr

#define NEAT_ADD_PARSABLE(type, parse) \
type: parse


#if defined(STRINGABLE_TYPES) && defined(STRINGABLE_TYPES2) && defined(STRINGABLE_TYPES3)

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES, \
    STRINGABLE_TYPES2, \
    STRINGABLE_TYPES3

#elif defined(STRINGABLE_TYPES) && defined(STRINGABLE_TYPES2)

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES, \
    STRINGABLE_TYPES2

#elif defined(STRINGABLE_TYPES) && defined(STRINGABLE_TYPES3)

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES, \
    STRINGABLE_TYPES3

#elif defined(STRINGABLE_TYPES)

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES

#elif defined(STRINGABLE_TYPES2) && defined(STRINGABLE_TYPES3) // by now STRINGABLE_TYPES is out of the picture, user didnt define it.

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES2, \
    STRINGABLE_TYPES3

#elif defined(STRINGABLE_TYPES2)

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES2

#elif defined(STRINGABLE_TYPES3)

    #define NEAT_USER_STRINGABLE_TYPES \
    , \
    STRINGABLE_TYPES3

#else // user didn't define any stringable type

    #define NEAT_USER_STRINGABLE_TYPES

#endif // User's STRINGABLE_TYPES


#if defined(PARSABLE_TYPES) && defined(PARSABLE_TYPES2) && defined(PARSABLE_TYPES3)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES, \
    PARSABLE_TYPES2, \
    PARSABLE_TYPES3

#elif defined(PARSABLE_TYPES) && defined(PARSABLE_TYPES2)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES, \
    PARSABLE_TYPES2

#elif defined(PARSABLE_TYPES) && defined(PARSABLE_TYPES3)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES, \
    PARSABLE_TYPES3

#elif defined(PARSABLE_TYPES)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES

#elif defined(PARSABLE_TYPES2) && defined(PARSABLE_TYPES3)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES2, \
    PARSABLE_TYPES3

#elif defined(PARSABLE_TYPES2)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES2

#elif defined(PARSABLE_TYPES3)

    #define NEAT_USER_PARSABLE_TYPES \
    , \
    PARSABLE_TYPES3

#else // user didn't define any parsable type

    #define NEAT_USER_PARSABLE_TYPES

#endif // User's PARSABLE_TYPES

#define NEAT_DEFAULT_STRINGABLE_TYPES \
NEAT_ADD_STRINGABLE(char,      neat_char2str), \
NEAT_ADD_STRINGABLE(char*,     neat_str2str), \
NEAT_ADD_STRINGABLE(bool,      neat_bool2str), \
NEAT_ADD_STRINGABLE(int8_t,    neat_int8_t2str), \
NEAT_ADD_STRINGABLE(int16_t,   neat_int16_t2str), \
NEAT_ADD_STRINGABLE(int32_t,   neat_int32_t2str), \
NEAT_ADD_STRINGABLE(long long, neat_int64_t2str), \
NEAT_ADD_STRINGABLE(int64_t,   neat_int64_t2str), \
NEAT_ADD_STRINGABLE(uint8_t,   neat_uint8_t2str), \
NEAT_ADD_STRINGABLE(uint16_t,  neat_uint16_t2str), \
NEAT_ADD_STRINGABLE(uint32_t,  neat_uint32_t2str), \
NEAT_ADD_STRINGABLE(unsigned   long long, neat_uint64_t2str), \
NEAT_ADD_STRINGABLE(uint64_t,  neat_uint64_t2str)

#define NEAT_DEFAULT_PARSABLE_TYPES \
NEAT_ADD_PARSABLE(char,      neat_parse_char), \
NEAT_ADD_PARSABLE(char*,     neat_parse_str), \
NEAT_ADD_PARSABLE(bool,      neat_parse_bool), \
NEAT_ADD_PARSABLE(int8_t,    neat_parse_int8_t), \
NEAT_ADD_PARSABLE(int16_t,   neat_parse_int16_t), \
NEAT_ADD_PARSABLE(int32_t,   neat_parse_int32_t), \
NEAT_ADD_PARSABLE(long long, neat_parse_int64_t), \
NEAT_ADD_PARSABLE(int64_t,   neat_parse_int64_t), \
NEAT_ADD_PARSABLE(uint8_t,   neat_parse_uint8_t), \
NEAT_ADD_PARSABLE(uint16_t,  neat_parse_uint16_t), \
NEAT_ADD_PARSABLE(uint32_t,  neat_parse_uint32_t), \
NEAT_ADD_PARSABLE(unsigned   long long, neat_parse_uint64_t), \
NEAT_ADD_PARSABLE(uint64_t,  neat_parse_uint64_t)

#define NEAT_ALL_STRINGABLE_TYPES \
NEAT_DEFAULT_STRINGABLE_TYPES \
NEAT_USER_STRINGABLE_TYPES

#define NEAT_ALL_PARSABLE_TYPES \
NEAT_DEFAULT_PARSABLE_TYPES \
NEAT_USER_PARSABLE_TYPES

#define neat_is_empty(dummy, ...) ( sizeof( (char[]){#__VA_ARGS__} ) == 1 )

#define neat_to_string(obj) \
_Generic(obj, NEAT_ALL_STRINGABLE_TYPES)( &(typeof(obj)[]){obj}[0] )

#define neat_array_to_string(arr, n) \
neat_array_to_string_f(arr, n, sizeof(*arr), (char*(*)(void*)) _Generic(*arr, NEAT_ALL_STRINGABLE_TYPES))

#define neat_fprint(file, obj) do { \
char *neat_str = neat_to_string(obj); \
fprintf(file, "%s", neat_str); \
free(neat_str); \
} while(0)

#define neat_print(obj) neat_fprint(stdout, obj)

#define neat_fprintln(file, obj) do { \
char *neat_str = neat_to_string(obj); \
fprintf(file, "%s\n", neat_str); \
free(neat_str); \
} while(0)

#define neat_println(obj) neat_fprintln(stdout, obj)

#define neat_fprint_array(file, arr, n) do { \
char *neat_str = neat_array_to_string(arr, n); \
fprintf(file, "%s", neat_str); \
free(neat_str); \
} while(0)

#define neat_print_array(arr, n) neat_fprint_array(stdout, arr, n)

#define neat_fprintln_array(file, arr, n) do { \
char *neat_str = neat_array_to_string(arr, n); \
fprintf(file, "%s\n", neat_str); \
free(neat_str); \
} while(0)

#define neat_println_array(arr, n) neat_fprintln_array(stdout, arr, n)

#define neat_parse(type, str, ...) \
_Generic((type){0}, NEAT_ALL_PARSABLE_TYPES)(str, (int*[2]){ &(int){0} , ##__VA_ARGS__ }[ !neat_is_empty(d,__VA_ARGS__) ])

#ifndef NEAT_TOSTR_PREFIX
    #define ADD_STRINGABLE NEAT_ADD_STRINGABLE
    #define ADD_PARSABLE NEAT_ADD_PARSABLE
    #define to_string neat_to_string
    #define array_to_string neat_array_to_string
    #define print neat_print
    #define fprint neat_fprint
    #define println neat_println
    #define fprintln neat_fprintln
    #define print_array neat_print_array
    #define fprint_array neat_fprint_array
    #define println_array neat_println_array
    #define fprintln_array neat_fprintln_array
    #define parse neat_parse
#endif

char *neat_array_to_string_f(const void *arr, size_t len, size_t elm_size, char*(*tostr)(void*));

// 2str functions declarations

char *neat_char2str(char *obj);
char *neat_str2str(char **obj);
char *neat_bool2str(bool *obj);
char *neat_int8_t2str(int8_t *obj);
char *neat_int16_t2str(int16_t *obj);
char *neat_int32_t2str(int32_t *obj);
char *neat_int64_t2str(int64_t *obj);
char *neat_uint8_t2str(uint8_t *obj);
char *neat_uint16_t2str(uint16_t *obj);
char *neat_uint32_t2str(uint32_t *obj);
char *neat_uint64_t2str(uint64_t *obj);
// char *neat_float2str(float *obj);
// char *neat_double2str(double *obj);

// parse functions declarations

char neat_parse_char(char *str, int *err);
char *neat_parse_str(char *str, int *err);
bool neat_parse_bool(char *str, int *err); // should I parse bool? Maybe user wants T or TRUE instead of true. Let user make his parse_bool func?
int8_t neat_parse_int8_t(char *str, int *err);
int16_t neat_parse_int16_t(char *str, int *err);
int32_t neat_parse_int32_t(char *str, int *err);
int64_t neat_parse_int64_t(char *str, int *err);
uint8_t neat_parse_uint8_t(char *str, int *err);
uint16_t neat_parse_uint16_t(char *str, int *err);
uint32_t neat_parse_uint32_t(char *str, int *err);
uint64_t neat_parse_uint64_t(char *str, int *err);
// float neat_parse_float(char *str, int *err);
// double neat_parse_double(char *str, int *err);

#endif // NEAT_TOSTR_H

#ifdef NEAT_TOSTR_IMPLEMENTATION

char *neat_array_to_string_f(const void *arr, size_t len, size_t elm_size, char*(*tostr)(void*))
{
    if(len == 0)
    {
        char *ret = malloc(3 * sizeof(char));
        memcpy(ret, "{}", 3);
        return ret;
    }
    
    const uint8_t *u8arr = arr;
    
    char **strings = malloc(len * sizeof(char*));
    for(size_t i = 0 ; i < len ; i++)
    {
        strings[i] = tostr(u8arr + (i * elm_size));
    }
    
    char *ret = NULL;
    
    size_t final_str_len = 1;
    for(size_t i = 0 ; i < len ; i++) {
        final_str_len += strlen(strings[i]) + 2;
    }
    ret = malloc(final_str_len * sizeof(char) );
    ret[0] = '{';
    for(size_t i = 0 ; i < len - 1; i++) {
        strcat(ret, strings[i]);
        strcat(ret, ", ");
    }
    strcat(ret, strings[len - 1]);
    ret[final_str_len - 2] = '}';
    
    return ret;
}

// 2str functions definitions

char *neat_char2str(char *obj) {
    char *ret = malloc(2 * sizeof(char));
    ret[0] = *obj;
    ret[1] = 0;
    return ret;
}

char *neat_str2str(char **obj) {
    size_t len = strlen(*obj);
    char *ret = malloc(len + 1);
    memcpy(ret, *obj, len + 1);
    return ret;
}

char *neat_bool2str(bool *obj) {
    char *ret = malloc(6 * sizeof(char));
    memcpy(ret, "false", 6);
    if(*obj) memcpy(ret, "true", 5);
    return ret;
}

char *neat_int8_t2str(int8_t *obj) {
    char *ret = malloc(5 * sizeof(char));
    sprintf(ret, "%d", *obj);
    return ret;
}

char *neat_int16_t2str(int16_t *obj) {
    char *ret = malloc(7 * sizeof(char));
    sprintf(ret, "%d", *obj);
    return ret;
}

char *neat_int32_t2str(int32_t *obj) {
    char *ret = malloc(12 * sizeof(char));
    sprintf(ret, "%d", *obj);
    return ret;
}

char *neat_int64_t2str(int64_t *obj) {
    char *ret = malloc(21 * sizeof(char));
    sprintf(ret, "%lld", *obj);
    return ret;
}

char *neat_uint8_t2str(uint8_t *obj) {
    char *ret = malloc(4 * sizeof(char));
    sprintf(ret, "%hhu", *obj);
    return ret;
}

char *neat_uint16_t2str(uint16_t *obj) {
    char *ret = malloc(6 * sizeof(char));
    sprintf(ret, "%hu", *obj);
    return ret;
}

char *neat_uint32_t2str(uint32_t *obj) {
    char *ret = malloc(12 * sizeof(char));
    sprintf(ret, "%u", *obj);
    return ret;
}

char *neat_uint64_t2str(uint64_t *obj) {
    char *ret = malloc(21 * sizeof(char));
    sprintf(ret, "%llu", *obj);
    return ret;
}

// char *neat_float2str(float *obj)
// {
//     int max_digits = 3 + DBL_MANT_DIG - DBL_MIN_EXP;
//     char *ret = malloc(max_digits * sizeof(char));
// }
// 
// char *neat_double2str(double *obj);

// parse functions definitions

char neat_parse_char(char *str, int *err) {
    *err = 0;
    return str[0];
}

char *neat_parse_str(char *str, int *err) {
    *err = 0;
    return str;
}

bool neat_parse_bool(char *str, int *err) {
    size_t len = strlen(str);
    *err = 0;
    if(len < 4) {
        *err = -1;
        return false;
    }
    if(strncmp(str, "true", 4) == 0) {
        return true;
    }
    else if(strncmp(str, "false", 5) == 0) {
        return false;
    }
    *err = -1;
    return false;
}

int8_t neat_parse_int8_t(char *str, int *err) {
    int8_t ret;
    *err = sscanf(str, "%hhd", &ret);
    return ret;
}

int16_t neat_parse_int16_t(char *str, int *err) {
    int16_t ret;
    *err = sscanf(str, "%hd", &ret);
    return ret;
}

int32_t neat_parse_int32_t(char *str, int *err) {
    int32_t ret;
    *err = sscanf(str, "%d", &ret);
    return ret;
}

int64_t neat_parse_int64_t(char *str, int *err) {
    int64_t ret;
    *err = sscanf(str, "%lld", &ret);
    return ret;
}

uint8_t neat_parse_uint8_t(char *str, int *err) {
    uint8_t ret;
    *err = sscanf(str, "%hhu", &ret);
    return ret;
}

uint16_t neat_parse_uint16_t(char *str, int *err) {
    uint16_t ret;
    *err = sscanf(str, "%hu", &ret);
    return ret;
}

uint32_t neat_parse_uint32_t(char *str, int *err) {
    uint32_t ret;
    *err = sscanf(str, "%u", &ret);
    return ret;
}

uint64_t neat_parse_uint64_t(char *str, int *err) {
    uint64_t ret;
    *err = sscanf(str, "%llu", &ret);
    return ret;
}

#endif // NEAT_TOSTR_IMPLEMENTATION