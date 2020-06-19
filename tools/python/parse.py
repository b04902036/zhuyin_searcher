#!/usr/bin/env python3

with open('total_zhuyin.txt') as f:
    x = f.read().strip().split('\n')


with open('total.txt', 'w') as f:
    for line in x:
        head = line.find(' ')
        head = line[:head]
        head2 = line.find('(') + 1
        head2 = line[head2:-1]
        if len(head2) != 5:
            print (head2)
            exit()
        assert len(head2) == 5
        if head2[0] == '＿':
            f.write(head + '-' + '\n')
        if head2[1] == 'ˊ':
            f.write(head + head2[1] + '\n')
        if head2[2] == 'ˇ':
            f.write(head + head2[2] + '\n')
        if head2[3] == 'ˋ':
            f.write(head + head2[3] + '\n')
        if head2[4] == '˙':
            f.write(head + head2[4] + '\n')

