#!/usr/bin/env python3
from search import SearchZhuyin

x = SearchZhuyin()

while True:
    now = input('now : ').strip('\n')
    print (x.search(now))
