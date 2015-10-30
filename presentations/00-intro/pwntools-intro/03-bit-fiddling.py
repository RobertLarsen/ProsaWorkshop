#!/usr/bin/env python2

from pwn import *

my_packer = make_packer(word_size = 32, endianness = 'little', sign = 'yes')
binary = my_packer(-1)
print 'Binary: %s' % repr(binary)

print 'Unsigned: %d' % u32(binary)
print 'Signed: %d' % u32(binary, sign = 'yes')
