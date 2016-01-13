#!/usr/bin/env python2

import subprocess, sys, re

def detect_functions(code):
    functions = []
    for line in code.split('\n'):
        m = re.match('^[^\s].*\s+([^\s]+)\(.*{$', line)
        if m:
            functions.append(m.group(1))
    return functions


if len(sys.argv) < 2:
    sys.stderr.write('Usage: %s <file>' % sys.argv[0])
else:
    c_file = sys.argv[1]
    idx_slash = c_file.rfind('/')
    idx_period = c_file.rfind('.')
    lib_file = '%s/lib%s.so' % (c_file[0:idx_slash], c_file[idx_slash + 1:idx_period])
    subprocess.call(['gcc'] + sys.argv[2:] + ['-m32', '-shared', '-fno-stack-protector', '-o', lib_file, c_file])
    p = subprocess.Popen(['objdump', '-Mintel', '-d', lib_file], stdout = subprocess.PIPE)
    functions = detect_functions(open(c_file).read())

    do_print = False
    for line in p.stdout:
        line = line.strip()
        if not do_print:
            m = re.search('<(.*)>:$', line)
            if m and m.group(1) in functions:
                do_print = True

        if do_print:
            m = re.match('^([0-9a-f]+:)\t( ?[0-9a-f]{2})+(\s+(.*))?$', line)
            if m:
                if not m.group(4) is None:
                    print '%s  %s' % (m.group(1), m.group(4))
            else:
                print line
            if line == '':
                do_print = False
