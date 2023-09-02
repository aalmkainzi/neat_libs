#include "neat_sort.h"
#include <string.h>

define_cmp_func(char);
define_cmp_func(uchar);
define_cmp_func(short);
define_cmp_func(ushort);
define_cmp_func(int);
define_cmp_func(uint);
define_cmp_func(long);
define_cmp_func(ulong);
define_cmp_func(llong);
define_cmp_func(ullong);
define_cmp_func(float);
define_cmp_func(double);

int str_cmp(const char** s1, const char** s2)
{
    return strcmp(*s1, *s2);
}
