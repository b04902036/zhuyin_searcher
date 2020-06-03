#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>

#include <unordered_map>
#include "methods.h"
#include "util.h"

class NoType noType;
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

    std::wifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << argv[1] << " does not exist!" << std::endl;
        exit(255);
    }
    file.imbue(utf8_locale);

    while (getline(file, line)) {
        found = line.find_first_of(L"@");
        if (found == std::wstring::npos) {
            continue;
        }
        found = line.find_first_of(L":;", found+1);
        if (found == std::string::npos) {
            continue;
        }
        noType.process(line.substr(found+1));        
    }
    noType.print();
}
