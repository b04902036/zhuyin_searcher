#ifndef __METHODS_H__
#define __METHODS_H__

#include <vector>

class NoType {
    public:
        NoType(){};
        ~NoType(){};
        void process(std::wstring input);
        void print();
    private:
        void reset();
        std::wstring ans;
        std::wstring englishAns, zhuyinAns, chineseAns;
        std::size_t size;
        std::vector<std::pair<int, int> > position;
};

#endif
