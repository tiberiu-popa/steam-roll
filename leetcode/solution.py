from collections import Counter

# Definition for singly-linked list.
class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

class Solution:
    def print_list(self, head: ListNode) -> None:
        s = set()
        s.add(head)
        while head is not None:
            print(head.val, end=' ')
            head = head.next
            if head in s:
                print('cycle hit at', head.val)
                break
            else:
                s.add(head)
        print()

    def sortList(self, head: ListNode) -> ListNode:
        root = ListNode(0, head)
        k = 1
        while True:
            node = root.next
            n = 0
            num_processed = 0
            last = root
            left = None
            right = None
            while node is not None:
                idx = n % (2 * k)
                next_node = node.next
                if idx == 0:
                    left = node
                if idx == k:
                    right = node
                    num_processed += 1
                    kl = 0
                    kr = 0
                    while kl < k and kr < k and right is not None:
                        if left.val <= right.val:
                            last.next = left
                            last = left
                            left = left.next
                            kl += 1
                        else:
                            next_right = right.next
                            last.next = right
                            right.next = left
                            last = right
                            right = next_right
                            kr += 1
                    n += kr
                    if kl < k:
                        last.next = left
                        for _ in range(k - kl):
                            last = last.next
                        last.next = right
                    else:
                        last.next = right
                        for _ in range(k - kr):
                            if last is not None:
                                last = last.next
                                n += 1
                            else:
                                break
                    node = last.next
                else:
                    node = next_node
                    n += 1
            k *= 2
            if k >= n:
                break
        return root.next

    def calculate(self, s: str) -> int:
        st = []
        out = []
        i = 0

        def is_op(t):
            return len(t) == 1 and t in '+-*/'

        def prec(t):
            if t == '+':
                return 0
            elif t == '-':
                return 0
            elif t == '*':
                return 1
            elif t == '/':
                return 1
            else:
                raise Exception('unknown operator')

        def is_left_assoc(t):
            return is_op(t)

        def should_process(prev_op, crt_op):
            prev_prec = prec(prev_op)
            crt_prec = prec(crt_op)
            return prev_prec > crt_prec or \
                (prev_prec == crt_prec and is_left_assoc(prev_op))

        def process_stack():
            op = st.pop()
            right = out.pop()
            if op == '+':
                out[-1] += right
            elif op == '-':
                out[-1] -= right
            elif op == '*':
                out[-1] *= right
            elif op == '/':
                out[-1] //= right

        while i < len(s):
            if s[i].isspace():
                while i < len(s) and s[i].isspace():
                    i += 1
                if i == len(s):
                    break

            if s[i].isdigit():
                start = i
                while i < len(s) and s[i].isdigit():
                    i += 1
                out.append(int(s[start:i]))
            else:
                if is_op(s[i]):
                    while st and is_op(st[-1]) and should_process(st[-1], s[i]):
                        process_stack()
                    st.append(s[i])
                elif s[i] == '(':
                    st.append('(')
                elif s[i] == ')':
                    while is_op(st[-1]):
                        process_stack()
                    if st[-1] == '(':
                        st.pop()
                    else:
                        raise Exception('mismatched parantheses')
                else:
                    raise Exception('unknown token')
                i += 1

        while st:
            process_stack()

        return out[0]

    def reversePairs(self, nums: [int]) -> int:
        result = self.mergeSort(nums, 0, len(nums))
        return result

    def mergeSort(self, v, left, right):
        if right - left <= 1:
            return 0
        elif right - left <= 16:
            return self.mergeSortSmall(v, left, right)
        mid = (left + right) // 2
        result = self.mergeSort(v, left, mid)
        result += self.mergeSort(v, mid, right)
        u = v[left:mid]
        left_sz = mid - left

        h = 0
        for j in range(mid, right):
            while h < left_sz and u[h] <= 2 * v[j]:
                h += 1
            if h == left_sz:
                break
            result += left_sz - h

        i = 0
        j = mid
        k = left
        while i < left_sz and j < right:
            if u[i] < v[j]:
                v[k] = u[i]
                i += 1
            else:
                v[k] = v[j]
                j += 1
            k += 1
        while i < left_sz:
            v[k] = u[i]
            i += 1
            k += 1
        return result

    def mergeSortSmall(self, v, left, right):
        result = 0
        for i in range(left, right):
            for j in range(i + 1, right):
                if v[i] > 2 * v[j]:
                    result += 1
        v[left:right] = sorted(v[left:right])
        return result

    def checkInclusion(self, s1: str, s2: str) -> bool:
        m = len(s1)
        n = len(s2)
        if m > n:
            return False

        def decr(d, k, by=1):
            d[k] -= by
            if d[k] == 0:
                del d[k]

        def incr(d, k, by=1):
            d[k] += by
            if d[k] == 0:
                del d[k]

        pattern = Counter(s1)
        haystack = Counter(s2[:m])
        for k, v in pattern.items():
            decr(haystack, k, v)
        if not haystack:
            return True
        for i in range(m, n):
            prev_ch = s2[i - m]
            decr(haystack, prev_ch)
            incr(haystack, s2[i])
            if not haystack:
                return True
        return False

if __name__ == '__main__':
    soln = Solution()

    print('sortList:')
    v1 = [4, 2, 3, 1, 6, 5, 9, 8]
    v2 = [-1, 5, 3, 4, 0]
    for v in [v1, v2]:
        l = None
        for x in reversed(v):
            l = ListNode(x, l)
        res = soln.sortList(l)
        soln.print_list(res)
    print()

    print('calculate:')
    res = soln.calculate('(2+6* 3+5- (3*14/7+2)*5)+3')
    print(res)
    print()

    print('reversePairs:')
    nums = [-12,-12,-9]
    res = soln.reversePairs(nums)
    print(res)
    print()

    print('checkInclusion:')
    s1 = "ab"
    s2 = "eidbaooo"
    res = soln.checkInclusion(s1, s2)
    print(res)
    print()
