#!/bin/python3

from bisect import bisect_left
from lib import read_ints

decbin_dp = []
total = []

# Complete the decibinaryNumbers function below.
def decibinaryNumbers(x):
    if x <= 0:
        return
    elif x == 1:
        return 0
    elif x == 2:
        return 1
    max_num = 1 << 54
    num_bits = 20
    if len(decbin_dp) <= 0:
        row = num_bits * [1]
        decbin_dp.append(row)
        total.append(row[-1])
    if len(decbin_dp) <= 1:
        row = num_bits * [1]
        row[0] = 0
        decbin_dp.append(row)
        total.append(total[-1] + row[-1])
    for i in range(2, max_num):
        if total[-1] >= x:
            break
        if len(decbin_dp) <= i:
            decbin_dp.append(num_bits * [0])
            for j in range(1, num_bits):
                m = 1 << (j - 1)
                if m <= i:
                    for k in range(1, 10):
                        if m * k <= i:
                            decbin_dp[i][j] += decbin_dp[i - m * k][j - 1]
                decbin_dp[i][j] += decbin_dp[i][j - 1]
            total.append(total[-1] + decbin_dp[i][-1])
    i = bisect_left(total, x)
    result = num_bits * [0]
    x -= total[i - 1]
    while i > 0:
        j = 0
        while j < num_bits and decbin_dp[i][j] < x:
            j += 1
        if j > 0:
            x -= decbin_dp[i][j - 1]
        m = 1 << (j - 1)
        k = 1
        while k < 10 and m * k <= i and decbin_dp[i - m * k][j - 1] < x:
            x -= decbin_dp[i - m * k][j - 1]
            k += 1
        result[j] = k
        i -= m * k
    s = ''.join([str(x) for x in reversed(result[1:])]).lstrip('0')
    return s

def main():
    queries = [1, 2, 3, 4, 10, 23, 9759941667961018]
    for query in queries:
        res = decibinaryNumbers(query)
        print('decibinaryNumbers: query={} res={}'.format(query, res))

if __name__ == '__main__':
    main()
