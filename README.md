## CNS final

### Run
- ./search

### Add new detection method
1. add src/<new method>.h, define the class here
2. add include/<new method>.cpp, define your function here
3. add `#include "<new method>.h"` into include/methods.h
4. use your class and function in `src/search.cpp`
5. add <new method>.o into `obj` list in Makefile
6. make clean && make 
7. ./search

### Strategies
- Global Strategy
    - Try to find possible zhuyin typing in english
    - If the matched string contains only digits, we will discard it. For example, `ppp040404`, since the matched string is `040404` (`暗暗暗`), it contains only digits.
- NoType
    - [a-zA-Z0-9]*chinese[a-zA-Z0-9]*
- OnlyChinese
    - chinese

### Notice
- `tools/bopomofo/` contains a modified project from others github that crawl chinese <-> zhuyin mapping from Ministry of Education's dictionary, we need the merged.dict to check if a zhuyin combination can form a chinese
