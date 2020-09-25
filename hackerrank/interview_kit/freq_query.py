#!/bin/python3

from collections import Counter, defaultdict

def freqQuery(queries):
    arr = []
    d = defaultdict(int)
    freq = defaultdict(int)
    for query in queries:
        op = query[0]
        x = query[1]
        if op == 1:
            d[x] += 1
            freq[d[x]] += 1
            if d[x] > 1:
                freq[d[x] - 1] -= 1
        elif op == 2:
            if x in d and d[x] > 0:
                d[x] -= 1
                freq[d[x] + 1] -= 1
                if d[x] > 0:
                    freq[d[x]] += 1
        else:
            if freq[x] > 0:
                arr.append(1)
            else:
                arr.append(0)
    return arr

def main():
    queries = [[1, 5], [1, 6], [3, 2], [1, 10], [1, 10], [1, 6], [2, 5], [3, 2]]
    res = freqQuery(queries)
    print('freqQuery', res)

if __name__ == '__main__':
    main()
