## C Notes

### Arrays 

Arrays can be initialized as follows:

 - `int classroom[5] ;` declaring an empty integer array of size 5.
- `int classroom[5] = {15, 18, 10, 23, 15};` declare the contents of the integer array **with** size 5.
- `int classroom[] = {15, 18, 10, 23, 15};` declare the contents of the integer array **without** explicit size.

Access array elements is just like in other programming languages:

- `classroom[2] += 2` adds 2 to the middle element of the array (2+10)

Passing an array to a function:

In the function definition below, we identify parameters as arrays by appending square brackets `[]` to the end of the parameter name. Note that if you want to know when to stop your for loop in C, you have to pass the length of the array as a parameter

```c
// this is a function that prints character arrays
void printArray(char dataName[], int dataSet[], int dataLength){
    for(int i =0; i<dataLength; i++){
        printf("%s[%d]:%d\n",dataName,i,dataSet[i])
    }
}
```

After calling `printArray` with the arguments listed in the excerpt below, `printArray` will print the contents of the `classroom[]` array to standard output .

```c
printArray("classrooms", classroom, 5);
/*output looks like:
classroom[0]: 15
classroom[1]: 18
classroom[2]: 10
classroom[3]: 23
classroom[4]: 15
*/
```

### Multi-Dimensional Arrays

- remember it's always `[rows][columns]` when accessing array elements (row-major order)
- declaring a multi-dimensional array in c is similar to regular arrays:

```c
// two row, three column 2-D array
#define NUM_ROWS 2
#define NUM_COLS 3
int table[2][3] = {
    {132, 142, 23},
    {0, 76, 872}
}
```

Passing a multi-Dimensional array to a function, because of the way memory is allocated in C, you have to stipulate the number of columns in your array when you define it as a parameter.The reason behind this will be explored in greater depth in the pointers and memory management section.

```c
void printTable(int table[][NUM_COLS]){
    printf("\n The Table:\n");
    for(int i=0;i<NUM_ROWS;i++){
        for(ing j=0; j<NUM_COLS; j++){
            printf("4%d",table[i][j]);
        }
        printf("\n")
    }
}
```



### All Arrays

for all arrays, the index has to be within the bounds of the array. First index is always 0 and the last is always the length of the array minus 1.



### Pointers
Initializing a pointer from a memory address.
```c
int number = 15; // variable that stores an int
int *numberPointer; //variable that stores address of type int
numberPointer = &number; //&number returns the address of number
```
To retrieve the value a pointer is stored at, use the `*` operator.

```c
printf("The hex address %x contains %d",numberPointer,*numberPointer);
//The hex address 54dec1c88 contains 15
```

Pointer arithmetic is extremely similar to arrays. When you initialize an array, what you are doing is grouping a set of memory locations under the name of the array where each memory location that makes up the array takes up as much space as is needed by the given data type.

```c
int sizes[] = {15, 20, 30};
int *ptr = sizes; //points to start of array
ptr++ //increments to next data element according to the size of int 
```

The advantage of `*` pointers over `[]` arrays is that any array declared must exist for the entire scope of the declaration. `int array[20];` allocates space for 20 integers that cannot be freed up until the function this array is declared in has returned. If the array is in static memory, then for the life of the entire program, that space is taken. Dynamic memory allocation with pointers allows us to manage our use of memory directly.

```c
int *array = malloc(20 * sizeOf(int)); //allocates 20 integer spaces in dynamic mem
free(array); // deallocates the memory (deletes array from dynamic mem)
```



### Pointers & structs

- say you have a pointer `foo` to a struct with attribute bar. 

- if you used `*foo.bar`  to access an attribute in the struct, the compiler would assume an address is stored in `foo.bar` and that you are trying to find the value of the object at that address.

- To access `bar` itself, when you need to dereference `foo` first with  `(*foo)` and then access the member element: `(*foo).bar`.
- This is clumsy to write so the shorthand version is `foo->bar`--member access by pointer operator.
- tldr `(*foo).bar` = `foo->bar`

### Pointers, Pass by Reference and Function calls

Functions with pointers as input parameters allow the function to modify the data that the pointers point to, instead of making a copy for the function to use.

```c
void function(int *a, int *b);
```

To call such a function on two integers `x` and `y`, for example, you have to pass by reference.

```c
int x = 5;
int y = 6;
function(&x, &y) // essentially asigning a = &x and b = &y
```

`&x` returns the address of `x`,  which is the address the pointer `a` will be set to point to; the same happens for `y` and `b`.



### Passing an array to a function

In function definitions, you can define array parameters in two ways, both of which accomplish the exact same thing.

```c
//function defintions
void function1(int array[]){};
void function2(int *array){};

//function usage
int newArray[] = {1,2,3,4,5};
function1(newArray);
function2(newArray); //you pass the array in the exact same way for both of them
```

### Returning Data from Functions

1. use return command
2. modify the value of a global variable (with a void function)
3. passing arguments by reference to modify them directly (also void)

### Function Prototype

I've always wondered about this. A function prototype's purpose is so that a function can be used in a line that comes before the function definition in code.

Function prototypes don't have to include parameter names, just their types.

```c
double function(double , double); //prototype
double function(double a, double b){ //definition
    /*code goes here*/
};
```

### Header Files

Header files let you

1. enable the use of functions defined in external files `#include "myCode.h"`
   - Function prototypes are in "myCode.h"
   - Functions are implemented in "myCode.c"
2. you can include code from external libraries `#include <cstdlib>`