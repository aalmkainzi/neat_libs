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
            
            char *T2str(T);
            
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
                void array_to_string(S* s, n, str);
                  Makes a string from the array s of size n,
                  and stores it into str.
                 
            parse:
                S parse(S, char *str);
                  Parses S from str and returns it.
                
                S parse(S, char *str, int *err);
                  Parses S from str and returns it.
                  Puts error code into err.
*/

#ifndef NEAT_TOSTR_H
#define NEAT_TOSTR_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>

#define ADD_STRINGABLE(type, tostr) \
type: tostr

#define ADD_PARSABLE(type, parse) \
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
ADD_STRINGABLE(char,     neat_char2str), \
ADD_STRINGABLE(char*,    neat_str2str), \
ADD_STRINGABLE(bool,     neat_bool2str), \
ADD_STRINGABLE(int8_t,   neat_int8_t2str), \
ADD_STRINGABLE(int16_t,  neat_int16_t2str), \
ADD_STRINGABLE(int32_t,  neat_int32_t2str), \
ADD_STRINGABLE(long long,  neat_int64_t2str), \
ADD_STRINGABLE(int64_t,  neat_int64_t2str), \
ADD_STRINGABLE(uint8_t,  neat_uint8_t2str), \
ADD_STRINGABLE(uint16_t, neat_uint16_t2str), \
ADD_STRINGABLE(uint32_t, neat_uint32_t2str), \
ADD_STRINGABLE(unsigned long long, neat_uint64_t2str), \
ADD_STRINGABLE(uint64_t, neat_uint64_t2str)

#define NEAT_DEFAULT_PARSABLE_TYPES \
ADD_PARSABLE(char,     neat_parse_char), \
ADD_PARSABLE(char*,    neat_parse_str), \
ADD_PARSABLE(int8_t,   neat_parse_int8_t), \
ADD_PARSABLE(int16_t,  neat_parse_int16_t), \
ADD_PARSABLE(int32_t,  neat_parse_int32_t), \
ADD_PARSABLE(long long,  neat_parse_int64_t), \
ADD_PARSABLE(int64_t,  neat_parse_int64_t), \
ADD_PARSABLE(uint8_t,  neat_parse_uint8_t), \
ADD_PARSABLE(uint16_t, neat_parse_uint16_t), \
ADD_PARSABLE(uint32_t, neat_parse_uint32_t), \
ADD_PARSABLE(unsigned long long, neat_parse_uint64_t), \
ADD_PARSABLE(uint64_t, neat_parse_uint64_t)

#define NEAT_ALL_STRINGABLE_TYPES \
NEAT_DEFAULT_STRINGABLE_TYPES \
NEAT_USER_STRINGABLE_TYPES

#define NEAT_ALL_PARSABLE_TYPES \
NEAT_DEFAULT_PARSABLE_TYPES \
NEAT_USER_PARSABLE_TYPES

#define neat_is_empty(dummy, ...) ( sizeof( (char[]){#__VA_ARGS__} ) == 1 )

#define to_string(obj) \
_Generic(obj, NEAT_ALL_STRINGABLE_TYPES)(obj)

// make sure to allocate enough for '{' '}' and the ", "s
#define neat_combine_strings(strings, n, str_out) \
do { \
    size_t final_str_len = 1; \
    for(int i = 0 ; i < n ; i++) { \
        final_str_len += strlen(strings[i]) + 2; \
    } \
    str_out = malloc(final_str_len * sizeof(char) ); \
    str_out[0] = '{'; \
    for(int i = 0 ; i < n - 1; i++) { \
        strcat(str_out, strings[i]); \
        strcat(str_out, ", "); \
    } \
    strcat(str_out, strings[n - 1]); \
    str_out[final_str_len - 2] = '}'; \
} while(0)

