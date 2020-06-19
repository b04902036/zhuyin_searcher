#!/usr/bin/env python
import re
import pickle
from collections import defaultdict


class SearchZhuyin():
    # CHANGE THESE
    ZHUYIN_DICTIONARY_PATH='./total.txt'
    EXCEPTION_WORD_PATH='../exception/exceptionWord.txt'

    # DO NOT CHANGE THESE
    ZHUYIN='ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ˙ˊˇˋ-'
    ENGLISH='1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634 '
    END='˙ˊˇˋ-'

    def __init__(self):
        self.position = []
        self.length = 0
        self.zhuyin_ans = []
        self.english_ans = ''
        self.english_to_zhuyin = defaultdict(str)
        for (zhuyin, english) in zip(SearchZhuyin.ZHUYIN, SearchZhuyin.ENGLISH):
            self.english_to_zhuyin[english] = zhuyin
            self.english_to_zhuyin[english.upper()] = zhuyin

        self.total_zhuyin = set()
        with open(SearchZhuyin.ZHUYIN_DICTIONARY_PATH) as zhuyins:
            with open(SearchZhuyin.EXCEPTION_WORD_PATH) as exceptions:
                exception = set(exceptions.read().strip().split('\n'))
                for zhuyin in zhuyins:
                    if zhuyin.strip() in exception:
                        continue

                    self.total_zhuyin.add(zhuyin.strip())

    def full_chinese(self):
        r"""
        After using :self.search: to parse password string, this function will check if the detected zhuyin forms a "only chinese password"

        :return:
            :ans: str. If this is a password contains only chinese, return the password constructed using zhuyin chars.
                       Otherwise, return an empty string
        """
        if len(self.english_ans) == self.length:
            ans = ''.join(self.zhuyin_ans)
        else:
            ans = ''

        return ans


    def two_connected(self, inp):
        cnt = 0
        prev = -2
        ans = ''
        for p in self.position:
            if p[0] - prev == 1:
                cnt += 1
                if cnt > 0:
                    break
            prev = p[1]
        if cnt == 0:
            return ''
        else:
            now = 0
            zhuyin_now = 0
            for i in range(len(self.position)):
                ans += inp[now:self.position[i][0]]
                now = self.position[i][1] + 1
                ans += self.zhuyin_ans[zhuyin_now:zhuyin_now+self.position[i][1]-self.position[i][0]+1]
                zhuyin_now += self.position[i][1] - self.position[i][0] + 1
            ans += inp[now:]
            return ans

    def three_connected(self, inp):
        cnt = 0
        prev = -2
        ans = ''
        for p in self.position:
            if p[0] - prev == 1:
                cnt += 1
                if cnt > 1:
                    break
            else:
                cnt = 0
            prev = p[1]
        if cnt < 2:
            return ''
        else:
            now = 0
            for i in range(len(self.position)):
                ans += inp[now:self.position[i][0]]
                now = self.position[i][1] + 1
                ans += self.zhuyin_ans[i]
            ans += inp[now:]
            return ans

    def only_english_plus_only_digit(self, inp):
        r"""
        Pre-filter
        Given an input password string, if it is constructed using one english string followed by one digit strings (for example, asd123), at most case this should not be a zhuyin password, so we pre-filter it.

        :param:
            :inp: str. Input password string

        :return:
            :ret: bool. If this is a string constructed using one english string followed by one digit string (In short, it is going to be filtered out), return True
                        Otherwisw, return False
        """
        if re.match(r'^[a-zA-Z]*[0-9]*$', inp):
            return True
        return False

    def search(self, inp):
        r"""
        Given an input password string, change possible zhuyin typing into zhuyin from english. For example, given 'ji394su3123', return 'ㄨㄛˇㄞˋㄋㄧˇ123'

        :param:
            :inp: str. Input password string

        :return:
            :success: bool. If zhuyin found, return True.
                            Otherwise, return False
            :ans: str. Replace chars with possible zhuyin
        """
        self.zhuyin_ans = []
        self.english_ans = ''
        self.position = []
        self.length = len(inp)

        if self.only_english_plus_only_digit(inp):
            return False, inp

        for idx in range(self.length):
            ret = self.english_to_zhuyin[inp[idx]]
            if len(ret) == 0:
                continue

            # if this charater is in END, which means we may be
            # able to construct a valid chinese word
            search = 1
            if ret in SearchZhuyin.END:
                while search < 4 and idx - search > -1:
                    tmp = self.english_to_zhuyin[inp[idx-search]]
                    if len(tmp) > 0:
                        ret = tmp + ret
                    else:
                        break

                    search += 1

                if search == 1:
                    continue

                for i in range(search-1):
                    if ret[i:] in self.total_zhuyin:
                        self.zhuyin_ans.append(ret[i:])
                        self.english_ans += inp[idx-search+i+1:idx+1]
                        self.position.append((idx-search+i+1, idx))
                        break


        if len(self.position) == 0:
            return False, inp


        # Because some of the password does not contain '-'
        # (for example "我是豬" may become "ji3g45j", instead of "ji3g45j ")
        # we try to insert '-' here between two possible chinese word
        position = []
        zhuyin_ans = []
        now = 0
        for idx, p in enumerate(self.position):
            if p[0] - now < 4:
                try_to_translate = ''
                for c in inp[now:p[0]]:
                    try_to_translate += self.english_to_zhuyin[c]
                if len(try_to_translate) == p[0] - now:
                    try_to_translate += '-'
                    if try_to_translate in self.total_zhuyin:
                        position.append((now, p[0]-1))
                        zhuyin_ans.append(try_to_translate[:-1])
            position.append(p)
            zhuyin_ans.append(self.zhuyin_ans[idx])
            now = p[1] + 1

            if idx == len(self.position) - 1:
                if self.length - p[1] - 1 < 4:
                    try_to_translate = ''
                    for c in inp[p[1]+1:]:
                        try_to_translate += self.english_to_zhuyin[c]
                    if len(try_to_translate) == self.length - p[1] - 1:
                        try_to_translate += '-'
                        if try_to_translate in self.total_zhuyin:
                            position.append((p[1]+1, self.length-1))
                            zhuyin_ans.append(try_to_translate[:-1])
        self.position = position
        self.zhuyin_ans = zhuyin_ans

        if self.english_ans.isdigit():
            return False, inp

        ret = self.full_chinese()
        if len(ret) > 0:
            return True, ret

        ret = self.three_connected(inp)
        if len(ret) > 0:
            return True, ret

        # ret = self.two_connected(inp)
        # if len(ret) > 0:
        #     return True, ret


        return False, inp
