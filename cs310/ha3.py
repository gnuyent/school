# CS 320 - Professor Liu
# Assignment 2
# Programmed by : Brandon Nguyen <bnguyen6720@sdsu.edu>
# RedID : 823630399

from typing import List


def script_one():
    # Referential array
    A = [[10, 5], [30], [40]]
    B = A
    B[0][1] = 3
    print(A)


def script_two():
    # Shallow copy
    A = [[10, 5], [30], [40]]
    B = list(A)
    B[0][1] = 3
    print(A)


def script_three():
    # Deep copy
    import copy

    A = [[10, 5], [30], [40]]
    B = copy.deepcopy(A)
    B[0][1] = 3
    print(A)


def matrix(n: int) -> List[List[int]]:
    """Performs operations on matrices.

    Question 2:
        (i) Randomly generates an int matrix A of n x n size with random values.
        (ii) Calculate mean of all elements in matrix.
        (iii) Create a matrix B as the transpose of A.
        (iv) Subtract the mean from each element of B.

    Args:
        n (int): Matrix size

    Returns:
        List[List[int]]: Modified matrix B
    """
    import random

    # (i) Generate an int matrix of n x n with values between
    # 0, 255 (inclusive). O(n^2)
    A: List[List[int]] = [[random.randint(0, 255) for x in range(n)] for i in range(n)]
    # (ii) Calculate mean of all elements in matrix. O(n^2)
    mean: float = 0
    for row in A:
        for col in row:
            mean += col

    mean /= n * n
    # (iii) Transpose of A. O(n^2)
    B: List[List[int]] = [[A[j][i] for j in range(n)] for i in range(n)]
    # (iv) Subtract mean value from each element of B. O(n^2)
    B = [[A[i][j] - mean for j in range(n)] for i in range(n)]
    return B


class LinkedList:
    """
    Question 4: Operations for a singly linked list.
    """

    class _Node:
        def __init__(self, element, next):  # initialize node's fields
            self._element: int = element  # reference to integers
            self._next: self._Node = next  # reference to next node

    def __init__(self):
        """Create an empty list."""
        self._head: self._Node = None  # reference to the head node
        self._size: int = 0  # number of stack elements

    def insert_head(self, value):
        if self._head is not None:  # list is not empty
            self._head = self._Node(value, self._head)
        else:  # list is empty
            self._head = self._Node(value, None)

        self._size += 1

    def insert(self, value):
        if self._head is None:  # list is empty
            self.insert_head(value)
        else:
            focus = self._head
            while focus._next:
                focus = focus._next
            focus._next = self._Node(value, None)

        self._size += 1

    def size(self) -> int:
        return self._size

    def next(self):
        if self._head is not None:
            self._head = self._head._next

    def __str__(self):
        out_str = "[ "
        last = self._head
        while last:
            out_str += str(last._element) + " "
            last = last._next
        out_str += "]"
        return out_str

    def max(self) -> int:
        node = self._head._next
        maximum: int = self._head._element
        while node is not None:
            if node._element > maximum:
                maximum = node._element
            node = node._next

        return maximum


def merge_sorted_linked_list(list1: LinkedList, list2: LinkedList) -> LinkedList:
    # [1, 3, 5] [2, 4, 6] -> [1, 2, 3, 4, 5, 6]
    merged_list = LinkedList()

    if list1._head is None:
        return list2
    elif list2._head is None:
        return list1

    list1_data = list1._head._element
    list2_data = list2._head._element

    node = None
    if list1_data <= list2_data:
        node = list1._head
        list1.next()
        node._next = merge_sorted_linked_list(list1, list2)._head
    else:  # list2_data < list1_data
        node = list2._head
        list2.next()
        node._next = merge_sorted_linked_list(list1, list2)._head

    merged_list._head = node
    return merged_list


def main():
    # print("Script One:")
    # script_one()
    # print("Script Two:")
    # script_two()
    # print("Script Three:")
    # script_three()
    # print("Matrix with n=3")
    # print(matrix(3))
    # print("LinkedList")
    # ll = LinkedList()
    # ll.insert_head(5)
    # print(ll.size())
    # print(ll.max())
    # ll.insert_head(10)
    # print(ll.max())
    # print(ll.size())
    print("Merge linked list")
    ll1 = LinkedList()
    ll1.insert(2)
    ll1.insert(4)
    ll1.insert(7)
    print(ll1)
    ll2 = LinkedList()
    ll2.insert(1)
    ll2.insert(2)
    ll2.insert(2)
    print(ll2)
    ll3 = merge_sorted_linked_list(ll1, ll2)
    print(ll3)


if __name__ == "__main__":
    main()
