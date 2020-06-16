#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <vector>
#include <fstream>

class StateMachine {
    public:
        StateMachine(int idx);
        ~StateMachine();
        void process(std::wstring input);
        bool filter(std::wstring input);
        void print();
    private:
        void reset();
        int searchChinese();

        enum State {
            EMPTY,
            HEAD,
            MIDDLE,
            BOTTOM
        };

        const char* outputPath = "result/StateMachine.txt";
        std::wofstream output;

        std::wstring ans;
        std::wstring englishAns, zhuyinAns, chineseAns;
        std::size_t size, length;
        std::vector<std::pair<int, int> > position;

        std::wstring current;
        std::wstring currentAns;
        State currentState;
};

#endif
