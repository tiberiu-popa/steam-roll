import argparse
from lib import read_ints, run_script
import sys

# Complete the countInversions function below.
def countInversions(arr):
    return mergeSort(arr[:], 0, len(arr))

def mergeSort(v, left, right):
    if right - left <= 1:
        return 0
    elif right - left <= 8:
        return mergeSortSmall(v, left, right)
    mid = (left + right) // 2
    result = mergeSort(v, mid, right)
    result += mergeSort(v, left, mid)
    u = v[mid:right]
    i = mid - 1
    j = len(u) - 1
    k = right - 1
    while i >= left and j >= 0:
        if v[i] <= u[j]:
            v[k] = v[i]
            result += len(u) - 1 - j
            i -= 1
        else:
            v[k] = u[j]
            j -= 1
        k -= 1
    if i >= left:
        result += (i - left + 1) * (len(u) - 1 - j)
    v[left:left+j+1] = u[0:j+1]
    return result

def mergeSortSmall(v, left, right):
    result = 0
    for i in range(left, right):
        for j in range(i + 1, right):
            if v[i] > v[j]:
                result += 1
    v[left:right] = sorted(v[left:right], reverse=True)
    return result

def process_file(input_filename, output_file=sys.stdout):
    reader = read_ints(input_filename)
    numTests = next(reader)
    for _ in range(numTests):
        n = next(reader)
        arr = [next(reader) for _ in range(n)]
        res = countInversions(arr)
        print(res, file=output_file)

def main(script):
    run_script(script, process_file)

if __name__ == '__main__':
    main(sys.argv[0])
