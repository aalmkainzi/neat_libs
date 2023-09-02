#include "neat_sort.h"
#include <string.h>

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

int str_cmp(const char** s1, const char** s2)
{
    return strcmp(*s1, *s2);
}
