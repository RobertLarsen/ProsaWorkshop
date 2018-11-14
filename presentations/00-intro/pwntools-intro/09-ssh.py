#!/usr/bin/env python2

from pwn import *

context(log_level = 'error')

doing = term.output(float = True)

solutions = (
    'cat $HOME/readme',
    'cat < -',
    'cat "spaces in this filename"',
    'find inhere -type f -name \'.*\' -exec cat {} \\;',
    'for file in inhere/*; do file -- ${file}; done | grep ASCII | awk -F: \'{print $1}\' | xargs cat'
)

def solve(name, password, cmd):
    doing.update('Solving %s' % name)
    con = ssh(user = name, password = password, host = 'bandit.labs.overthewire.org', port = 2220)
    shell = con.shell(tty = False)
    shell.sendline(cmd)
    next_password = shell.recvline().strip()
    shell.close()
    con.close()
    doing.update('%s gave us %s\n' % (name, next_password))
    return next_password

password = 'bandit0'
for i in range(0, len(solutions)):
    password = solve('bandit%d' % i, password, solutions[i])
