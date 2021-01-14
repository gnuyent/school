# CS 320 - Professor Liu
# Assignment 6
# Programmed by : Brandon Nguyen <bnguyen6720@sdsu.edu>
# RedID : 823630399

from copy import deepcopy  # ensures consistency in benchmark
import random  # random input for benchmarking
import time  # times benchmarks
from typing import Dict, List


def timer(func):
    def wrapper(*args):
        start = time.time()
        value = func(*args)
        end = time.time()
        print(f"completed in {round(end-start, 5)} seconds")
        return value

    return wrapper


# following merge sort alg taken from lecture slides
def merge(S1, S2, S):
    i = j = 0
    while i + j < len(S):
        if j == len(S2) or (i < len(S1) and S1[i] < S2[j]):
            S[i + j] = S1[i]
            i += 1
        else:
            S[i + j] = S2[j]
            j += 1


def merge_sort(S):
    n = len(S)
    if n < 2:
        return
    mid = n // 2
    S1 = S[0:mid]
    S2 = S[mid:n]
    merge_sort(S1)
    merge_sort(S2)
    merge(S1, S2, S)


@timer
def get_winner_sort(S: List[int]) -> int:
    """
    Determines the election winner given a list of votes containing candidate IDs.

    This implementation runs in O(nlogn) using an interpreted merge sort (not optimized in C).

    Parameters
    ----------
    S : List[int]
        List of candidate IDs which were voted.

    Returns
    -------
    int:
        Int containing the ID of the winning candidate.
    """
    merge_sort(S)
    number = S[0]
    greatest = S[0]
    count = 0
    greatest_count = 0
    for x in S:
        if x == number:
            count += 1
        else:  # we are at a different number
            count = 1
            number = x
        if count > greatest_count:
            greatest_count = count
            greatest = number

    return greatest


@timer
def get_winner_sort_builtin(S: List[int]) -> int:
    """
    Determines the election winner given a list of votes containing candidate IDs.

    This implementation runs in O(nlogn) using Python's built-in sorting method (optimized in C). See https://wiki.python.org/moin/TimeComplexity#list
    Python's in-built sorting algorithm uses a modified version of merge sort called "timsort". Information can be found here: https://svn.python.org/projects/python/trunk/Objects/listsort.txt

    Parameters
    ----------
    S : List[int]
        List of candidate IDs which were voted.

    Returns
    -------
    int:
        Int containing the ID of the winning candidate.
    """
    S.sort()
    number = S[0]
    greatest = S[0]
    count = 0
    greatest_count = 0
    for x in S:
        if x == number:
            count += 1
        else:  # we are at a different number
            count = 1
            number = x
        if count > greatest_count:
            greatest_count = count
            greatest = number

    return greatest


@timer
def get_winner(S: List[int]) -> int:
    """
    Determines the election winner given a list of votes containing candidate IDs.

    This implementation runs in O(n) using a dictionary. In the case that there is a tie, this algorithm returns the candidate that was encountered first in the given list.

    Parameters
    ----------
    S : List[int]
        List of candidate IDs which were voted.

    Returns
    -------
    int:
        Int containing the ID of the winning candidate.
    """
    # key: vote, val: total votes. This is a Python dictionary.
    map: Dict[int, int] = {}
    highest_id = S[0]
    highest_count = 1
    for vote in S:  # O(n)
        # This dict lookup runs in O(1) for integers (unique ID), see https://wiki.python.org/moin/TimeComplexity#dict
        # The dict implementation in CPython relies on a hash table with a hashing function that is guaranteed to be unique for integers.
        # This allows the dictionary implementation (and subsequent "in" search) to run in O(1), regardless of input size (S).
        # For information on hashing built-in objects : https://docs.python.org/3/glossary.html#term-hashable
        # For information on Python's dict            : https://docs.python.org/3/library/stdtypes.html#mapping-types-dict
        if vote in map:
            map[vote] += 1
        else:
            map[vote] = 1
        if map[vote] > highest_count:
            highest_id = vote
            highest_count = map[vote]
    return highest_id


def test_one(val: int):
    """Randomized input."""
    arr = [x for x in range(val)]
    arr.append(val - 1)
    random.shuffle(arr)
    one = deepcopy(arr)
    print(f"Testing values 0-{val} (random) with dict... ", end="")
    assert (val - 1) == get_winner(one)
    two = deepcopy(arr)
    print(f"Testing values 0-{val} (random) with merge sort... ", end="")
    assert (val - 1) == get_winner_sort(two)
    three = deepcopy(arr)
    print(f"Testing values 0-{val} (random) with built-in sort... ", end="")
    assert (val - 1) == get_winner_sort_builtin(three)


def test_two(val: int):
    """Sorted input, duplicate at end."""
    arr = [x for x in range(val)]
    arr.append(val - 1)
    one = deepcopy(arr)
    print(f"Testing values 0-{val} (sorted, dupe at end) with dict... ", end="")
    assert (val - 1) == get_winner(one)
    two = deepcopy(arr)
    print(f"Testing values 0-{val} (sorted, dupe at end) with merge sort... ", end="")
    assert (val - 1) == get_winner_sort(two)
    three = deepcopy(arr)
    print(
        f"Testing values 0-{val} (sorted, dupe at end) with built-in sort... ", end=""
    )
    assert (val - 1) == get_winner_sort_builtin(three)


def test_three(val: int):
    """Sorted input, duplicate at start."""
    arr = [x for x in range(val)]
    arr.append(0)
    one = deepcopy(arr)
    print(f"Testing values 0-{val} (sorted, dupe at start) with dict... ", end="")
    assert 0 == get_winner(one)
    two = deepcopy(arr)
    print(f"Testing values 0-{val} (sorted, dupe at start) with merge sort... ", end="")
    assert 0 == get_winner_sort(two)
    three = deepcopy(arr)
    print(
        f"Testing values 0-{val} (sorted, dupe at start) with built-in sort... ", end=""
    )
    assert 0 == get_winner_sort_builtin(three)


if __name__ == "__main__":
    print("No AssertionErrors means that a winner was successfully found!\n\n")
    SMALL_NUM = 100000
    MED_NUM = 500000
    BIG_NUM = 1000000
    test_one(SMALL_NUM)
    test_one(MED_NUM)
    test_one(BIG_NUM)
    print()
    test_two(SMALL_NUM)
    test_two(MED_NUM)
    test_two(BIG_NUM)
    print()
    test_three(SMALL_NUM)
    test_three(MED_NUM)
    test_three(BIG_NUM)
