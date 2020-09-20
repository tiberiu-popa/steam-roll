from collections import Counter
import json
import random
import sys
from timeit import default_timer as timer

class LinkedListIterator:
    def __init__(self, root, level):
        self.root = root
        self.level = level
        self.node = root.next_nodes[level]

    def __iter__(self):
        return self

    def __next__(self):
        if self.node is self.root:
            raise StopIteration
        else:
            node = self.node
            self.node = self.node.next_nodes[self.level]
            return node

class LinkedListNode:
    def __init__(self, val, next_nodes=None, prev_nodes=None):
        self.val = val
        self.next_nodes = next_nodes or []
        self.prev_nodes = prev_nodes or []

    def __iter__(self):
        return LinkedListIterator(self, 0)

    def unlink(self):
        max_level = len(self.next_nodes)
        for i in range(max_level):
            prev_node = self.prev_nodes[i]
            next_node = self.next_nodes[i]
            prev_node.next_nodes[i] = next_node
            next_node.prev_nodes[i] = prev_node

class Skiplist:

    def __init__(self):
        self.root = LinkedListNode(val=None)

    def search(self, target: int) -> bool:
        return self.searchNode(target) is not None

    def searchNode(self, target: int) -> LinkedListNode:
        node = self.root
        max_level = len(self.root.next_nodes)
        i = max_level - 1
        while i >= 0:
            while True:
                prev_node = node
                node = node.next_nodes[i]
                if node is self.root or node.val >= target:
                    break
            if node is not self.root and node.val == target:
                return node
            node = prev_node
            i -= 1
        return None

    def add(self, num: int) -> None:
        level = 1
        while random.randint(0, 1) == 0:
            level += 1
        while len(self.root.next_nodes) < level:
            self.root.next_nodes.append(self.root)
            self.root.prev_nodes.append(self.root)
        max_level = len(self.root.next_nodes)
        new_node = LinkedListNode(num, level * [None], level * [None])
        node = self.root
        for i in range(max_level - 1, -1, -1):
            while node.next_nodes[i] is not self.root and node.next_nodes[i].val <= num:
                node = node.next_nodes[i]
            if i < level:
                new_node.next_nodes[i] = node.next_nodes[i]
                new_node.prev_nodes[i] = node
                node.next_nodes[i].prev_nodes[i] = new_node
                node.next_nodes[i] = new_node

    def erase(self, num: int) -> bool:
        node = self.searchNode(num)
        if node is not None:
            node.unlink()
            return True
        else:
            return False

    def printAllLevels(self):
        level = len(self.root.next_nodes)
        for i in range(level):
            self.print(i)
        print()

    def print(self, level) -> None:
        node = self.root
        print('List @ level={}:'.format(level), end='')
        while True:
            print(' {}'.format(node.val), end='')
            node = node.next_nodes[level]
            if node is self.root:
                break
        print()

def main(filenames):
    for filename in filenames:
        with open(filename) as f:
            v = []
            for line in f:
                v.append(json.loads(line))
        results = []
        c = Counter()
        s = Counter()
        for name, args in zip(v[0], v[1]):
            c[name] += 1
            start = timer()
            if name[0].isupper():
                cls_type = globals()[name]
                obj = cls_type(*args)
            else:
                results.append(getattr(obj, name)(*args))
            end = timer()
            s[name] += end - start
        print(results)
        ls = [len(node.next_nodes) for node in obj.root]

if __name__ == '__main__':
    main(sys.argv[1:])
