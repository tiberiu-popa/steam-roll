from lib import read_ints
import sys

# Complete the poisonousPlants function below.
def poisonousPlants(p):
    st = []
    result = 0
    for x in p:
        max_run = 0
        while st and st[-1][1] >= x:
            run, _ = st.pop()
            if run > max_run:
                max_run = run
        if st:
            max_run += 1
        else:
            max_run = 0
        if max_run > result:
            result = max_run
        st.append((max_run, x))
    while st:
        run, _ = st.pop()
        if run > result:
            result = run
    return result

def poisonousPlantsSlow(p):
    steps = 0
    while True:
        q = []
        for i, x in enumerate(p):
            if i == 0 or p[i - 1] >= x:
                q.append(x)
        if len(p) == len(q):
            break
        p = q
        steps += 1
    return steps

def main(filenames):
    for filename in filenames:
        print('Processing', filename)
        reader = read_ints(filename)
        numElems = next(reader)
        arr = []
        for _ in range(numElems):
            arr.append(next(reader))
        res = poisonousPlants(arr)
        res_slow = poisonousPlantsSlow(arr)
        print('poisonousPlants: res={} res_slow={}'.format(res, res_slow))

if __name__ == '__main__':
    main(sys.argv[1:])
