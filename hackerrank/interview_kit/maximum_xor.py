from collections import defaultdict
from lib import dbg, read_ints, run_script
import sys

class TreeNode:
    def __init__(self, x):
        self.x = x
        self.children = [None, None]

class Trie:
    def __init__(self):
        self.root = TreeNode(None)
        self.height = 0

    def insert(self, x):
        digits = self.to_digits(x)
        num_digits = len(digits)
        if num_digits > self.height:
            children = [child for child in self.root.children if child is not None]
            if children:
                for _ in range(num_digits - self.height):
                    new_root = TreeNode(0)
                    new_root.children = self.root.children
                    self.root.children = [new_root, None]
            self.height = num_digits
        self.insertRec(self.root, digits, 0)

    def insertRec(self, node, digits, idx):
        if idx == len(digits):
            return
        if digits[idx] is None:
            raise Exception('wtf')
        next_node = node.children[digits[idx]]
        if next_node is None:
            next_node = TreeNode(digits[idx])
            node.children[digits[idx]] = next_node
        self.insertRec(next_node, digits, idx + 1)

    def query(self, x):
        digits = self.to_digits(x)
        if len(digits) > self.height:
            digits = digits[-self.height:]
        node = self.root
        result = 0
        for digit in digits:
            node = node.children[digit ^ 1] or node.children[digit]
            result = 2 * result + node.x
        return result ^ x

    def to_digits(self, x):
        if x == 0:
            digits = [0]
        else:
            digits = []
            while x > 0:
                digits.append(x & 1)
                x >>= 1
        for _ in range(self.height - len(digits)):
            digits.append(0)
        digits.reverse()
        return digits

    def checkHeight(self):
        path = []
        self.checkHeightRec(self.root, self.height, path)

    def checkHeightRec(self, node, height, path):
        if height == 0:
            return
        children = [child for child in node.children if child is not None]
        if not children:
            raise Exception('Expected to have at least one child')
        for child in children:
            path.append(child.x)
            self.checkHeightRec(child, height - 1, path)
            path.pop()

# Complete the maxXor function below.
def maxXor(arr, queries):
    trie = Trie()
    for elem in arr:
        trie.insert(elem)

    return [trie.query(elem) for elem in queries]

def process_file(input_filename, output_file=sys.stdout):
    reader = read_ints(input_filename)
    arr = []
    n = next(reader)
    for _ in range(n):
        arr.append(next(reader))
    queries = []
    m = next(reader)
    for _ in range(m):
        queries.append(next(reader))
    for elem in maxXor(arr, queries):
        print(elem, file=output_file)

def main(script):
    run_script(script, process_file)

if __name__ == '__main__':
    main(sys.argv[0])
