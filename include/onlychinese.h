#ifndef __ONLYCHINESE_H__
#define __ONLYCHINESE_H__

#include <vector>
#include <fstream>

class OnlyChinese {
    public:
        OnlyChinese();
        ~OnlyChinese();
        void process(std::wstring input);
        void print();
    private:
        void reset();
        
        const char* outputPath = "result/OnlyChinese.txt";
        std::wofstream output;

        std::wstring ans;
        std::wstring englishAns, zhuyinAns, chineseAns;
        std::size_t size, length;
        std::vector<std::pair<int, int> > position;
};

#endif
