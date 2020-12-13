# CS 320 - Professor Liu
# Assignment 2
# Programmed by : Brandon Nguyen <bnguyen6720@sdsu.edu>
# RedID : 823630399

from typing import List, Tuple


def min_max1(sequence: List[int]) -> Tuple[int, int]:


def min_max2(sequence: List[int]) -> Tuple[int, int]:
    """Returns the minimal and maximal number in an unsorted sequence.

    Solution 2: Non-recursive O(n), maintaining hi and lo.

    Note that this function runs slower than min_max1 despite similar
    implementations. The built-in Python min() and max() functions are
    optimized in C.

    Args:
        sequence (List[int]): Unsorted sequence of ints.

    Returns:
        Tuple[int, int]: (Minimal, Maximal) of the inputted sequence.
    """
    if len(sequence) == 1:
        return (sequence[0], sequence[0])
    lo = hi = sequence[0]
    for num in sequence[1:]:
        if num > hi:
            hi = num
        elif num < lo:
            lo = num
    return (lo, hi)


def min_max_recursive(sequence: List[int]) -> Tuple[int, int]:
    """Returns the minimal and maximal number in an unsorted sequence.

    Question 4: Recursive O(2^n), using slices.

    Args:
        sequence (List[int]): List to search over.

    Returns:
        Tuple[int, int]: (Minimal, Maximal) of the inputted sequence.
    """
    if len(sequence) == 2:
        return (min(sequence), max(sequence))
    else:
        return (
            min(sequence[0], min_max_recursive(sequence[1:])[0]),
            max(sequence[0], min_max_recursive(sequence[1:])[1]),
        )


def reverse_recursive(instr: str) -> str:
    """Returns the inputted string, reversed.

    Solution 5: Recursive O(n), using slices.

    Args:
        instr (str): String to reverse.

    Returns:
        str: Reversed string.
    """
    if len(instr) == 1:
        return instr
    else:
        # Add last char and call function again without last char
        return instr[-1] + reverse_recursive(instr[:-1])


def main():
    assert min_max1([10, 39, 47, 1, 49, 100]) == (1, 100)
    assert min_max2([10, 39, 47, 1, 49, 100]) == (1, 100)
    assert min_max_recursive([10, 39, 47, 1, 49, 100]) == (1, 100)
    assert reverse_recursive("SDSU@San") == "naS@USDS"


if __name__ == "__main__":
    main()
