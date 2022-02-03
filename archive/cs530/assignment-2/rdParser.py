"""
rdParser.py - LL recursive descent parser
    Author     : Brandon Nguyen (823630399)
    Course     : CS-530 - Systems Programming
    Instructor : Ben Shen
    Session    : Fall 2021
    School     : San Diego State University

Affidavit:
I the undersigned promise that the attached assignment is my own work. While I
was free to discuss ideas with others, the work contained is my own. I recognize
that should this not be the case, I will be subject to penalties as outlined in
the course syllabus.

Signed: Brandon Nguyen (823630399)
"""

import re
from functools import *
from typing import List, Set


class recDescent:
    ## Static Variables
    # Relational (unary) operators (prefix)
    rel_ops: Set[str] = set(["<", ">", "<=", ">=", "=", "!=", "not"])

    # Dash operator (infix)
    dash_op: str = "-"

    # Logical (binary) operators (infix)
    logic_ops: Set[str] = set(["and", "or", "nand", "xor", "xnor"])

    priority: int = 0

    # Priority tokens
    paren_open: str = "("
    paren_close: str = ")"

    # IMPORTANT:
    # You MUST NOT change the signatures of
    # the constructor, lex(self) and validate(self)
    # Otherwise, autograding tests will FAIL
    # constructor to initialize and set class level variables
    def __init__(self, expr=""):
        # string to be parsed
        self.expr = expr
        # tokens from lexer tokenization of the expression
        self.tokens = []

    # lexer - tokenize the expression into a list of tokens
    # the tokens are stored in an list which can be accessed by self.tokens
    # do not edit any piece of code in this function
    def lex(self):
        self.tokens = re.findall("[-\(\)=]|[!<>]=|[<>]|\w+|[^ +]\W+", self.expr)
        # transform tokens to lower case, and filter out possible spaces in the tokens
        self.tokens = list(
            filter(
                (lambda x: len(x)),
                list(map((lambda x: x.strip().lower()), self.tokens)),
            )
        )

    def expr_parser(self, stack: List[str], position: int, length: int) -> int:
        # End early if input is too short
        if length == 0 or length == 1:
            return 0
        if self.priority <= -1:
            return 0
        if position == length and self.priority == 0:
            return 1
        if position == length and self.priority != 0:
            return 0
        # Found open parenthesis, increase priority
        if stack[position] == self.paren_open:
            stack[position] = "0"
            self.priority += 1
            # recursive move to the right and re-parse
            return self.expr_parser(stack, position + 1, length)
        # Found close parenthesis, decrease priority
        if stack[position] == self.paren_close:
            stack[position] = "0"
            self.priority -= 1
            return self.expr_parser(stack, position + 1, length)
        if stack[position].isnumeric():
            return self.term_parser(stack, position, length)
        elif stack[position] == self.dash_op:
            return self.term_parser(stack, position, length)
        elif stack[position] in self.rel_ops:
            return self.term_parser(stack, position, length)
        if stack[position] in self.logic_ops:
            # fail if at start or end
            if position == 0 or position == length - 1:
                return 0
            else:
                return self.expr_parser(stack, position + 1, length)

    def term_parser(self, stack: List[str], position: int, length: int) -> int:
        if stack[position].isnumeric():
            return self.expr_parser(stack, position + 1, length)
        elif stack[position] == self.dash_op:
            if position == 0 or position == length - 1:
                return 0
            # previous and next position need to be numbers for range
            elif stack[position - 1].isnumeric() and stack[position + 1].isnumeric():
                return self.expr_parser(stack, position + 1, length)
            else:
                return 0
        elif stack[position] in self.rel_ops:
            if stack[position] == "!=" or stack[position] == "=":
                # Fail if located at end
                if position == length - 1:
                    return 0
                # success if next is number or logical op
                elif (
                    stack[position + 1].isnumeric()
                    or stack[position + 1] in self.logic_ops
                ):
                    # recursive move to the right and re-parse
                    return self.expr_parser(stack, position + 1, length)
                else:
                    return 0
            # Check validity with surrounding characters
            elif (
                stack[position] == ">"
                or stack[position] == ">="
                or stack[position] == "not"
            ):
                # Fail if located at end
                if position == (length - 1):
                    return 0
                # followed by an int token or logical operator
                elif (
                    stack[position + 1].isnumeric()
                    or stack[position + 1] in self.logic_ops
                ):
                    # recursive move to the right and re-parse
                    return self.expr_parser(stack, position + 1, length)
                else:
                    return 0
            # otherwise checking these operators
            elif stack[position] == "<" or stack[position] == "<=":
                # if less than operators are in front, invalid
                if position == 0:
                    return 0
                # valid if preceding said operator is int or logical operator
                elif (
                    stack[position - 1].isnumeric()
                    or stack[position - 1] in self.logic_ops
                ):
                    return self.expr_parser(stack, position + 1, length)
                else:
                    return 0

    # validate() function will return True if the expression is valid, False otherwise
    # do not change the method signature as this function will be called by the autograder
    def validate(self):
        # Using the tokens from lex() tokenization,
        # your validate would first call lex() to first tokenize the expression
        # then call the top level parsing procedure and go from there
        self.lex()
        stack: List[str] = self.tokens
        flag: int = self.expr_parser(stack, 0, len(stack))
        return flag == 1


# parsing procedures corresponding to the grammar rules - follow Figure 5.17

# Simple test procedure
def test_parser():
    check = {
        # assignment
        "7 - 17": True,
        "> 90": True,
        "(1 - 100 and not 50) or > 200": True,
        "(7 - 17) or > 90": True,
        "> 50 or = 20": True,
        "1 - 100 and != 50": True,
        "(5 - 100) and (not 50) or (>= 130 or (2 - 4))": True,
        ">": False,
        "2 - - 4": False,
        "- 7": False,
        "7 -": False,
        "(!= 5) and": False,
        "2 - 4 and >< 300": False,
        ">= 5) xnor < 1": False,
        # autograder
        "1 - 7": True,
        "> 90": True,
        "1 - 100 and not 50": True,
        "> 50 xnor = 20": True,
        "((1- 100) xor > 150) and < 300": True,
        "1 - 100 and != 50": True,
        "(100 - 200) or (>= 30)": True,
        "(5 - 100) nand (not 50) and (>= 30 or (6 - 12))": True,
        "7 - ": False,
        "> ": False,
        "(7 - 17": False,
        "(7 - 17 or > ": False,
        "2 - 4 and >< 300": False,
        ">= 5) xnor < 10": False,
        "> 120 > 710": False,
        # custom
        "": False,
        "5": False,
    }

    for key, value in check.items():
        prog: recDescent = recDescent(key)
        result = prog.validate()
        try:
            assert result == value
        except AssertionError:
            print(f"!!FAILED!! {key} returned {result}.")


# FIXME: comment out when submitting
# test_parser()
