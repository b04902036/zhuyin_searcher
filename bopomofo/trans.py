#!/usr/bin/env python3

import os
import subprocess as sp
import json
import sys

def load(filename):
    return json.loads(open(filename).read())

def process(src_words):
    HEAD = 'ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙ'
    MIDDLE = 'ㄧㄨㄩ'
    BOTTOM = 'ㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ'
    END = '˙ˊˇˋ-'
    return_words = {}
    for x in words:
        word = x
        pinin = words[word]
        pinin = pinin.replace("（語音）", '').replace("（變）", '').replace('\n', '')
        if '(' in word or '(' in pinin or '音' in pinin:
            continue
        tmp_pinin = pinin
        pinin = pinin.split('\u3000')
        if len(word) != len(pinin):
            continue
        for i, single_pinin in enumerate(pinin):
            if 'ˊ' not in single_pinin and 'ˇ' not in single_pinin and 'ˋ' not in single_pinin and '˙' not in single_pinin:
                pinin[i] = pinin[i] + "-"
        for i, p in enumerate(pinin):
            if p.startswith('˙'):
                continue
            final_p = ''
            now = 5
            to_continue = False
            for c in reversed(p):
                c_now = 0
                if c in END:
                    c_now = 4
                elif c in BOTTOM:
                    c_now = 3
                elif c in MIDDLE:
                    c_now = 2
                elif c in HEAD:
                    c_now = 1
                else:
                    to_continue = True
                    break
                if c_now >= now:
                    break
                now = c_now
                final_p = c + final_p
            if to_continue:
                continue
            # if final_p == 'ㄋㄢ':
            #     print (word, tmp_pinin)
            #     exit()
            return_words[final_p] = word[i]
    return return_words

def save(filename, words):
    with open(filename, 'w') as f:
        f.write(json.dumps(words))



try:
    os.makedirs('./dict')
except:
    pass

for root, dirnames, filenames in os.walk("./src_dict"):
    for filename in filenames:
        words = load('./src_dict/%s'%(filename))
        words = process(words)
        save('./dict/%s'%(filename), words)

