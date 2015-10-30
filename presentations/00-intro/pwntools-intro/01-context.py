#!/usr/bin/env python2

from pwn import *

context(endian='be')
print 'Big endian: ' + repr(p32(7))

context(endian='le')
print 'Little endian: ' + repr(p32(7))
