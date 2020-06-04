#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>
#include <omp.h>

#include <unordered_map>
#include "util.h"
#include "methods.h"

#define MAX_OMP_THREAD 1

class Util util;

int main(int argc, char* argv[]) {
    omp_set_num_threads(MAX_OMP_THREAD);
    std::wstring line;
    std::size_t found;
    std::setlocale(LC_ALL, "");
    const std::locale utf8_locale
        = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    if (argc < 2) {
        std::cout << "usage : ./search <path/to/BreachCompilation/data\n\te.g. ./search ../BreachCompilation/data/y/z" << std::endl;
        exit(255);
    }

    std::wifstream file[MAX_OMP_THREAD];
    for (int i = 0; i < MAX_OMP_THREAD; ++i) {
        file[i].open(argv[i+1]);
        if (!file[i].is_open()) {
            std::cout << argv[i+1] << " does not exist!" << std::endl;
            exit(255);
        }
        file[i].imbue(utf8_locale);
    }
   
    // class NoType* noType = new NoType();
    class OnlyChinese* onlyChinese = new OnlyChinese();
    
    while (getline(file[0], line)) {
        found = line.find_first_of(L"@");
        if (found == std::wstring::npos) {
            continue;
        }
        found = line.find_first_of(L":;", found+1);
        if (found == std::string::npos) {
            continue;
        }
        line = line.substr(found+1);

        // noType->process(line);
        onlyChinese->process(line);
    }
    // noType->print();
    onlyChinese->print();
}
