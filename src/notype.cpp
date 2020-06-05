#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "noType.h"
#include "util.h"
#include <algorithm>

extern Util util;

void NoType::reset() {
    englishAns.clear();
    zhuyinAns.clear();
    chineseAns.clear();
    length = 0;
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
        /*
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
        */
        
        std::wstring tmpChineseAns;
        int now = 0;
        for (int i = 0; i < chineseAns.length(); ++i) {
            tmpChineseAns += input.substr(now, position[i].first-now);
            now = position[i].second + 1;
            tmpChineseAns += chineseAns[i];
        }
        tmpChineseAns += input.substr(now);
        ans += L"password: " + tmpChineseAns + L", zhuyin: " + zhuyinAns + L", origin: " + englishAns + L"EOF\n";
        ++size;
        
        // output to file if the string is too long
        if (size > 10000) {
            output << ans;
            ans = L"";
            size = 0;
        }
    } 
}

void NoType::print() {
    output << ans;
    output.close();
}

NoType::NoType() {
    const std::locale utf8_locale
        = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    output.open(outputPath);
    output.imbue(utf8_locale);
    if (!output.is_open()) {
        fprintf(stderr, "open result/NoType.txt failed\n");
        exit(255);
    }
}

NoType::~NoType() {
    output.close();
}

