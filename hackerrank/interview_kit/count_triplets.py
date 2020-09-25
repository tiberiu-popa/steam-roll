#!/bin/python3

from collections import defaultdict

def countTriplets(arr, r):
    n = len(arr)
    d0 = defaultdict(list)
    for i, x in enumerate(arr):
        d0[x].append(i)

    result = 0
    if r == 1:
        for l in d0.values():
            c = len(l)
            result += c * (c - 1) * (c - 2) // 6
        return result

    d1 = defaultdict(list)
    for i, x in enumerate(arr):
        if x % r == 0:
            d1[x // r].append(i)

    d2 = defaultdict(list)
    rsq = r * r
    for i, x in enumerate(arr):
        if x % rsq == 0:
            d2[x // rsq].append(i)

    for elem, l2 in d2.items():
        if elem in d0 and elem in d1:
            l0 = d0[elem]
            l1 = d1[elem]
            j = k = 0
            mid = []
            while j < len(l1):
                while k < len(l2) and l1[j] > l2[k]:
                    k += 1
                mid.append(len(l2) - k)
                j += 1
            for i in range(len(mid) - 2, -1, -1):
                mid[i] += mid[i + 1]
            i = j = 0
            while i < len(l0):
                while j < len(l1) and l0[i] > l1[j]:
                    j += 1
                if j < len(l1):
                    result += mid[j]
                else:
                    break
                i += 1

    return result

def main():
    arr = [1, 3, 9, 9, 27, 81]
    r = 3
    res = countTriplets(arr, r)
    print('countTriplets:', res)

if __name__ == '__main__':
    main()
