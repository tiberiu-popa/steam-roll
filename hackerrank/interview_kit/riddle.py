#!/bin/python3

from lib import read_ints

# Complete the riddle function below.
def riddle(arr):
    # complete this function
    result = len(arr) * [None]
    fw = longest_min_runs(arr)
    bw = longest_min_runs(arr[::-1])
    for i, v in enumerate(bw):
        fw[len(arr) - 1 - i] += v - 1
    for i, v in enumerate(fw):
        if result[v - 1] is None or arr[i] > result[v - 1]:
            result[v - 1] = arr[i]
    for i in range(len(result) - 2, -1, -1):
        if result[i] is None or result[i] < result[i + 1]:
            result[i] = result[i + 1]
    return result

def longest_min_runs(arr):
    st = []
    runs = len(arr) * [None]
    for i, x in enumerate(arr):
        while st and st[-1][1] > x:
            pos, y = st.pop()
            runs[pos] = i - pos
        if not st or x >= st[-1][1]:
            st.append((i, x))
    while st:
        pos, y = st.pop()
        runs[pos] = len(arr) - pos
    return runs

def main():
    filename = 'files/riddle_01.in'
    contents = read_ints(filename)
    numElems = next(contents)
    arr = []
    for _ in range(numElems):
        arr.append(next(contents))
    res = riddle(arr)
    filename = 'files/riddle_01.out'
    out = []
    contents = read_ints(filename)
    for _ in range(numElems):
        out.append(next(contents))
    cmp = []
    for i, (x, y) in enumerate(zip(out, res)):
        if x != y:
            cmp.append((i, (x, y)))
    print('riddle: res={} cmp={}'.format(res, cmp))

if __name__ == '__main__':
    main()
