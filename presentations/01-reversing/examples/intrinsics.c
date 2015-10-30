#include <string.h>
void strcpy_short() {
    char buffer[128];
    strcpy(buffer, "Hello, World!");
}

void strcpy_long() {
    char buffer[128];
    /* 85 chars + nul terminator */
    strcpy(buffer, "This is a much longer string "
                   "so maybe it will be handled "
                   "differently by the compiler?");
}
