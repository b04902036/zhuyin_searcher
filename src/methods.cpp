#include <iostream>
#include <string>
#include "methods.h"
#include "util.h"
#include <algorithm>

extern Util util;

void NoType::reset() {
    englishAns.clear();
    zhuyinAns.clear();
    chineseAns.clear();
    size = 0;
    position.clear();
}

void NoType::process(std::wstring input) {
    std::size_t totalLength = input.length();
    std::wstring ret, tmp;
    int search;

    reset();
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
                tmp = util.zhuyinToChinese(ret.substr(i));    
                if (tmp.length() > 0) {
                    englishAns += input.substr(idx-search+i+1, search-i);
                    zhuyinAns += ret.substr(i);
                    chineseAns += tmp;
                    size += 1;
                    position.push_back(std::pair<int, int>(idx-search+i+1, idx));
                    break;
                }
            }
        }
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
        ans += L"password: " + tmpChineseAns + L", zhuyin: " + zhuyinAns + L", origin: " + englishAns + L"\n";
    } 
}

void NoType::print() {
    std::wcout << ans;
}


