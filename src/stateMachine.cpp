#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>

#include "stateMachine.h"
#include "util.h"
#include <algorithm>

extern Util util;

void StateMachine::reset() {
    englishAns.clear();
    zhuyinAns.clear();
    chineseAns.clear();
    current.clear();
    length = 0;
    position.clear();
    currentState = State::EMPTY;
}

void StateMachine::process(std::wstring input) {
    std::size_t totalLength = input.length();
    std::wstring ret, tmp;
    int search;

    if (filter(input)) {
        return;
    }
    reset();
    for (int idx = 0; idx < totalLength; ++idx) {
        ret = util.englishToZhuyin(input.substr(idx, 1));
        if (ret.length() == 0) {
            continue;
        } 
        if (util.isHead(ret)) {
            if (currentState != State::EMPTY) {
                current += L"-";
                search = searchChinese();
                if (search > 1) {
                    englishAns += input.substr(idx-search+1, search-1);
                    zhuyinAns += current.substr(current.length()-search);
                    chineseAns += currentAns;
                    length += 1;
                    position.push_back(std::pair<int, int>(idx-search+1, idx-1));
                }
                current.clear();
            }
            current += ret;
            currentState = State::HEAD;
        }
        else if (util.isMiddle(ret)) {
            if (currentState == State::EMPTY || currentState == State::HEAD) {
                current += ret;
            }
            else {
                current += L"-";
                search = searchChinese();
                if (search > 1) {
                    englishAns += input.substr(idx-search+1, search-1);
                    zhuyinAns += current.substr(current.length()-search);
                    chineseAns += currentAns;
                    length += 1;
                    position.push_back(std::pair<int, int>(idx-search+1, idx-1));
                }
                current.clear();
                current += ret;
            }
            currentState = State::MIDDLE;
        }
        else if (util.isBottom(ret)) {
            if (currentState != State::BOTTOM) {
                current += ret;
            }
            else {
                current += L"-";
                search = searchChinese();
                if (search > 1) {
                    englishAns += input.substr(idx-search+1, search-1);
                    zhuyinAns += current.substr(current.length()-search);
                    chineseAns += currentAns;
                    length += 1;
                    position.push_back(std::pair<int, int>(idx-search+1, idx-1));
                }
                current.clear();
                current += ret;
            }
            currentState = State::BOTTOM;
        }
        else {
            // TODO:
            // current solution does not consider case like this:
            //  ㄊ一ㄣˋ -> ㄊ一 ㄣˋ (correct)
            //          -> 一ㄣˋ (current solution, which is wrong)
            current += ret;
            search = searchChinese();
            if (search > 1) {
                englishAns += input.substr(idx-search+1, search);
                zhuyinAns += current.substr(current.length()-search);
                chineseAns += currentAns;
                length += 1;
                position.push_back(std::pair<int, int>(idx-search+1, idx));
            } 
            current.clear();
            currentState = State::EMPTY;
        }
    }
    
    if (length == 0) {
        return;
    }

    bool sign = false;
    for (auto c: englishAns) {
        if (!iswdigit(c)) {
            sign = true;
            break;
        }
    }

    if (sign) {
        int cnt = 0;
        int prev = -2;
        for (auto p: position) {
            if (p.first - prev == 1) {
                ++cnt;
            }
            prev = p.second;
        }
        if (cnt == 0 || cnt != position.size() - 1) {
            return;
        }
        
        std::wstring tmpChineseAns;
        int now = 0;
        for (int i = 0; i < chineseAns.length(); ++i) {
            tmpChineseAns += input.substr(now, position[i].first-now);
            now = position[i].second + 1;
            tmpChineseAns += chineseAns[i];
        }
        tmpChineseAns += input.substr(now);
        ans += L"password: " + tmpChineseAns + L", zhuyin: " + zhuyinAns + L", origin: " + englishAns + L", total: " + input  + L",EOF\n";
        ++size;
        
        // output to file if the string is too long
        if (size > 10000) {
            output << ans;
            ans = L"";
            size = 0;
        }
    } 
}

/*
 * find possible chinese word from self->current
 * return first possible chinese word's length (zhuyin length)
 * set the chinese word in :self->currentAns:
 */
int StateMachine::searchChinese() {
    std::wstring tmp;
    int search = current.length();
    for (int i = 0; i < search - 1; ++i) {
        if (util.isExceptionWord(current.substr(i))) {
            continue;
        }
        tmp = util.zhuyinToChinese(current.substr(i));    
        if (tmp.length() > 0) {
            currentAns = tmp;
            return search - i;
        }
    }
    return 0;
}

bool StateMachine::filter(std::wstring input) {
    std::size_t length = input.length();
    int idx = -1;
    if (length == 0) {
        return true;
    } 
    if (!iswalpha(input[0])) {
        return false;
    }

    for (int i = 1; i < length; ++i) {
        if (!iswalpha(input[i])) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        return true;
    }

    for (int i = idx; i < length; ++i) {
        if (!iswdigit(input[i])) {
            return false;
        } 
    }
    return true;
}


void StateMachine::print() {
    output << ans;
    output.close();
}

StateMachine::StateMachine(int idx) {
    const std::locale utf8_locale
        = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    std::ostringstream stringIdx;
    stringIdx << outputPath << idx;
    output.open(stringIdx.str());
    output.imbue(utf8_locale);
    if (!output.is_open()) {
        std::cerr << "open " << stringIdx.str() << " failed" << std::endl;
        exit(255);
    }
}

StateMachine::~StateMachine() {
    output.close();
}

