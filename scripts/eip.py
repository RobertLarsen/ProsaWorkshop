#!/usr/bin/env python2

import sys

if len(sys.argv) < 2:
    sys.stderr.write('Usage: %s <line>' % sys.argv[0])
else:
    line = int(sys.argv[1])
    lines = sys.stdin.read().split('\n')

    if line < 0:
        line += len(lines)
    for i in range(len(lines)):
        if i == 0:
            print lines[i]
        elif i == line:
            print 'eip -> ' + lines[i]
        else:
            print '       ' + lines[i]
