#include "util.h"
#include <fstream>
#include <algorithm>

Util::Util() :
        typeInZhuyin(L"ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ˙ˊˇˋ-"),
    typeInEnglishLower(L"1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634 "),
    typeInEnglishUpper(L"1QAZ2WSXEDCRFV5TGBYHNUJM8IK,9OL.0P;/-7634 ") {
    // for zhuyin to chinese
    std::wifstream chinese(chinesePath);
    std::wifstream zhuyin(zhuyinPath);
    if (!chinese.is_open() || !zhuyin.is_open()) {
        fprintf(stderr, "open chinese.txt or zhuyin.txt failed\n");
        exit(255);
    }
    chinese.imbue(utf8_locale);
    zhuyin.imbue(utf8_locale);
    std::wstring zhuyinLine, chineseLine;
    while (getline(chinese, chineseLine)) {
        getline(zhuyin, zhuyinLine);
        zhuyinToChineseMap[zhuyinLine] = chineseLine;
    }
    chinese.close();
    zhuyin.close();

    // for english to (from) zhuyin
    for (int i = 0; i < size; ++i) {
        zhuyinToEnglishMap[typeInZhuyin.substr(i, 1)] = typeInEnglishLower.substr(i, 1);
        englishToZhuyinMap[typeInEnglishUpper.substr(i, 1)] = typeInZhuyin.substr(i, 1);
        englishToZhuyinMap[typeInEnglishLower.substr(i, 1)] = typeInZhuyin.substr(i, 1);
    }

    // for misc functions
    endSet.insert(L"˙");
    endSet.insert(L"ˊ");
    endSet.insert(L"ˇ");
    endSet.insert(L"ˋ");
    endSet.insert(L"-");
}

std::wstring Util::zhuyinToChinese(std::wstring zhuyin) {
    auto it = zhuyinToChineseMap.find(zhuyin);
    if (it != zhuyinToChineseMap.end()) {
        return it->second;
    }
    else {
        return std::wstring(L"");
    }
}

std::wstring Util::zhuyinToEnglish(std::wstring zhuyin) {
    auto it = zhuyinToEnglishMap.find(zhuyin);
    if (it != zhuyinToEnglishMap.end()) {
        return it->second;
    }
    else {
        return std::wstring(L"");
    }
}

std::wstring Util::englishToZhuyin(std::wstring english) {
    auto it = englishToZhuyinMap.find(english);
    if (it != englishToZhuyinMap.end()) {
        return it->second;
    }
    else {
        return std::wstring(L"");
    }
}

bool Util::isEnd(std::wstring str) {
    auto it = endSet.find(str);
    if (it != endSet.end()) {
        return true;
    }
    return false;
}
