[SECTION .text]
bits 32
global _start
_start:
    jmp a             ;Jump to 'call'

b:  pop edx           ;Make edx point to string
    xor eax, eax      ;Make eax nul
    mov [edx+5], al   ;Replace first X with a null byte
    mov [edx+11], al  ;Replace second X with a null byte
    mov [edx+17], al  ;Replace third X with a null byte
    ;Now edx points to first string
    ;Second string lies six bytes past edx
    ;Third string lies 12 bytes past edx

    xor eax, eax      ;Zero out eax
    inc eax           ;eax = 1 = SYS_exit
    xor ebx, ebx      ;exit code
    int 0x80          ;syscall

a:  call b
db  "helloXcruelXworldX"
