#!/usr/bin/env python3
import os
import re
import sys
assert sys.version >= '3.6'

CONST_CHOICE = 5
BASE_DIR = os.path.dirname(os.path.realpath(__file__))
EXCEPTION_SENTENCE = os.path.join(BASE_DIR, 'exceptionSentence.txt')
EXCEPTION_WORD = os.path.join(BASE_DIR, 'exceptionWord.txt')
TMP = os.path.join(BASE_DIR, 'tmp')

def parse_zhuyin(line, length):
    """
    given one line in result, and chinese sentence length, parse corresponding zhuyin out

    :param:
        :line: str. One line in result.
        :length: list[int]. length of each chinese sentence in this line

    :return:
        :zhuyin: list[str]. corresponding zhuyin
    """
    END = '˙ˊˇˋ-'
    line = re.findall('zhuyin: .*, origin: ', line)[0][8:-10]
    zhuyin = []
    idx = 0
    for num in length:
        now = ''
        now_length = 0
        for i in range(idx, len(line)):
            now += line[i]

            if line[i] in END:
                now_length += 1
                if now_length == num:
                    zhuyin.append(now)
                    idx = i + 1
                    break
    return zhuyin




if __name__ == '__main__':
    if len(sys.argv) < 2:
        print ('usage: python3 ./add.py <path/to/result>\n\te.g. python3.6 ./add.py ../../result/OnlyChinese.txt')
        exit(255)

    result = open(sys.argv[1])

    while True:
        line = result.readline()
        if not line:
            break

        length = []
        prev = 0
        others = re.findall('[ -~]+', line)
        for other in others:
            now = line[prev:].find(other) + prev
            if now - prev > 0:
                length.append(now - prev)

            prev = now + len(other)

        zhuyins = parse_zhuyin(line, length)



        while True:
            one_word = []
            print ('')
            print (line)
            print (f'[1] show tail of exception sentence list')
            print (f'[2] delete tail of exception sentence list')
            print (f'[3] show tail of exception word list')
            print (f'[4] delete tail of exception word list')
            print (f'[5] next line')
            for i, zhuyin in enumerate(zhuyins):
                print (f'[{i+CONST_CHOICE+1}] add {zhuyin} into exception sentence list')
            for i, zhuyin in enumerate(zhuyins):
                if length[i] == 1:
                    print (f'[{i+CONST_CHOICE+1+len(zhuyins)}] add {zhuyin} into exception word list')
                    one_word.append(zhuyin)

            try:
                choice = int(input('> '))
                if choice == 1:
                    os.system(f'tail {EXCEPTION_SENTENCE}')
                elif choice == 2:
                    os.system(f'head -n -1 {EXCEPTION_SENTENCE} > {EXCEPTION_SENTENCE}.tmp; mv {EXCEPTION_SENTENCE}.tmp {EXCEPTION_SENTENCE}')
                if choice == 3:
                    os.system(f'tail {EXCEPTION_WORD}')
                elif choice == 4:
                    os.system(f'head -n -1 {EXCEPTION_WORD} > {EXCEPTION_WORD}.tmp; mv {EXCEPTION_WORD}.tmp {EXCEPTION_WORD}')
                elif choice == 5:
                    break
                else:
                    choice -= (CONST_CHOICE+1)
                    if len(one_word) == 0 and choice > len(zhuyins) - 1:
                        print (f'[!] {choice+CONST_CHOICE+1} is out of the range. Should be in {CONST_CHOICE+1} ~ {len(zhuyins)+CONST_CHOICE} inclusive')
                        continue
                    elif choice < len(zhuyins):
                        with open(f'{TMP}', 'w') as f:
                            f.write(zhuyins[choice]+'\n')
                        os.system(f'cat {EXCEPTION_SENTENCE} {TMP} > {EXCEPTION_SENTENCE}.tmp; mv {EXCEPTION_SENTENCE}.tmp {EXCEPTION_SENTENCE}; rm {TMP}')
                        print (f'[+] {zhuyins[choice]} added successfully. You can check it by input 1')
                    else:
                        choice -= len(zhuyins)
                        if choice > len(one_word) - 1:
                            print (f'[!] {choice+len(zhuyins)+CONST_CHOICE+1} is out of the range. Should be in {len(zhuyins)+CONST_CHOICE+1} ~ {len(one_word)+len(zhuyins)+CONST_CHOICE} inclusive')
                            continue
                        with open(f'{TMP}', 'w') as f:
                            f.write(one_word[choice]+'\n')
                        os.system(f'cat {EXCEPTION_WORD} {TMP} > {EXCEPTION_WORD}.tmp; mv {EXCEPTION_WORD}.tmp {EXCEPTION_WORD}; rm {TMP}')
                        print (f'[+] {one_word[choice]} added successfully. You can check it by input 3')

            except KeyboardInterrupt:
                print (f'[-] Receive keyboard interrupt')
                exit(0)
            except:
                print (f'[!] Please input integer')

