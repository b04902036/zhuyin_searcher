#ifndef __METHODS_H__
#define __METHODS_H__

#include <vector>
#include <fstream>

class NoType {
    public:
        NoType(int idx);
        ~NoType();
        void process(std::wstring input);
        void print();
    private:
        void reset();
        
        std::wofstream output;

        std::wstring ans;
        std::wstring englishAns, zhuyinAns, chineseAns;
        std::size_t size, length;
        std::vector<std::pair<int, int> > position;
};

#endif
