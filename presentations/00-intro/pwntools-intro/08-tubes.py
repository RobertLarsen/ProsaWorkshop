#!/usr/bin/env python2

from pwn import *
import sys

def brainfuck(s):
    cur = 0
    code = ''
    for c in s:
        v = ord(c)
        d = v - cur
        if d > 0:
            code += '+' * d
        else:
            code += '-' * (-d)
        cur = v
        code += '.'
    return code

if len(sys.argv) > 1 and sys.argv[1] == 'remote':
    tube = remote('pwnable.kr', 9001)
else:
    tube = process('./bf')

print tube.recvuntil(']\n')
tube.send(brainfuck("Hello, World!") + '\n')
print tube.recvall()
