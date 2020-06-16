#include "util.h"
#include <fstream>
#include <algorithm>
#include <iostream>

Util::Util() :
        typeInZhuyin(L"ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ˙ˊˇˋ-"),
    typeInEnglishLower(L"1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634 "),
    typeInEnglishUpper(L"1QAZ2WSXEDCRFV5TGBYHNUJM8IK,9OL.0P;/-7634 ") {
    std::setlocale(LC_ALL, "");
    
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

    // for preventing conversion from english to infeasible zhuyin
    std::wifstream exception(exceptionSentencePath);
    if (!exception.is_open()) {
        fprintf(stderr, "open exceptionSentence.txt failed\n");
        exit(255);
    }
    exception.imbue(utf8_locale);
    
    std::wstring exceptionLine;
    while (getline(exception, exceptionLine)) {
        exceptionSentenceSet.insert(exceptionLine);
    }
    exception.close();

    exception.open(exceptionWordPath);
    if (!exception.is_open()) {
        fprintf(stderr, "open exceptionWord.txt failed\n");
        exit(255);
    }
    exception.imbue(utf8_locale);
    
    while (getline(exception, exceptionLine)) {
        exceptionWordSet.insert(exceptionLine);
    }

    // for misc functions
    std::wstring headString = L"ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙ";
    std::wstring middleString = L"ㄧㄨㄩ";
    std::wstring bottomString = L"ㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ";
    std::wstring endString = L"˙ˊˇˋ-";
    for (int i = 0; i < headString.length(); ++i) {
        headSet.insert(headString.substr(i, 1));
    }
    for (int i = 0; i < middleString.length(); ++i) {
        middleSet.insert(middleString.substr(i, 1));
    }
    for (int i = 0; i < bottomString.length(); ++i) {
        bottomSet.insert(bottomString.substr(i, 1));
    }
    for (int i = 0; i < endString.length(); ++i) {
        endSet.insert(endString.substr(i, 1));
    }
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

bool Util::isExceptionSentence(std::wstring str) {
    auto it = exceptionSentenceSet.find(str);
    if (it != exceptionSentenceSet.end()) {
        return true;
    }
    return false;
}

bool Util::isExceptionWord(std::wstring str) {
    auto it = exceptionWordSet.find(str);
    if (it != exceptionWordSet.end()) {
        return true;
    }
    return false;
}

bool Util::isHead(std::wstring str) {
    auto it = headSet.find(str);
    if (it != headSet.end()) {
        return true;
    }
    return false;
}

bool Util::isMiddle(std::wstring str) {
    auto it = middleSet.find(str);
    if (it != middleSet.end()) {
        return true;
    }
    return false;
}

bool Util::isBottom(std::wstring str) {
    auto it = bottomSet.find(str);
    if (it != bottomSet.end()) {
        return true;
    }
    return false;
}

bool Util::isEnd(std::wstring str) {
    auto it = endSet.find(str);
    if (it != endSet.end()) {
        return true;
    }
    return false;
}
