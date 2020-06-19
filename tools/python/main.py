#!/usr/bin/env python3
import sys

from search import SearchZhuyin


with open(sys.argv[1], 'r') as f:
    lines = f.read().split('\n')


searcher = SearchZhuyin()
for line in lines:
    off1 = line.find('@')
    if off1 < 0:
        continue
    off2 = line[off1:].find(':')
    if off2 < 0:
        off2 = line[off1:].find(';')
    if off2 < 0:
        continue

    off2 += off1 + 1
    password = line[off2:]
    if password.endswith(' '):
        print (password)
    continue
    success, ret = searcher.search(password)

    if success:
        print (ret)
