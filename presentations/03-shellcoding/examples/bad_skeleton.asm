[SECTION .text]
bits 32
global _start
_start:
    mov eax, 1    ;SYS_exit
    xor ebx, ebx  ;exit code
    int 0x80      ;syscall
