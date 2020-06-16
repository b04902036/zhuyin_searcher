#ifndef __ATLEASTTWO_H__
#define __ATLEASTTWO_H__

#include <vector>
#include <fstream>

class AtLeastTwo {
    public:
        AtLeastTwo(int idx);
        ~AtLeastTwo();
        void process(std::wstring input);
        bool filter(std::wstring input);
        void print();
    private:
        void reset();

        const char* outputPath = "result/AtLeastTwo.txt";
        std::wofstream output;

        std::wstring ans;
        std::wstring englishAns, zhuyinAns, chineseAns;
        std::size_t size, length;
        std::vector<std::pair<int, int> > position;
};

#endif
