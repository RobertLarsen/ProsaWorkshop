#!/usr/bin/env python2

from pwn import *

for arch, mnemonic in [('i386', 'mov eax, 42'),
                       ('amd64', 'mov rax, 42'),
                       ('arm', 'mov r0, 42'),
                       ('thumb', 'mov r0, 42'),
                       ('mips', 'ori $a0, $zero, 42')]:
    with context.local(arch = arch):
        print '%s(%s): %s' % (arch, mnemonic, enhex(asm(mnemonic)))
