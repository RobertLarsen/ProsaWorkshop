#include <string.h>
void prologue_epilogue(int a, int b, int c) {
    int local1, local2;
    char string[32];
    local1 = a + b * c;
    local2 = a * c + b;
    strcpy(string, "Hello, World!");
}
