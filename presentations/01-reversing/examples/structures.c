#include <malloc.h>

typedef struct {     /* Size in bytes * Packed index * Aligned index */
    char first;      /*      1        *        0     *       0       */ 
    int second;      /*      4        *        1     *       4       */
    short third;     /*      2        *        5     *       8       */
    long long fourth;/*      8        *        7     *      12       */
} /* __attribute__((packed)) */ some_struct_t;

some_struct_t global;

void struct_global() {
    global.first = 10; global.second = 20;
    global.third = 30; global.fourth = 40;
}

void struct_stack() {
    some_struct_t stack;
    stack.first = 10; stack.second = 20;
    stack.third = 30; stack.fourth = 40;
}

void struct_heap() {
    some_struct_t * heap = malloc(sizeof(some_struct_t));
    heap->first = 10; heap->second = 20;
    heap->third = 30; heap->fourth = 40;
}
