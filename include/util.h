#ifndef __UTIL_H__
#define __UTIL_H__

#include <unordered_set>
#include <unordered_map>
#include <codecvt>
#include <locale>
#include <stdbool.h>

class Util {
    public:
        Util();
        ~Util(){};
        
        std::wstring zhuyinToChinese(std::wstring zhuyin);

        std::wstring zhuyinToEnglish(std::wstring zhuyin);
        std::wstring englishToZhuyin(std::wstring english);

        bool isEnd(std::wstring str);
    private:
        // for zhuyin to chinese
        const std::locale utf8_locale
                = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
        const char* chinesePath = "./tools/bopomofo/chinese.txt";
        const char* zhuyinPath = "./tools/bopomofo/zhuyin.txt";

        std::unordered_map<std::wstring, std::wstring> zhuyinToChineseMap;

        // for english to (from) zhuyin
        std::wstring typeInZhuyin;
        std::wstring typeInEnglishUpper, typeInEnglishLower;
        
        std::unordered_map<std::wstring, std::wstring> zhuyinToEnglishMap;
        std::unordered_map<std::wstring, std::wstring> englishToZhuyinMap;
        std::size_t size = 42;

        // for misc functions
        std::unordered_set<std::wstring> endSet;
};


#endif
