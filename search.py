#!/usr/bin/env python3.7
# -*- coding: utf-8 -*-

import os
import sys
assert sys.version >= '3.6'
import pickle
import pyautogui

from enum import Enum

class InputState(Enum):
    START = 'START'
    HEAD = 'HEAD'
    HEAD_MIDDLE = 'HEAD_MIDDLE'
    HEAD_MIDDLE_BOTTOM = 'HEAD_MIDDLE_BOTTOM'
    HEAD_BOTTOM = 'HEAD_BOTTOM'
    MIDDLE = 'MIDDLE'
    MIDDLE_BOTTOM = 'MIDDLE_BOTTOM'
    BOTTOM = 'BOTTOM'
    END = 'END'
    ERROR = 'ERROR'

z='ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ˙ˊˇˋ-'
e='1QAZ2WSXEDCRFV5TGBYHNUJM8IK,9OL.0P;/-7634 '

def step_end(state):
    if state == InputState.HEAD:
        return InputState.ERROR
    elif state == InputState.START:
        return InputState.ERROR
    else:
        return InputState.END

def step_bottom(state):
    if state == InputState.START:
        return InputState.BOTTOM
    elif state == InputState.HEAD:
        return InputState.HEAD_BOTTOM
    elif state == InputState.HEAD_MIDDLE:
        return InputState.HEAD_MIDDLE_BOTTOM
    elif state == InputState.MIDDLE:
        return InputState.MIDDLE_BOTTOM
    else:
        return InputState.ERROR

def step_middle(state):
    if state == InputState.START:
        return InputState.MIDDLE
    elif state == InputState.HEAD:
        return InputState.HEAD_MIDDLE
    else:
        return InputState.ERROR

def step_head(state):
    if state == InputState.START:
        return InputState.HEAD
    else:
        return InputState.ERROR

def step(state, c, zhuyin_to_english):
    HEAD = set([zhuyin_to_english[x] for x in u'ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙ'])
    MIDDLE = set([zhuyin_to_english[x] for x in u'ㄧㄨㄩ'])
    BOTTOM = set([zhuyin_to_english[x] for x in u'ㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ'])
    END = set([zhuyin_to_english[x] for x in '˙ˊˇˋ-'])

    if c in HEAD:
        state = step_head(state)
    elif c in MIDDLE:
        state = step_middle(state)
    elif c in BOTTOM:
        state = step_bottom(state)
    elif c in END:
        state = step_end(state)
    else:
        state = InputState.ERROR

    return state


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print ('usage : python[>=3.7] search.py <path/to/BreachCompilation/data\n\te.g. python3.7 search.py ../BreachCompilation/data/y/z')
        exit()

    with open(sys.argv[1], 'rb') as f:
        to_check = f.read().strip(b'\n').split(b'\n')

    if not os.path.isfile('zhuyin-english_mapping.pkl'):
        zhuyin_to_english = {}
        english_to_zhuyin = {}
        for k, v in zip(e, z):
            zhuyin_to_english[v] = k
            english_to_zhuyin[k] = v
        with open('zhuyin-english_mapping.pkl', 'wb') as f:
            pickle.dump(zhuyin_to_english, f)
            pickle.dump(english_to_zhuyin, f)
    else:
        with open('zhuyin-english_mapping.pkl', 'rb') as f:
            zhuyin_to_english = pickle.load(f)
            english_to_zhuyin = pickle.load(f)

    with open('bopomofo/merged.dict', 'r') as f:
        merged = eval(f.read())


    print (to_check[0].decode().strip('\n'))


    for line in to_check:
        # check if we can decode it to unicode
        # if failed, it is not so likely to be a Zhuyin password...
        try:
            line = line.decode().strip('\n')
        except:
            continue

        line = line.split(':')
        if len(line) < 2:
            line = line[0].split(';')
        if len(line) < 2:
            continue

        try:
            email, password = line[0], ''.join(line[1:])
        except:
            continue

        ans = ''
        prev = ''
        length = 0
        state = InputState.START
        for c in password:
            state = step(state, c.upper(), zhuyin_to_english)
            if state == InputState.ERROR:
                state = InputState.START
                prev = ''
                continue

            prev += c
            if state == InputState.END:
                try:
                    error = int(prev)
                except:
                    zhuyin = ''.join([english_to_zhuyin[x.upper()] for x in prev])
                    if zhuyin in merged:
                        ans += prev
                        length += 1
                prev = ''
                state = InputState.START

        if length > 1:
            print (f'email: {email}, password: {password}, zhuyin: {ans}')
        ans += '\n'

import time
print ('確保你現在是中文輸入法而且大寫要關閉!')
while True:
    s = input(f'give me the string you want to type (e.g. ji3g45j ) : ').lower()
    pyautogui.typewrite(s + '\n\n')
    ans = input('')
    print (f'the zhuyin is actually {ans}')
    print (f'')

