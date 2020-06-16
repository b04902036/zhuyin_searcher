#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as f:
    x = f.read().split('\n')

start = 0
if len(sys.argv) > 2:
    start = int(sys.argv[2])
print (f'start from line {start}')

with open('1.txt', 'w') as f:
    for i in range(start, 325):
        ret = input(f'{i:03d} [{x[i]}] is this rare word ? [y/N]').strip()
        if 'y' in ret.lower():
            f.write(x[i] + '\n')

