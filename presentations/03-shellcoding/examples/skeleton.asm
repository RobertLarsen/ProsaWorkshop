[SECTION .text]
bits 32
global _start
_start:
    xor eax, eax  ;Zero out eax
    inc eax       ;eax = 1 = SYS_exit
    xor ebx, ebx  ;exit code
    int 0x80      ;syscall
