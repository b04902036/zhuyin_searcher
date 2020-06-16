#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>

#include "atLeastTwo.h"
#include "util.h"
#include <algorithm>

extern Util util;

void AtLeastTwo::reset() {
    englishAns.clear();
    zhuyinAns.clear();
    chineseAns.clear();
    length = 0;
    position.clear();
}

void AtLeastTwo::process(std::wstring input) {
    std::size_t totalLength = input.length();
    std::wstring ret, tmp;
    int search;

    reset();
    if(filter(input)) {
        return;
    }
    for (int idx = 0; idx < totalLength; ++idx) {
        ret = util.englishToZhuyin(input.substr(idx, 1));
        if (ret.length() == 0) {
            continue;
        } 
        
        if (util.isEnd(ret)) {
            for (search = 1; search < 4, idx - search > -1; ++search) {
                tmp = util.englishToZhuyin(input.substr(idx-search, 1));

                if (tmp.length() > 0) {
                    ret = tmp + ret;
                }
                else {
                    break;
                }
            }
            
            if (search == 1) {
                continue;
            }
            
            for (int i = 0; i < search - 1; ++i) {
                if (util.isExceptionWord(ret.substr(i))) {
                    continue;
                }
                tmp = util.zhuyinToChinese(ret.substr(i));    
                if (tmp.length() > 0) {
                    englishAns += input.substr(idx-search+i+1, search-i);
                    zhuyinAns += ret.substr(i);
                    chineseAns += tmp;
                    length += 1;
                    position.push_back(std::pair<int, int>(idx-search+i+1, idx));
                    break;
                }
            }
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
        if (cnt == 0) {
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
        ans += L"password: " + tmpChineseAns + L", zhuyin: " + zhuyinAns + L", origin: " + englishAns + L", total: " + input + L",EOF\n";
        ++size;
        
        // output to file if the string is too long
        if (size > 10000) {
            output << ans;
            ans = L"";
            size = 0;
        }
    } 
}

bool AtLeastTwo::filter(std::wstring input) {
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

void AtLeastTwo::print() {
    output << ans;
    output.close();
}

AtLeastTwo::AtLeastTwo(int idx) {
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

AtLeastTwo::~AtLeastTwo() {
    output.close();
}

