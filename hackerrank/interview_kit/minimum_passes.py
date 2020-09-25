#!/bin/python3

# Complete the minimumPasses function below.
def minimumPasses(m, w, p, n):
    prev_passes = None
    passes = 0
    candles = 0
    while True:
        step = m * w
        total_passes = passes + (n - candles + step - 1) // step
        candles += step
        if prev_passes is not None and total_passes > prev_passes:
            return prev_passes
        inc = candles // p
        if inc >= 1:
            diff = min(abs(m - w), inc)
            if m < w:
                m += diff
            if m > w:
                w += diff
            rem = inc - diff
            if rem % 2 == 0:
                m += rem // 2
                w += rem // 2
            else:
                m += (rem + 1) // 2
                w += rem // 2
            candles -= p * inc
            passes += 1
        else:
            skip = (p - candles + step - 1) // step
            passes += skip + 1
            candles += skip * step
            if m <= w:
                m += 1
            else:
                w += 1
            candles -= p
        prev_passes = total_passes

def main():
    tests = [
        [1, 1, 6, 45],
        [1, 100, 10000000000, 1000000000000],
        [3, 13, 13, 1000000000000],
    ]
    for [m, w, p, n] in tests:
        res = minimumPasses(m, w, p, n)
        print('minimumPasses', res)

if __name__ == '__main__':
    main()
