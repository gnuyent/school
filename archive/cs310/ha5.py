# CS 320 - Professor Liu
# Assignment 5
# Programmed by : Brandon Nguyen <bnguyen6720@sdsu.edu>
# RedID : 823630399

from typing import List
import unittest


class PriorityQueueBase:
    """Abstract base class for a priority queue."""

    # ------------------------------ nested _Item class ------------------------------
    class _Item:
        """Lightweight composite to store priority queue items."""

        __slots__ = "_key", "_value"

        def __init__(self, k, v):
            self._key = k
            self._value = v

        def __lt__(self, other):
            return self._key < other._key  # compare items based on their keys

        def __repr__(self):
            return "({0},{1})".format(self._key, self._value)

    # ------------------------------ public behaviors ------------------------------
    def is_empty(self):  # concrete method assuming abstract len
        """Return True if the priority queue is empty."""
        return len(self) == 0

    def __len__(self):
        """Return the number of items in the priority queue."""
        raise NotImplementedError("must be implemented by subclass")

    def add(self, key, value):
        """Add a key-value pair."""
        raise NotImplementedError("must be implemented by subclass")

    def min(self):
        """Return but do not remove (k,v) tuple with minimum key.

        Raise Empty exception if empty.
        """
        raise NotImplementedError("must be implemented by subclass")

    def remove_min(self):
        """Remove and return (k,v) tuple with minimum key.

        Raise Empty exception if empty.
        """
        raise NotImplementedError("must be implemented by subclass")


class HeapPriorityQueue(PriorityQueueBase):  # base class defines _Item
    """A min-oriented priority queue implemented with a binary heap."""

    # ------------------------------ nonpublic behaviors ------------------------------
    def _parent(self, j):
        return (j - 1) // 2

    def _left(self, j):
        return 2 * j + 1

    def _right(self, j):
        return 2 * j + 2

    def _has_left(self, j):
        return self._left(j) < len(self._data)  # index beyond end of list?

    def _has_right(self, j):
        return self._right(j) < len(self._data)  # index beyond end of list?

    def _swap(self, i, j):
        """Swap the elements at indices i and j of array."""
        self._data[i], self._data[j] = self._data[j], self._data[i]

    def _upheap(self, j):
        #
        # NON RECURSIVE IMPLEMENTATION
        #
        parent = self._parent(j)
        while j > 0 and self._data[j] < self._data[parent]:
            self._swap(j, parent)
            j = parent
            parent = self._parent(j)

    def _downheap(self, j):
        if self._has_left(j):
            left = self._left(j)
            small_child = left  # although right may be smaller
            if self._has_right(j):
                right = self._right(j)
                if self._data[right] < self._data[left]:
                    small_child = right
            if self._data[small_child] < self._data[j]:
                self._swap(j, small_child)
                self._downheap(small_child)  # recur at position of small child

    # ------------------------------ public behaviors ------------------------------
    def __init__(self):
        """Create a new empty Priority Queue."""
        self._data = []

    def __len__(self):
        """Return the number of items in the priority queue."""
        return len(self._data)

    def add(self, key, value):
        """Add a key-value pair to the priority queue."""
        self._data.append(self._Item(key, value))
        self._upheap(len(self._data) - 1)  # upheap newly added position

    def min(self):
        """Return but do not remove (k,v) tuple with minimum key.

        Raise KeyError if empty.
        """
        if self.is_empty():
            raise KeyError("Priority queue is empty.")
        item = self._data[0]
        return (item._key, item._value)

    def remove_min(self):
        """Remove and return (k,v) tuple with minimum key.

        Raise KeyError if empty.
        """
        if self.is_empty():
            raise KeyError("Priority queue is empty.")
        self._swap(0, len(self._data) - 1)  # put minimum item at the end
        item = self._data.pop()  # and remove it from the list;
        self._downheap(0)  # then fix new root
        return (item._key, item._value)


def heapify(arr: List[int], size: int, start: int):
    """
    Create a heap out of an unsorted array.

    Uses downheap approach.

    Parameters
    ----------
    arr : List[int]
        Unsorted array.
    size: int
        Size of heap.
    start: int
        Index to begin at.
    """
    left = 2 * start + 1
    right = 2 * start + 2
    largest = start

    if left < size and arr[left] > arr[largest]:
        largest = left

    if right < size and arr[right] > arr[largest]:
        largest = right

    if largest != start:
        arr[start], arr[largest] = arr[largest], arr[start]  # swap
        heapify(arr, size, largest)


def HeapSortInPlace(S: List[int]) -> List[int]:
    """
    Perform an in-place heap sort on an unordered list.

    Parameters
    ----------
    S : List[int]
        An unsorted list of integers.

    Returns
    -------
    List[int]:
        A sorted list of integers.
    """

    # Build Max Heap
    import math

    for idx in range(math.floor(len(S) / 2) - 1, -1, -1):  # maxheap
        heapify(S, len(S), idx)

    for idx in range(len(S) - 1, 0, -1):
        S[idx], S[0] = S[0], S[idx] # swap maximum at front with last index
        heapify(S, idx, 0) # repair heap not including last index

    return S


class Testha5(unittest.TestCase):
    def test_nonrecursive(self):
        P = HeapPriorityQueue()
        P.add(5, "A")
        P.add(9, "C")
        P.add(3, "B")
        P.add(7, "D")
        self.assertEqual((3, "B"), P.min())
        self.assertEqual((3, "B"), P.remove_min())
        self.assertEqual((5, "A"), P.remove_min())
        self.assertEqual(2, len(P))
        self.assertEqual((7, "D"), P.remove_min())
        self.assertEqual((9, "C"), P.remove_min())
        self.assertTrue(P.is_empty())
        self.assertRaises(KeyError, lambda: P.remove_min())

    def test_heapsort(self):
        self.assertEqual(
            [1, 2, 3, 4, 5, 9, 10], HeapSortInPlace([10, 3, 4, 1, 9, 2, 5])
        )


# Driver code
if __name__ == "__main__":
    from os import system, name

    # clear screen for posterity
    if name == "nt":
        system("cls")
    else:
        system("clear")

    # Question 1
    count = 1
    print("Question #1")
    q1 = HeapPriorityQueue()
    q1.add(1, "a")
    q1.add(3, "b")
    q1.add(2, "c")
    q1.add(5, "e")
    kv = q1.remove_min()
    print(f"remove_min() {count}: key = {kv[0]}")
    count += 1
    q1.add(4, "j")
    q1.add(9, "k")
    kv = q1.remove_min()
    print(f"remove_min() {count}: key = {kv[0]}")
    count += 1
    kv = q1.remove_min()
    print(f"remove_min() {count}: key = {kv[0]}")
    count += 1

    # Question 4/5
    print("\n\nQuestion #4/5")
    unittest.main()