#define array_to_string(arr, n, str_out) \
do { \
    if(n <= 0) { \
        str_out = malloc(3 * sizeof(char)); \
        memcpy(str_out, "{}", 3); \
    } \
    else { \
        char **strings = malloc(n * sizeof(char*)); \
        for(int i = 0 ; i < n ; i++) { \
            strings[i] = to_string(arr[i]); \
        } \
        neat_combine_strings(strings, n, str_out); \
        for(int i = 0 ; i < n ; i++) \
            free(strings[i]); \
        free(strings); \
    } \
} while(0)

#define parse(type, str, ...) \
_Generic((type){0}, NEAT_ALL_PARSABLE_TYPES)(str, (int*[]){ &(int){0} , ##__VA_ARGS__ }[ !neat_is_empty(d,__VA_ARGS__) ])

// 2str functions declarations

char *neat_char2str(char obj);
char *neat_str2str(char *obj);
char *neat_bool2str(bool obj);
char *neat_int8_t2str(int8_t obj);
char *neat_int16_t2str(int16_t obj);
char *neat_int32_t2str(int32_t obj);
char *neat_int64_t2str(int64_t obj);
char *neat_uint8_t2str(uint8_t obj);
char *neat_uint16_t2str(uint16_t obj);
char *neat_uint32_t2str(uint32_t obj);
char *neat_uint64_t2str(uint64_t obj);
// char *neat_float2str(float obj);
// char *neat_double2str(double obj);

// parse functions declarations

char neat_parse_char(char *str, int *err);
char *neat_parse_str(char *str, int *err);
// bool neat_parse_bool(char *str, int *err); // let's not parse bool. Maybe user wants T or TRUE instead of true. Let user make his parse_bool func
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

// 2str functions definitions

char *neat_char2str(char obj) {
    char *ret = malloc(2 * sizeof(char));
    ret[0] = obj;
    ret[1] = 0;
    return ret;
}

char *neat_str2str(char *obj) {
    size_t len = strlen(obj);
    char *ret = malloc(len + 1);
    memcpy(ret, obj, len + 1);
    return ret;
}

char *neat_bool2str(bool obj) {
    char *ret = malloc(6 * sizeof(char));
    memcpy(ret, "false", 6);
    if(obj) memcpy(ret, "true", 5);
    return ret;
}

char *neat_int8_t2str(int8_t obj) {
    char *ret = malloc(5 * sizeof(char));
    sprintf(ret, "%d", obj);
    return ret;
}

char *neat_int16_t2str(int16_t obj) {
    char *ret = malloc(7 * sizeof(char));
    sprintf(ret, "%d", obj);
    return ret;
}

char *neat_int32_t2str(int32_t obj) {
    char *ret = malloc(12 * sizeof(char));
    sprintf(ret, "%d", obj);
    return ret;
}

char *neat_int64_t2str(int64_t obj) {
    char *ret = malloc(21 * sizeof(char));
    sprintf(ret, "%lld", obj);
    return ret;
}

char *neat_uint8_t2str(uint8_t obj) {
    char *ret = malloc(4 * sizeof(char));
    sprintf(ret, "%hhu", obj);
    return ret;
}

char *neat_uint16_t2str(uint16_t obj) {
    char *ret = malloc(6 * sizeof(char));
    sprintf(ret, "%hu", obj);
    return ret;
}

char *neat_uint32_t2str(uint32_t obj) {
    char *ret = malloc(12 * sizeof(char));
    sprintf(ret, "%u", obj);
    return ret;
}

char *neat_uint64_t2str(uint64_t obj) {
    char *ret = malloc(21 * sizeof(char));
    sprintf(ret, "%llu", obj);
    return ret;
}

// char *neat_float2str(float obj)
// {
//     int max_digits = 3 + DBL_MANT_DIG - DBL_MIN_EXP;
//     char *ret = malloc(max_digits * sizeof(char));
// }
// 
// char *neat_double2str(double obj);

// parse functions definitions

char neat_parse_char(char *str, int *err) {
    *err = 0;
    return str[0];
}

char *neat_parse_str(char *str, int *err) {
    *err = 0;
    return str;
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