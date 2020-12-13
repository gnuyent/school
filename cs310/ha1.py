# CS 320 - Professor Liu
# Assignment 1
# Programmed by : Brandon Nguyen <bnguyen6720@sdsu.edu>
# RedID : 823630399

from typing import List


def factorial(num: int) -> int:
    """Calculate factorial of an int.

    Problem 1.

    Args:
        num (int): Number to calculate factorial of.

    Returns:
        int: Factorial of the input.
    """
    if num < 0:
        raise ValueError(f"{num} cannot be negative.")
    elif num == 0:
        return 1
    else:
        total: int = 1
        while num > 1:
            total *= num
            num -= 1
        return total


def odd_pairs(sequence: List[int]) -> bool:
    """Determines if any sequential pairs have an odd product.

    Problem 2.

    Args:
        sequence (List[int]): List of integers in any order.

    Returns:
        bool: True if any sequential pair has an odd product, False otherwise.
    """
    while len(sequence) > 1:
        focus_odd: bool = sequence.pop() % 2 == 1
        next_odd: bool = (
            sequence[-1] % 2 == 1
        )  # The "next" number is now the last element
        # Two integers have an odd product iff both are odd
        if focus_odd and next_odd:
            return True

    return False


def reverse_integer(normal: int) -> int:
    """Takes an integer and reverses it.

    Problem 3.

    Args:
        normal (int): Integer to reverse.

    Returns:
        int: A reversed integer.
    """
    negative: bool = abs(normal) != normal
    reverse = int(str(abs(normal))[::-1])
    if negative:
        return reverse * -1
    return reverse


def remove_commas(sentence: str) -> str:
    """Removes commas from a given string.

    Problem 4.

    Args:
        sentence (str): String containing commas.

    Returns:
        str: String without commas.
    """
    return sentence.replace(",", "")


def check_closers(sentence: str) -> bool:
    """Check if brackets and parentheses are properly closed.

    Problem 5.

    Args:
        sentence (str): String to check.

    Returns:
        bool: True if string is valid, false otherwise.
    """
    opening_delimiters: List[str] = ["(", "{", "["]
    closing_dict: List[str] = {")": "(", "}": "{", "]": "["}
    delimiters: List = []  # stack

    for character in sentence:
        if character in opening_delimiters:
            delimiters.append(character)
        elif character in closing_dict:
            if closing_dict[character] != delimiters.pop():
                return False

    return True


def merge_and_sort(list1: List, list2: List) -> List:
    """Combines two input lists and sorts them.

    Args:
        list1 (List): First list to sort.
        list2 (List): Second list to sort.

    Returns:
        List: Sorted combined list.
    """
    one_count: int = 0
    two_count: int = 0
    sorted_list: List = []
    if len(list1) == 0:
        return list2
    if len(list2) == 0:
        return list1
    while len(list1) > 0 or len(list2) > 0:
        try:
            one_val = list1[one_count]
        except IndexError:
            sorted_list += list2[two_count:]
            break
        try:
            two_val = list2[two_count]
        except IndexError:
            sorted_list += list1[one_count:]
            break
        if one_val < two_val:
            sorted_list.append(one_val)
            one_count += 1
        elif two_val < one_val:
            sorted_list.append(two_val)
            two_count += 1
        else:  # equals
            sorted_list.append(one_val)
            one_count += 1

    return sorted_list


def main():
    print(merge_and_sort([1, 3, 4], [1, 2, 6, 8]))


if __name__ == "__main__":
    main()