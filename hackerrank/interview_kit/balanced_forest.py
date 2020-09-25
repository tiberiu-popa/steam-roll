from collections import defaultdict
from lib import dbg, read_ints
import sys

# Complete the balancedForest function below.
def balancedForest(c, edges):
    n = len(c)
    out = []
    for _ in range(n):
        out.append([])
    for x, y in edges:
        out[x - 1].append(y - 1)
        out[y - 1].append(x - 1)

    visited = set()
    arr = n * [0]
    euler = []

    def dfs(root):
        s = c[root]
        visited.add(root)
        euler.append((root, 0))
        for node in out[root]:
            if node not in visited:
                s += dfs(node)
        arr[root] = s
        euler.append((root, 1))
        return s

    dfs(0)

    result = None
    total = arr[0]

    def update(cand):
        nonlocal result
        if result is None or cand < result:
            result = cand

    bw_set = set()
    for i in range(len(euler) - 1, -1, - 1):
        node, state = euler[i]
        c_sum = arr[node]
        if state == 0:
            bw_set.add(c_sum)
        else:
            rem = total - 2 * c_sum
            if rem <= c_sum:
                if c_sum in bw_set or rem in bw_set:
                    update(c_sum - rem)
            diff = total - c_sum
            if diff % 2 == 0:
                half_diff = diff // 2
                if c_sum <= half_diff and half_diff in bw_set:
                    update(half_diff - c_sum)

    fw_hm = defaultdict(int)
    for i in range(len(euler)):
        node, state = euler[i]
        c_sum = arr[node]
        if state == 0:
            dbl_sum = 2 * c_sum
            rem = total - dbl_sum
            if rem <= c_sum:
                if dbl_sum in fw_hm or (rem + c_sum) in fw_hm:
                    update(c_sum - rem)
            diff = total - c_sum
            if diff % 2 == 0:
                half_diff = diff // 2
                if c_sum <= half_diff and (half_diff + c_sum) in fw_hm:
                    update(half_diff - c_sum)
            fw_hm[c_sum] += 1
        else:
            fw_hm[c_sum] -= 1
            if fw_hm[c_sum] == 0:
                del fw_hm[c_sum]

    if result is None and total % 2 == 0:
        half_total = total // 2
        if half_total in bw_set:
            return half_total

    return result if result is not None else -1

def main(filenames):
    for filename in filenames:
        print('Processing', filename)
        reader = read_ints(filename)
        q = next(reader)
        for _ in range(q):
            n = next(reader)
            c = []
            for _ in range(n):
                c.append(next(reader))
            edges = []
            for _ in range(n - 1):
                x = next(reader)
                y = next(reader)
                edges.append((x, y))
            res = balancedForest(c, edges)
            print('\tbalancedForest: {}'.format(res))
        print()

if __name__ == '__main__':
    main(sys.argv[1:])
