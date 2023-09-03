# neat_sort_library

A simple api for sorting in C. By default it has sorting for all the main numeric types, also strings. Additional types can be added by user of the library.

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

void func(int *arr, int n) {
  SORT_PTR(arr, n)
}
```
SORT_PTR can also be called on local arrays.

Extending with additional types:
---
Adding addtional types to sort is simple. Before doing ```#include "neat_sort.h"``` define a macro called ```#define ADDITIONAL_SORT_TYPES``` and in it, you can put additional types like this:

```C
#define ADDITIONAL_SORT_TYPES ADD_SORT_TYPE(my_type, my_type_cmp)
```

the function ```my_type_cmp``` must be in this format: 
```C
int (const my_type*, const my_type*)
```
Basically a qsort compare function.

code example:
```C
#define NEAT_SORT_IMPLEMENTATION
#define ADDITIONAL_SORT_TYPES ADD_SORT_TYPE(A, a_cmp), ADD_SORT_TYPE(B, b_cmp)
#include "neat_sort.h"

typedef struct A {
    int x;
} A;

typedef struct B {
    char c;
} B;

int a_cmp(const A* a, const A* b) {
    return a->x - b->x;
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
```
#define ADDITIONAL_SORT_TYPES \
ADD_SORT_TYPE(A, a_cmp), \
ADD_SORT_TYPE(B, b_cmp)
```



