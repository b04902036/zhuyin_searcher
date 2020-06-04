#ifndef __NOTYPE_H__
#define __NOTYPE_H__

#include <vector>
#include <fstream>

class NoType {
    public:
        NoType();
        ~NoType();
        void process(std::wstring input);
        void print();
    private:
        void reset();

        const char* outputFileName = "result/NoType.txt";
        std::wofstream output;

        std::wstring ans;
        std::wstring englishAns, zhuyinAns, chineseAns;
        std::size_t size, length;
        std::vector<std::pair<int, int> > position;
};

#endif
