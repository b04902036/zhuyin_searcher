## CNS final

### Usage
- python3 search.py

### Strategy
- Try to find possible zhuyin typing in english
- If the matched string contains only digits, we will discard it.

### Notice
- `bopomofo/` contains a modified project from others github that crawl chinese <-> zhuyin mapping from Ministry of Education's dictionary, we need the merged.dict to check if a zhuyin combination can form a chinese
