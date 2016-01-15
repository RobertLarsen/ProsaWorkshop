[SECTION .text]
bits 32
global _start
_start:
    jmp a         ;Jump to 'call'

b:  pop edx       ;Make edx point to /bin/bash
                  ;Do stuff
    xor eax, eax  ;Zero eax
    inc eax       ;eax = 1 = SYS_exit
    xor ebx, ebx  ;exit code
    int 0x80      ;syscall

a:  call b        ;Place address of /bin/bash on stack
db  "/bin/bash",0
