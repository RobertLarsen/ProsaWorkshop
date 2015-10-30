#!/usr/bin/env python2

from pwn import *

context(endian='le')
print 'Little endian 32 bit: ' + repr(flat(1,2,3,'Hello, World',4))

context(endian='le', word_size=16)
print 'Little endian 16 bit: ' + repr(flat(1,2,3,'Hello, World',4))
