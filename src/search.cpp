#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <unordered_map>

#include "util.h"
#include "methods.h"

#define OMP_NUM_THREADS 2

class Util util;

int main(int argc, char* argv[]) {
    std::wstring line;
    std::size_t found;
    std::setlocale(LC_ALL, "");
    const std::locale utf8_locale
        = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    if (argc < 2) {
        std::cout << "usage : ./search <path/to/BreachCompilation/data\n\te.g. ./search ../BreachCompilation/data/y/z" << std::endl;
        exit(255);
    }

    if (argc < 1 + OMP_NUM_THREADS) {
        std::cerr << "Please provide " << OMP_NUM_THREADS << " files in argv!" << std::endl;
        exit(255);
    }
    std::wifstream files[OMP_NUM_THREADS];
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        files[i].open(argv[i+1]);
        if (!files[i].is_open()) {
            std::cerr << argv[i+1] << " does not exist!" << std::endl;
            exit(255);
        }
        files[i].imbue(utf8_locale);
    }

#ifdef NOTYPE
    class NoType* noTypes[OMP_NUM_THREADS];
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        noTypes[i] = new NoType(i);
    }
#endif

#ifdef ONLYCHINESE
    class OnlyChinese* onlyChineses[OMP_NUM_THREADS];
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        onlyChineses[i] = new OnlyChinese(i);
    }
#endif

#ifdef ATLEASTTWO
    class AtLeastTwo* atLeastTwos[OMP_NUM_THREADS];
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        atLeastTwos[i] = new AtLeastTwo(i);
    }
#endif

#ifdef STATEMACHINE
    class StateMachine* stateMachines[OMP_NUM_THREADS];
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        stateMachines[i] = new StateMachine(i);
    }
#endif

    long long int cnt = 0;

    #pragma omp parallel sections firstprivate(cnt, line)
    {
        #pragma omp section
        while (getline(files[0], line)) {
            if (++cnt % 1000000 == 0) {
                std::cout << "[0] " << cnt << std::endl;
            }

            found = line.find_first_of(L"@");
            if (found == std::wstring::npos) {
                continue;
            }
            found = line.find_first_of(L":;", found+1);
            if (found == std::string::npos) {
                continue;
            }
            line = line.substr(found+1);

#ifdef NOTYPE
            noTypes[0]->process(line);
#endif

#ifdef ONLYCHINESE
            onlyChineses[0]->process(line);
#endif

#ifdef ATLEASTTWO
            atLeastTwos[0]->process(line);
#endif

#ifdef STATEMACHINE
            stateMachines[0]->process(line);
#endif

        }
        
        #pragma omp section
        while (getline(files[1], line)) {
            if (++cnt % 1000000 == 0) {
                std::cout << "[1] " << cnt << std::endl;
            }

            found = line.find_first_of(L"@");
            if (found == std::wstring::npos) {
                continue;
            }
            found = line.find_first_of(L":;", found+1);
            if (found == std::string::npos) {
                continue;
            }
            line = line.substr(found+1);

#ifdef NOTYPE
            noTypes[1]->process(line);
#endif

#ifdef ONLYCHINESE
            onlyChineses[1]->process(line);
#endif

#ifdef ATLEASTTWO
            atLeastTwos[1]->process(line);
#endif

#ifdef STATEMACHINE
            stateMachines[1]->process(line);
#endif

        }
    }

#ifdef NOTYPE
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        noTypes[i]->print();
    }
#endif

#ifdef ONLYCHINESE
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        onlyChineses[i]->print();
    }
#endif

#ifdef ATLEASTTWO
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        atLeastTwos[i]->print();
    }
#endif

#ifdef STATEMACHINE
    for (int i = 0; i < OMP_NUM_THREADS; ++i) {
        stateMachines[i]->print();
    }
#endif

}
