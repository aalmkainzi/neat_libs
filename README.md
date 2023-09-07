# neat_sort_library

A simple API for sorting and searching in C. By default it has sorting for all the main numeric types, also strings. Additional types can be added by user of the library.

API:
---
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

Extending with additional types:
---
Adding addtional types to sort is simple. Before doing ```#include "neat_sort.h"``` define a macro called ```#define SORTABLE_TYPES``` and in it, you can put additional types like this:

```C
#define SORTABLE_TYPES ADD_SORTABLE(my_type, my_type_cmp)
```

the function ```my_type_cmp``` must be in this format: 
```C
int my_type_cmp(const my_type*, const my_type*)
```
Basically a qsort compare function.

Sorting code example:
---
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

Searching
---
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
Both return a pointer to the found element in the array.

```SEARCH_PTR``` and ```BSEARCH_PTR``` are similar to their sorting equivilant:
```C
void g(float *arr, int n)
{
  float *elm;
  elm = BSEARCH_PTR(arr, n, 3.14); // calls stdlib bsearch
  elm = SEARCH_PTR(arr, n, 6.28);  // does linear search
}
```
Searching code example
---
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
