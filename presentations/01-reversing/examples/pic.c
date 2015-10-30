#include <stdio.h>

static int some_global = 42;

int pic_function1() {
    return some_global;
}

int pic_function2() {
    return some_global;
}
