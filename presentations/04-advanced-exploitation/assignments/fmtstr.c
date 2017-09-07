// gcc -O3 -pie -fPIC fmtstr.c -o fmtstr
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char flag3[100];

int main(int argc, char **argv){
    char *flag2_ptr;
    char *flag3_ptr;
    char flag1[100];
    char buffer[100];

    alarm(60); // One minute to get all the flags

    // Put the first flag on the stack
    if(getenv("FLAG1")){
        strncpy(flag1, getenv("FLAG1"), sizeof(flag1));
    }

    // So where is this flag?
    flag2_ptr = getenv("FLAG2");

    // OMG! Have you seen my .bss section?
    flag3_ptr = getenv("FLAG3");
    if(flag3_ptr){
        strncpy(flag3, flag3_ptr, sizeof(flag3));
        memset(flag3_ptr, 0, strlen(flag3_ptr));
    }

    // flag4 is in /home/fmtstr, you need to pop a shell...

    // happy %%%dc%%%d$hhn'ing
    while(memcmp(buffer, "exit", 4)){
        memset(buffer, 0, sizeof(buffer));
        read(0, buffer, sizeof(buffer));
        buffer[sizeof(buffer)-1] = 0;
        dprintf(1, buffer);
    }

    return 0;
}
