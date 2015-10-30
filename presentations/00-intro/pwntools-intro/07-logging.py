#!/usr/bin/env python2

from pwn import *
import sys

#Uncomment next line for more silliness
#splash()

def brute(alphabet = 'abcdefghijklmnopqrstuvwxyz'):
    indices = []
    while True:
        carry = True
        for i in range(0, len(indices)):
            indices[i] += 1
            if indices[i] == len(alphabet):
                indices[i] = 0
            else:
                carry = False
                break
        if carry:
            indices.append(0)
        yield ''.join([alphabet[indices[i]] for i in range(len(indices) - 1, -1, -1)])

if len(sys.argv) > 1:
    if sys.argv[1].upper() in ['CRITICAL', 'DEBUG', 'ERROR', 'INFO', 'NOTSET', 'WARN', 'WARNING']:
        context.log_level = sys.argv[1]

log.info('Starting long running job')
with log.progress('Brute forcing') as bf:
    with log.progress('Count') as c:
        count = 0
        for s in brute():
            count += 1
            if count % 5000 == 0:
                bf.status(s)
                c.status('%d' % count)
            if s == 'hello':
                bf.success('Got it in the %dth try: %s' % (count,s))
                c.success('%d' % count)
                sys.exit(0)
