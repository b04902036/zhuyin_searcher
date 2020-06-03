#!/usr/bin/env python3
import os
import glob
import json

fs = glob.glob('./dict/*.dict')
total = {}
for f in fs:
    print (f)
    with open(f, 'r') as ff:
        x = eval(ff.read())
    for k in x:
        total[k] = x[k]

print (len(total))
with open('merged.dict', 'w') as f:
    f.write(str(total))
