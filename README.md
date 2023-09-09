
# neat_libs
A collection of header-only C libraries.
- [neat_sort](#neat_sort)
- [neat_iterator](#neat_iterator)

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

## neat_iterator
An easy-to-use library that provides iterators in C.

### API
To add iterable types (arrays, linkedlist, hashmap, etc.) you do this:
```C
#define ITERABLE_TYPES \
ADD_ITERATOR(linkedlist, ls_begin,  ls_end,  ls_next,  ls_prev), \
ADD_ITERATOR(hashmap,    hm_begin,  hm_end,  hm_next,  hm_prev), \
ADD_ITERATOR(vector,     vec_begin, vec_end, vec_next, vec_prev)

#include "neat_iterator.h"
```
After which you can do stuff like:
```C
vector vec = ... ;

int *i;
i = it_begin(vec);
i = it_next(vec, i);
i = it_prev(vec, i);
```
And you can also use foreach loops:
```C
// loops from beginning to end. with iter name it
foreach(it, vec) {
    ...
}

// loops from end to beginning. with iter name it
foreach_r(it, vec) {
    ...
}

// loops from beginning to end, skipping n every iteration. with iter name it
foreach_skip(it, vec, n) {
    ...
}

// loops from end to beginning, skipping n every iteration. with iter name it
foreach_skip_r(it, vec, n) {
    ...
}
```

### Iterator example
```C
#define ITERABLE_TYPES \
ADD_ITERATOR(Arr10, arr10_begin, arr10_end, arr10_next, arr10_prev)

#include "neat_iterator.h"
#include <stdio.h>

typedef struct Arr10
{
    int data[10];
} Arr10;

int *arr10_begin(Arr10 *a)
{
    return &a->data[0];
}

int *arr10_end(Arr10 *a)
{
    return &a->data[10];
}

int *arr10_next(Arr10 *a, int *current)
{
    size_t index = (size_t)(current - a->data);
    return &(a->data[index + 1]);
}

int *arr10_prev(Arr10 *a, int *current)
{
    size_t index = (size_t)(current - a->data);
    return &(a->data[index - 1]);
}

int main()
{
    Arr10 arr = { .data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
    
    foreach(it, arr) {
        printf("%d, ", *it);
    }
}
```
