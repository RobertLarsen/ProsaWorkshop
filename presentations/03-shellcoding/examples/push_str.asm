[SECTION .text]
bits 32
global _start
_start:
    push 0x64
    push 0x6c726f77
    push 0x1010101
    xor [esp], dword 0x16d6474
    push 0x1010101
    xor [esp], dword 0x7362016e
    push 0x6c6c6568
    ;Now esp points to first string
    ;Second string lies six bytes past esp
    ;Third string lies 12 bytes past esp

    xor eax, eax      ;Zero out eax
    inc eax           ;eax = 1 = SYS_exit
    xor ebx, ebx      ;exit code
    int 0x80          ;syscall
