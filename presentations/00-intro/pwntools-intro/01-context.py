#!/usr/bin/env python3

from pwn import *

context(endian='be')
print('Big endian: ' + repr(p32(7)))

context(endian='le')
print('Little endian: ' + repr(p32(7)))
