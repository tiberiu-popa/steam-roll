#!/bin/python3

from collections import Counter

def arrayManipulation(n, queries):
    if n <= 0:
        raise Exception('n should be positive')
    diff = (n + 1) * [0]
    for a, b, k in queries:
        diff[a - 1] += k
        diff[b] -= k
    elem = diff[0]
    for i in range(n):
        diff[i + 1] += diff[i]
    if diff[-1] != 0:
        raise Exception('wtf')
    return max(diff[:-1])

def reverseShuffleMerge(s):
    target = ''.join(sorted(list(s))[::2])
    freq = Counter(s)
    target_freq = Counter(target)
    n = len(s)
    arr = []
    for i in range(n - 1, -1, -1):
        ch = s[i]
        if freq[ch] > 0 and target_freq[ch] > 0:
            if not arr:
                target_freq[ch] -= 1
                arr.append(ch)
            else:
                while arr:
                    last_ch = arr[-1]
                    if ch >= last_ch or freq[last_ch] < target_freq[last_ch] + 1:
                        break
                    target_freq[last_ch] += 1
                    arr.pop()
                target_freq[ch] -= 1
                arr.append(ch)
        freq[ch] -= 1
    return ''.join(arr)

def main():
    s = 'abcdefgabcdefg'
    res = reverseShuffleMerge(s)
    print('reverseShuffleMerge', res)

if __name__ == '__main__':
    main()
