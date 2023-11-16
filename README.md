
# neat_libs
A collection of header-only C libraries. Aimed at providing a neat API that's simple and easy-to-use
- [neat_sort](#neat_sort)
- [neat_tostr](#neat_tostr)

## neat_sort
A simple API for sorting and searching in C. By default it has sorting for all the main numeric types, also strings. Additional types can be added by user of the library.

### API:
Before including the library, you must do ```#define NEAT_SORT_IMPLEMENTATION``` once in your project.

To sort a local stack array:
```C
#define NEAT_SORT_IMPLEMENTATION
#include "neat_sort.h"

int main()
{
    int arr[10];
    SORT(arr);
}
```
And to sort an array pointer:
```C
#include "neat_sort.h"

void f(int *arr, int n) {
    SORT_PTR(arr, n)
}
```
```SORT_PTR``` can also be called on local arrays.

To do descending sort, use ```SORT_DESC``` and ```SORT_DESC_PTR``` instead

### Extending with additional types:
Adding addtional types to sort is simple. Before doing ```#include "neat_sort.h"``` define a macro called ```#define SORTABLE_TYPES``` and in it, you can put additional types like this:

```C
#define SORTABLE_TYPES ADD_SORTABLE(my_type, my_type_cmp)
```

the function ```my_type_cmp``` must be in this format: 
```C
int my_type_cmp(const my_type*, const my_type*)
```
Basically a qsort compare function.

### Sorting example:
```C
#define SORTABLE_TYPES ADD_SORTABLE(A, a_cmp), ADD_SORTABLE(B, b_cmp)

#define NEAT_SORT_IMPLEMENTATION
#include "neat_sort.h"

typedef struct A {
    int i;
} A;

typedef struct B {
    char c;
} B;

int a_cmp(const A* a, const A* b) {
    return a->i - b->i;
}

int b_cmp(const B* a, const B* b) {
    return a->c - b->c;
}

int main() {
    A arr[10];
    SORT( arr );

    B arr2[10];
    SORT( arr2 );
}
```
If you prefer you can define the additional sort types on multiple lines, like so:
```C
#define SORTABLE_TYPES \
ADD_SORTABLE(A, a_cmp), \
ADD_SORTABLE(B, b_cmp)
```

### Searching
For all sortable types, you can also use binary search and linear search with ```BSEARCH``` and ```SEARCH```:
```C
#define NEAT_SORT_IMPLEMENTATION
#include "neat_sort.h"

int main()
{
    float arr[10];
    SORT( arr );

    float *elm;
    elm = BSEARCH(arr, 3.14); // calls stdlib bsearch
    elm = SEARCH(arr, 6.28);  // does linear search
}
```
Both return a pointer to the found element in the array, ```NULL``` if not found.

```SEARCH_PTR``` and ```BSEARCH_PTR``` are similar to their sorting equivalent:
```C
void g(float *arr, int n)
{
    float *elm;
    elm = BSEARCH_PTR(arr, n, 3.14); // calls stdlib bsearch
    elm = SEARCH_PTR(arr, n, 6.28);  // does linear search
}
```
### Searching example
```C
#define SORTABLE_TYPES \
ADD_SORTABLE(A, a_cmp), \
ADD_SORTABLE(B, b_cmp)

#define NEAT_SORT_IMPLEMENTATION
#include "neat_sort.h"

typedef struct A {
    int i;
} A;

typedef struct B {
    char c;
} B;

int a_cmp(const A* a, const A* b) {
    return a->i - b->i;
}

int b_cmp(const B* a, const B* b) {
    return a->c - b->c;
}

int main() {
    A arr[10];
    A *elm = SEARCH(arr, (A){.i = 3} );

    SORT( arr );
    elm = BSEARCH(arr, (A){.i = 5} );

    B arr2[10];
    B *elm2 = SEARCH(arr2, (B){.c = 'a'} );

    SORT( arr2 );
    elm2 = BSEARCH(arr2, (B){.c = 'c'} );
}
```

## neat_tostr
A library for converting to string and parsing from string. By default it has parse/to_string support for all the main number types, char, bool, and string.
Additional types can easily be added

### API
Before including the library, you must define `NEAT_TOSTR_IMPLEMENTATION` once in your project.

#### Converting to string
To convert to string:
```C
#define NEAT_TOSTR_IMPLEMENTATION
#include "neat_tostr.h"

int main()
{
    char *str = to_string(500);

    free(str);
}
```
To turn an array into a string:
```C
int main()
{
    int arr[10];
    char *str = array_to_string(arr, 10);

    free(str);
}
```

To call `print` and `fprint`:
```C
int main()
{
    print(100, "hello", 15.5f, "\n");
    fprint(stdout, 3.14, "\n", "neat!\n"); 
}
```
You can also call `println` and `fprintln` to put a newline afterwards.

To print arrays you can use `print_array` and `fprint_array`:
```C
int main()
{
    int arr[5] = {1, 2, 3, 4, 5};
    
    print_array(arr, 5); // prints "{1, 2, 3, 4, 5}"
    fprint_array(stdout, arr, 5); 
}
```

These functions (macros) work for any stringable type.

To add a stringable type:
```C
#define STRINGABLE_TYPES ADD_STRINGABLE(S, s2str)

#include "neat_tostr.h"
```
Where the function ```s2str``` is in this format:
```C
char *s2str(S*);
```
The string it retuns must be a ```malloc```ed string

#### Parsing
On top of type-to-string conversion, this library also has parsing from string to your types.

To parse from string:
```C
void f(char *num_str)
{
    int y = parse(int, num_str);
    // or
    int err;
    int z = parse(int, num_str, &err);
}
```

To add a parsable type:
```C
#define PARSABLE_TYPES ADD_PARSABLE(S, parse_s)

#include "neat_tostr.h"
```
Where the function ```parse_s``` must be in this format:
```C
S parse_s(char *str, int *err);
```
You can add as many stringable/parsable types as you want:
```C
#define STRINGABLE_TYPES \
ADD_STRINGABLE(A, a2str), \
ADD_STRINGABLE(B, b2str), \
ADD_STRINGABLE(C, c2str)

#define PARSABLE_TYPES \
ADD_PARSABLE(A, parse_a), \
ADD_PARSABLE(B, parse_b), \
ADD_PARSABLE(C, parse_c)

#define NEAT_TOSTR_IMPLEMENTATION
#include "neat_tostr.h"
```
