# neat_sort_library

A neat api for sorting in C. By default it has sorting for all the main numeric types, also strings. Additional types can be added by user of the library.

API:
---
To sort a local stack array:
```
int main()
{
  int arr[10];
  SORT(arr);
}
```
And to sort an array pointer:
```
void func(int n) {
  int *arr = malloc(n * sizeof(int));
  SORT_PTR(arr, n)
}
```
SORT_PTR can also be called on local arrays.

Extending with additional types:
---
Adding addtional types to sort is simple. Go to the ```SORT_TYPES``` macro defined in ```neat_sort.h``` and add a new entry for your type, for example:
```
#define SORT_TYPES \
..., \
ADD_SORT_TYPE(my_type, my_type_cmp)
```

the function ```my_type_cmp``` must be in this format: ```int (const my_type*, const my_type*)```
Basically a qsort compare function.

