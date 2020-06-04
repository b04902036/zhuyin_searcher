#!/usr/bin/env python3
with open('merged.dict', 'r') as f:
    x = eval(f.read())

key = open('zhuyin.txt', 'w')
value = open('chinese.txt', 'w')

for k, v in x.items():
    key.write(k + '\n')
    value.write(v + '\n')

key.close()
value.close()

