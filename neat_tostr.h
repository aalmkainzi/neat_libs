/*
    This is header-only library that provides an easy-to-use API
    for parsing from strings and converting to strings.
    
    To use this library, do this in *one* C file before including:
        #define NEAT_TOSTR_IMPLEMENTATION
    
    DOCUMENTATION
        
        By default this library has parsing/to_string support
        for all main number types, char, bool, and string.
        User types can easily be added.
        
        Extending with additonal stringable types:
            
            #define STRINGABLE_TYPES ADD_STRINGABLE(T, T2str)
            #include "neat_tostr.h"
            
        T2str must be a function with this prototype:
            
            char *T2str(T*);
            
        And it should return a 'malloc'ed string.
        
        Extending with additonal parsable types:
            
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
                  of size n.
                
            print:
                void print(...);
                  Prints to stdout the string representation of all
                  the arguments.
                  
            fprint:
                void fprint(FILE* f, ...);
                  Prints to f the string representation of all the
                  arguments.
                
            println:
                void println(...);
                  Prints to stdout the string representation of all
                  the args, with '\n' afterwards.
                  
            fprintln:
                void fprintln(FILE* f, ...);
                  Prints to f the string representation of all
                  the args, with '\n' afterwards.
                  
            print_array:
                void print_array(S* s, int n);
                  Prints to stdout the string representation of the
                  array s with length n.
                  
            fprint_array:
                void fprint_array(FILE* f, S* s, int n);
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
                  
            get_tostr:
                char*(*)(S*) get_tostr(S);
                  Returns the to_string function associated
                  with S.
                
            get_parse:
                S(*)(char*,int*) get_parse(S);
                  Returns the parse function associated
                  with S.
                  
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
NEAT_ADD_STRINGABLE(bool,      neat_bool2str), \
NEAT_ADD_STRINGABLE(int8_t,    neat_int8_t2str), \
NEAT_ADD_STRINGABLE(int16_t,   neat_int16_t2str), \
NEAT_ADD_STRINGABLE(int32_t,   neat_int32_t2str), \
NEAT_ADD_STRINGABLE(long long, neat_int64_t2str), \
NEAT_ADD_STRINGABLE(int64_t,   neat_int64_t2str), \
NEAT_ADD_STRINGABLE(uint8_t,   neat_uint8_t2str), \
NEAT_ADD_STRINGABLE(uint16_t,  neat_uint16_t2str), \
NEAT_ADD_STRINGABLE(uint32_t,  neat_uint32_t2str), \
NEAT_ADD_STRINGABLE(uint64_t,  neat_uint64_t2str), \
NEAT_ADD_STRINGABLE(float,     neat_float2str), \
NEAT_ADD_STRINGABLE(double,    neat_double2str), \
NEAT_ADD_STRINGABLE(unsigned long long, neat_uint64_t2str)

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
NEAT_ADD_PARSABLE(uint64_t,  neat_parse_uint64_t), \
NEAT_ADD_PARSABLE(float,     neat_parse_float), \
NEAT_ADD_PARSABLE(double,    neat_parse_double), \
NEAT_ADD_PARSABLE(unsigned long long, neat_parse_uint64_t)

#define NEAT_ALL_STRINGABLE_TYPES \
NEAT_DEFAULT_STRINGABLE_TYPES \
NEAT_USER_STRINGABLE_TYPES

#define NEAT_ALL_PARSABLE_TYPES \
NEAT_DEFAULT_PARSABLE_TYPES \
NEAT_USER_PARSABLE_TYPES

#define neat_is_empty(dummy, ...) ( sizeof( (char[]){#__VA_ARGS__} ) == 1 )

#define neat_get_tostr(type) \
_Generic( (typeof(type)){0} , NEAT_ALL_STRINGABLE_TYPES, char*: ((char*(*)(typeof(type)*))neat_str2str_dummy) )

#define neat_get_parse(type) \
_Generic( (typeof(type)){0} , NEAT_ALL_PARSABLE_TYPES)

// call neat_get_tostr
#define neat_to_string(obj) \
_Generic(obj, char*: ((char*(*)(typeof(obj)))neat_str2str)(obj), default: neat_get_tostr(obj)( &(typeof(obj)){obj} ) )

#define neat_array_to_string(arr, n) \
neat_array_to_string_f(arr, n, sizeof(*arr), (char*(*)(void*)) neat_get_tostr(*arr))

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

#define NEAT_NARG(...) \
NEAT_NARG_(__VA_ARGS__,NEAT_100SEQ())

#define NEAT_NARG_(...) \
NEAT_ARG_N(__VA_ARGS__)

#define NEAT_ARG_N( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
_61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
_71,_72,_73,_74,_75,_76,_77,_78,_79,_80, \
_81,_82,_83,_84,_85,_86,_87,_88,_89,_90, \
_91,_92,_93,_94,_95,_96,_97,_98,_99, N,...) N

#define NEAT_100SEQ() \
99,98,97,96,95,94,93,92,91,90, \
89,88,87,86,85,84,83,82,81,80, \
79,78,77,76,75,74,73,72,71,70, \
69,68,67,66,65,64,63,62,61,60, \
59,58,57,56,55,54,53,52,51,50, \
49,48,47,46,45,44,43,42,41,40, \
39,38,37,36,35,34,33,32,31,30, \
29,28,27,26,25,24,23,22,21,20, \
19,18,17,16,15,14,13,12,11,10, \
9,8,7,6,5,4,3,2,1,0

#define NEAT_CAT(a, ...) NEAT_PRIMITIVE_CAT(a, __VA_ARGS__)
#define NEAT_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define neat_fprint(file, ...) NEAT_CAT(neat_fprint, NEAT_NARG(__VA_ARGS__))(file, __VA_ARGS__)

#define neat_print(...) neat_fprint(stdout, __VA_ARGS__)

#define neat_fprintln(file, ...) do { \
neat_fprint(file, __VA_ARGS__); \
fputc('\n', file); \
} while(0)

#define neat_println(...) neat_fprintln(stdout, __VA_ARGS__)

#define neat_fprint1(file, o1) do { \
char *neat_str = neat_to_string(o1); \
fprintf(file, "%s", neat_str); \
free(neat_str); \
} while(0)

#define neat_fprint2(file, o1, o2) do { \
neat_fprint1(file, o1); \
neat_fprint1(file, o2); \
} while(0)

#define neat_fprint3(file, o1, o2, o3) do { \
neat_fprint2(file, o1, o2); \
neat_fprint1(file, o3); \
} while(0)

#define neat_fprint4(file, o1, o2, o3, o4) do { \
neat_fprint3(file, o1, o2, o3); \
neat_fprint1(file, o4); \
} while(0)

#define neat_fprint5(file, o1, o2, o3, o4, o5) do { \
neat_fprint4(file, o1, o2, o3, o4); \
neat_fprint1(file, o5); \
} while(0)

#define neat_fprint6(file, o1, o2, o3, o4, o5, o6) do { \
neat_fprint5(file, o1, o2, o3, o4, o5); \
neat_fprint1(file, o6); \
} while(0)

#define neat_fprint7(file, o1, o2, o3, o4, o5, o6, o7) do { \
neat_fprint6(file, o1, o2, o3, o4, o5, o6); \
neat_fprint1(file, o7); \
} while(0)

#define neat_fprint8(file, o1, o2, o3, o4, o5, o6, o7, o8) do { \
neat_fprint7(file, o1, o2, o3, o4, o5, o6, o7); \
neat_fprint1(file, o8); \
} while(0)

#define neat_fprint9(file, o1, o2, o3, o4, o5, o6, o7, o8, o9) do { \
neat_fprint8(file, o1, o2, o3, o4, o5, o6, o7, o8); \
neat_fprint1(file, o9); \
} while(0)

#define neat_fprint10(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10) do { \
neat_fprint9(file, o1, o2, o3, o4, o5, o6, o7, o8, o9); \
neat_fprint1(file, o10); \
} while(0)

#define neat_fprint11(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11) do { \
neat_fprint10(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10); \
neat_fprint1(file, o11); \
} while(0)

#define neat_fprint12(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12) do { \
neat_fprint11(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11); \
neat_fprint1(file, o12); \
} while(0)

#define neat_fprint13(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13) do { \
neat_fprint12(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12); \
neat_fprint1(file, o13); \
} while(0)

#define neat_fprint14(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14) do { \
neat_fprint13(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13); \
neat_fprint1(file, o14); \
} while(0)

#define neat_fprint15(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15) do { \
neat_fprint14(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14); \
neat_fprint1(file, o15); \
} while(0)

#define neat_fprint16(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16) do { \
neat_fprint15(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15); \
neat_fprint1(file, o16); \
} while(0)

#define neat_fprint17(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17) do { \
neat_fprint16(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16); \
neat_fprint1(file, o17); \
} while(0)

#define neat_fprint18(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18) do { \
neat_fprint17(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17); \
neat_fprint1(file, o18); \
} while(0)

#define neat_fprint19(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19) do { \
neat_fprint18(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18); \
neat_fprint1(file, o19); \
} while(0)

#define neat_fprint20(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20) do { \
neat_fprint19(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19); \
neat_fprint1(file, o20); \
} while(0)

#define neat_fprint21(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21) do { \
neat_fprint20(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20); \
neat_fprint1(file, o21); \
} while(0)

#define neat_fprint22(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22) do { \
neat_fprint21(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21); \
neat_fprint1(file, o22); \
} while(0)

#define neat_fprint23(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23) do { \
neat_fprint22(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22); \
neat_fprint1(file, o23); \
} while(0)

#define neat_fprint24(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24) do { \
neat_fprint23(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23); \
neat_fprint1(file, o24); \
} while(0)

#define neat_fprint25(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25) do { \
neat_fprint24(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24); \
neat_fprint1(file, o25); \
} while(0)

#define neat_fprint26(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26) do { \
neat_fprint25(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25); \
neat_fprint1(file, o26); \
} while(0)

#define neat_fprint27(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27) do { \
neat_fprint26(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26); \
neat_fprint1(file, o27); \
} while(0)

#define neat_fprint28(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28) do { \
neat_fprint27(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27); \
neat_fprint1(file, o28); \
} while(0)

#define neat_fprint29(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29) do { \
neat_fprint28(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28); \
neat_fprint1(file, o29); \
} while(0)

#define neat_fprint30(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30) do { \
neat_fprint29(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29); \
neat_fprint1(file, o30); \
} while(0)

#define neat_fprint31(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31) do { \
neat_fprint30(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30); \
neat_fprint1(file, o31); \
} while(0)

#define neat_fprint32(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32) do { \
neat_fprint31(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31); \
neat_fprint1(file, o32); \
} while(0)

#define neat_fprint33(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33) do { \
neat_fprint32(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32); \
neat_fprint1(file, o33); \
} while(0)

#define neat_fprint34(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34) do { \
neat_fprint33(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33); \
neat_fprint1(file, o34); \
} while(0)

#define neat_fprint35(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35) do { \
neat_fprint34(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34); \
neat_fprint1(file, o35); \
} while(0)

#define neat_fprint36(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36) do { \
neat_fprint35(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35); \
neat_fprint1(file, o36); \
} while(0)

#define neat_fprint37(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37) do { \
neat_fprint36(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36); \
neat_fprint1(file, o37); \
} while(0)

#define neat_fprint38(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38) do { \
neat_fprint37(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37); \
neat_fprint1(file, o38); \
} while(0)

#define neat_fprint39(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39) do { \
neat_fprint38(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38); \
neat_fprint1(file, o39); \
} while(0)

#define neat_fprint40(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40) do { \
neat_fprint39(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39); \
neat_fprint1(file, o40); \
} while(0)

#define neat_fprint41(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41) do { \
neat_fprint40(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40); \
neat_fprint1(file, o41); \
} while(0)

#define neat_fprint42(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42) do { \
neat_fprint41(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41); \
neat_fprint1(file, o42); \
} while(0)

#define neat_fprint43(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43) do { \
neat_fprint42(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42); \
neat_fprint1(file, o43); \
} while(0)

#define neat_fprint44(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44) do { \
neat_fprint43(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43); \
neat_fprint1(file, o44); \
} while(0)

#define neat_fprint45(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45) do { \
neat_fprint44(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44); \
neat_fprint1(file, o45); \
} while(0)

#define neat_fprint46(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46) do { \
neat_fprint45(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45); \
neat_fprint1(file, o46); \
} while(0)

#define neat_fprint47(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47) do { \
neat_fprint46(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46); \
neat_fprint1(file, o47); \
} while(0)

#define neat_fprint48(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48) do { \
neat_fprint47(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47); \
neat_fprint1(file, o48); \
} while(0)

#define neat_fprint49(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49) do { \
neat_fprint48(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48); \
neat_fprint1(file, o49); \
} while(0)

#define neat_fprint50(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50) do { \
neat_fprint49(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49); \
neat_fprint1(file, o50); \
} while(0)

#define neat_fprint51(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51) do { \
neat_fprint50(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50); \
neat_fprint1(file, o51); \
} while(0)

#define neat_fprint52(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52) do { \
neat_fprint51(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51); \
neat_fprint1(file, o52); \
} while(0)

#define neat_fprint53(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53) do { \
neat_fprint52(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52); \
neat_fprint1(file, o53); \
} while(0)

#define neat_fprint54(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54) do { \
neat_fprint53(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53); \
neat_fprint1(file, o54); \
} while(0)

#define neat_fprint55(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55) do { \
neat_fprint54(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54); \
neat_fprint1(file, o55); \
} while(0)

#define neat_fprint56(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56) do { \
neat_fprint55(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55); \
neat_fprint1(file, o56); \
} while(0)

#define neat_fprint57(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57) do { \
neat_fprint56(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56); \
neat_fprint1(file, o57); \
} while(0)

#define neat_fprint58(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58) do { \
neat_fprint57(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57); \
neat_fprint1(file, o58); \
} while(0)

#define neat_fprint59(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59) do { \
neat_fprint58(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58); \
neat_fprint1(file, o59); \
} while(0)

#define neat_fprint60(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60) do { \
neat_fprint59(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59); \
neat_fprint1(file, o60); \
} while(0)

#define neat_fprint61(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61) do { \
neat_fprint60(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60); \
neat_fprint1(file, o61); \
} while(0)

#define neat_fprint62(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62) do { \
neat_fprint61(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61); \
neat_fprint1(file, o62); \
} while(0)

#define neat_fprint63(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63) do { \
neat_fprint62(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62); \
neat_fprint1(file, o63); \
} while(0)

#define neat_fprint64(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64) do { \
neat_fprint63(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63); \
neat_fprint1(file, o64); \
} while(0)

#define neat_fprint65(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65) do { \
neat_fprint64(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64); \
neat_fprint1(file, o65); \
} while(0)

#define neat_fprint66(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66) do { \
neat_fprint65(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65); \
neat_fprint1(file, o66); \
} while(0)

#define neat_fprint67(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67) do { \
neat_fprint66(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66); \
neat_fprint1(file, o67); \
} while(0)

#define neat_fprint68(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68) do { \
neat_fprint67(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67); \
neat_fprint1(file, o68); \
} while(0)

#define neat_fprint69(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69) do { \
neat_fprint68(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68); \
neat_fprint1(file, o69); \
} while(0)

#define neat_fprint70(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70) do { \
neat_fprint69(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69); \
neat_fprint1(file, o70); \
} while(0)

#define neat_fprint71(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71) do { \
neat_fprint70(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70); \
neat_fprint1(file, o71); \
} while(0)

#define neat_fprint72(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72) do { \
neat_fprint71(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71); \
neat_fprint1(file, o72); \
} while(0)

#define neat_fprint73(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73) do { \
neat_fprint72(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72); \
neat_fprint1(file, o73); \
} while(0)

#define neat_fprint74(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74) do { \
neat_fprint73(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73); \
neat_fprint1(file, o74); \
} while(0)

#define neat_fprint75(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75) do { \
neat_fprint74(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74); \
neat_fprint1(file, o75); \
} while(0)

#define neat_fprint76(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76) do { \
neat_fprint75(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75); \
neat_fprint1(file, o76); \
} while(0)

#define neat_fprint77(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77) do { \
neat_fprint76(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76); \
neat_fprint1(file, o77); \
} while(0)

#define neat_fprint78(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78) do { \
neat_fprint77(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77); \
neat_fprint1(file, o78); \
} while(0)

#define neat_fprint79(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79) do { \
neat_fprint78(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78); \
neat_fprint1(file, o79); \
} while(0)

#define neat_fprint80(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80) do { \
neat_fprint79(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79); \
neat_fprint1(file, o80); \
} while(0)

#define neat_fprint81(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81) do { \
neat_fprint80(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80); \
neat_fprint1(file, o81); \
} while(0)

#define neat_fprint82(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82) do { \
neat_fprint81(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81); \
neat_fprint1(file, o82); \
} while(0)

#define neat_fprint83(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83) do { \
neat_fprint82(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82); \
neat_fprint1(file, o83); \
} while(0)

#define neat_fprint84(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84) do { \
neat_fprint83(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83); \
neat_fprint1(file, o84); \
} while(0)

#define neat_fprint85(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85) do { \
neat_fprint84(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84); \
neat_fprint1(file, o85); \
} while(0)

#define neat_fprint86(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86) do { \
neat_fprint85(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85); \
neat_fprint1(file, o86); \
} while(0)

#define neat_fprint87(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87) do { \
neat_fprint86(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86); \
neat_fprint1(file, o87); \
} while(0)

#define neat_fprint88(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88) do { \
neat_fprint87(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87); \
neat_fprint1(file, o88); \
} while(0)

#define neat_fprint89(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89) do { \
neat_fprint88(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88); \
neat_fprint1(file, o89); \
} while(0)

#define neat_fprint90(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90) do { \
neat_fprint89(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89); \
neat_fprint1(file, o90); \
} while(0)

#define neat_fprint91(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91) do { \
neat_fprint90(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90); \
neat_fprint1(file, o91); \
} while(0)

#define neat_fprint92(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92) do { \
neat_fprint91(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91); \
neat_fprint1(file, o92); \
} while(0)

#define neat_fprint93(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93) do { \
neat_fprint92(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92); \
neat_fprint1(file, o93); \
} while(0)

#define neat_fprint94(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94) do { \
neat_fprint93(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93); \
neat_fprint1(file, o94); \
} while(0)

#define neat_fprint95(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95) do { \
neat_fprint94(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94); \
neat_fprint1(file, o95); \
} while(0)

#define neat_fprint96(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96) do { \
neat_fprint95(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95); \
neat_fprint1(file, o96); \
} while(0)

#define neat_fprint97(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97) do { \
neat_fprint96(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96); \
neat_fprint1(file, o97); \
} while(0)

#define neat_fprint98(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97, o98) do { \
neat_fprint97(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97); \
neat_fprint1(file, o98); \
} while(0)

#define neat_fprint99(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97, o98, o99) do { \
neat_fprint98(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97, o98); \
neat_fprint1(file, o99); \
} while(0)

#define neat_fprint100(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97, o98, o99, o100) do { \
neat_fprint99(file, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28, o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42, o43, o44, o45, o46, o47, o48, o49, o50, o51, o52, o53, o54, o55, o56, o57, o58, o59, o60, o61, o62, o63, o64, o65, o66, o67, o68, o69, o70, o71, o72, o73, o74, o75, o76, o77, o78, o79, o80, o81, o82, o83, o84, o85, o86, o87, o88, o89, o90, o91, o92, o93, o94, o95, o96, o97, o98, o99); \
neat_fprint1(file, o100); \
} while(0)

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
    #define get_tostr neat_get_tostr
    #define get_parse neat_get_parse
#endif

char *neat_array_to_string_f(void *arr, size_t len, size_t elm_size, char*(*tostr)(void*));

// 2str functions declarations

char *neat_char2str(char *obj);
char *neat_str2str(char *obj); char *neat_str2str_dummy(char **obj);
char *neat_bool2str(bool *obj);
char *neat_int8_t2str(int8_t *obj);
char *neat_int16_t2str(int16_t *obj);
char *neat_int32_t2str(int32_t *obj);
char *neat_int64_t2str(int64_t *obj);
char *neat_uint8_t2str(uint8_t *obj);
char *neat_uint16_t2str(uint16_t *obj);
char *neat_uint32_t2str(uint32_t *obj);
char *neat_uint64_t2str(uint64_t *obj);
char *neat_float2str(float *obj);
char *neat_double2str(double *obj);

// parse functions declarations

char neat_parse_char(char *str, int *err);
char *neat_parse_str(char *str, int *err);
bool neat_parse_bool(char *str, int *err);
int8_t neat_parse_int8_t(char *str, int *err);
int16_t neat_parse_int16_t(char *str, int *err);
int32_t neat_parse_int32_t(char *str, int *err);
int64_t neat_parse_int64_t(char *str, int *err);
uint8_t neat_parse_uint8_t(char *str, int *err);
uint16_t neat_parse_uint16_t(char *str, int *err);
uint32_t neat_parse_uint32_t(char *str, int *err);
uint64_t neat_parse_uint64_t(char *str, int *err);
float neat_parse_float(char *str, int *err);
double neat_parse_double(char *str, int *err);

#endif // NEAT_TOSTR_H

#ifdef NEAT_TOSTR_IMPLEMENTATION

char *neat_array_to_string_f(void *arr, size_t len, size_t elm_size, char*(*tostr)(void*))
{
    if(len == 0)
    {
        char *ret = malloc(3 * sizeof(char));
        memcpy(ret, "{}", 3);
        return ret;
    }
    
    uint8_t *u8arr = arr;
    
    char **strings = malloc(len * sizeof(char*));
    for(size_t i = 0 ; i < len ; i++)
    {
        strings[i] = tostr(u8arr + (i * elm_size));
    }
    
    size_t final_str_len = 1;
    for(size_t i = 0 ; i < len ; i++) {
        final_str_len += strlen(strings[i]) + 2;
    }
    
    char *ret = calloc(final_str_len, sizeof(char));
    ret[0] = '{';
    for(size_t i = 0 ; i < len - 1; i++) {
        strcat(ret, strings[i]);
        strcat(ret, ", ");
    }
    strcat(ret, strings[len - 1]);
    ret[final_str_len - 2] = '}';
    
    for(size_t i = 0 ; i < len ; i++)
    {
        free(strings[i]);
    }
    free(strings);
    
    return ret;
}

// 2str functions definitions

char *neat_char2str(char *obj) {
    char *ret = malloc(2 * sizeof(char));
    ret[0] = *obj;
    ret[1] = 0;
    return ret;
}

char *neat_str2str(char *obj) {
    size_t len = strlen(obj);
    char *ret = malloc(len + 1);
    memcpy(ret, obj, len + 1);
    return ret;
}

char *neat_str2str_dummy(char **obj) { return (char**)obj; };

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
    sprintf(ret, "%zd", *obj);
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
    sprintf(ret, "%zu", *obj);
    return ret;
}

char *neat_float2str(float *obj) {
    int len = snprintf(NULL, 0, "%g", *obj);
    char *ret = calloc(sizeof(char), len + 1);
    
    sprintf(ret, "%g", *obj);
    
    return ret;
}

char *neat_double2str(double *obj) {
    int len = snprintf(NULL, 0, "%g", *obj);
    char *ret = calloc(sizeof(char), len + 1);
    
    sprintf(ret, "%lg", *obj);
    
    return ret;
}

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
    if(strncmp(str, "true", 4) == 0) { // can actually replace this with memcmp since we know the size
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
    *err = sscanf(str, "%hhd", &ret) - 1;
    *err &= -(*err < 0); // sets *err to 0 if positive.
    return ret;
}

int16_t neat_parse_int16_t(char *str, int *err) {
    int16_t ret;
    *err = sscanf(str, "%hd", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

int32_t neat_parse_int32_t(char *str, int *err) {
    int32_t ret;
    *err = sscanf(str, "%d", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

int64_t neat_parse_int64_t(char *str, int *err) {
    int64_t ret;
    *err = sscanf(str, "%zd", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

uint8_t neat_parse_uint8_t(char *str, int *err) {
    uint8_t ret;
    *err = sscanf(str, "%hhu", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

uint16_t neat_parse_uint16_t(char *str, int *err) {
    uint16_t ret;
    *err = sscanf(str, "%hu", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

uint32_t neat_parse_uint32_t(char *str, int *err) {
    uint32_t ret;
    *err = sscanf(str, "%u", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

uint64_t neat_parse_uint64_t(char *str, int *err) {
    uint64_t ret;
    *err = sscanf(str, "%zu", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

float neat_parse_float(char *str, int *err)
{
    float ret;
    *err = sscanf(str, "%g", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

double neat_parse_double(char *str, int *err)
{
    double ret;
    *err = sscanf(str, "%lg", &ret) - 1;
    *err &= -(*err < 0);
    return ret;
}

#endif // NEAT_TOSTR_IMPLEMENTATION
