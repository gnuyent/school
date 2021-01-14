from __future__ import annotations

from typing import Any, List, Optional


class ArrayBinaryTree:
    """Array representation of a binary tree structure."""

    # nested _Node class

    class _Node:
        """Lightweight, nonpublic class for storing a node."""

        __slots__ = (
            "_element",
            "_rank",
        )  # streamline memory usage

        def __init__(
            self,
            element: Any,
            rank: int,
        ) -> None:
            self._element: Any = element
            self._rank: int = rank

    # remove Position because it is an unnecessary abstraction.
    # we can find the position already through the array

    # binary tree constructor

    def __init__(self) -> None:
        """Create an initially empty binary tree."""
        self._tree: List[_Node] = [None]
        self._size: int = 0

    # public accessors
    def __len__(self) -> int:
        """
        Number of elements in the tree.

        Returns
        -------
        int:
            The total number of elements in the tree.
        """
        return self._size

    def root(self) -> Optional[_Node]:
        """
        Return the root node of the tree.

        Returns
        -------
        Optional[_Node]:
            Root node of the tree or None if tree is empty.
        """
        try:  # tree may be empty
            return self._tree[1]
        except IndexError:
            return None

    def parent(self, node: _Node) -> Optional[_Node]:
        """
        Return the node of p's parent.

        Parameters
        ----------
        node : _Node
            _Node containing the parent.

        Returns
        -------
        Optional[_Node]:
            _Node of p's parent or None if p is root.
        """
        parent = self._tree[node._rank / 2]
        if parent == self.root():
            return None
        return parent

    def left(self, node: _Node) -> Optional[_Node]:
        """
        Return node's left child.

        Parameters
        ----------
        node: _Node
            _Node containing the left child.

        Returns
        -------
        Optional[_Node]:
            _Node of node's left child or None if no left child.
        """
        left = None
        try:
            left = self._tree[node._rank * 2]
        except (AttributeError, IndexError):
            pass
        return left

    def right(self, node: _Node) -> Optional[_Node]:
        """
        Return node's right child.

        Parameters
        ----------
        node : _Node
            _Node containing the right child.

        Returns
        -------
        Optional[_Node]:
            _Node of node's right child or None if no right child.
        """
        right = None
        try:
            right = self._tree[node._rank * 2 + 1]
        except (AttributeError, IndexError):
            pass
        return right

    def num_children(self, node: _Node) -> int:
        """
        Return the number of children of node.

        Parameters
        ----------
        node : _Node
            _Node to begin at.

        Returns
        -------
        int:
            Number of children at _Node node.
        """
        count = 0
        if self.left(node) is not None:  # left child exists
            count += 1
        if self.right(node) is not None:  # right child exists
            count += 1
        return count

    # nonpublic mutators
    def _add_root(self, e: Any) -> _Node:
        """
        Place element e at the root of an empty tree and return position.

        Parameters
        ----------
        e : Any
            Element to place at root.

        Returns
        -------
        _Node:
            _Node of element at root.

        Raises
        ------
        ValueError:
            If tree is nonempty.
        """
        if self.root() is not None:
            raise ValueError("Root exists")
        node = self._Node(e, 1)
        self._size = 1
        self._tree.append(node)
        return node

    def _grow(self, size: int):
        """
        Grow array to fit all nodes.

        Parameters
        ----------
        size : int
            Size to grow to.
        """
        while len(self._tree) < size:
            self._tree.append(None)

    def _add_left(self, node: _Node, e: Any):
        """
        Create new left child for _Node node, storing element e.

        Parameters
        ----------
        node : _Node
            _Node to create left child for.
        e : Any
            Element to store.

        Returns
        -------
        _Node:
            The newly created left node.

        Raises
        ------
        ValueError:
            If left child exists.
        """
        left_rank = node._rank * 2
        if self._tree[left_rank] is not None:
            raise ValueError("Left child exists")
        self._size += 1
        self._grow(left_rank)
        left_node = _Node(e, left_rank)
        self._tree[left_rank] = left_node
        return left_node

    def _add_right(self, p: _Node, e: Any):
        """
        Create new right child for _Node node, storing element e.

        Parameters
        ----------
        node : _Node
            _Node to create right child for.
        e : Any
            Element to store.

        Raises
        ------
        ValueError:
            If right child exists.
        """
        right_rank = node._rank * 2 + 1
        if self._tree[right_rank] is not None:
            raise ValueError("Right child exists")
        self._size += 1
        self._grow(right_rank)
        right_node = _Node(e, right_rank)
        self._tree[right_rank] = right_node
        return right_node

    def _replace(self, node: _Node, e: Any) -> Any:
        """
        Replace the element at _Node node with e, return old element.

        Parameters
        ----------
        node : _Node
            _Node to replace.
        e : Any
            Element to replace with

        Returns
        -------
        Any:
            Element that was replaced.
        """
        old = node._element
        node._element = e
        return old

    def _delete(self, node: _Node) -> Any:
        """
        Delte the node at _Node node, and replace it with its child, if any.

        Deletes the node at _Node node and replaces it with its child if it exists.
        Returns the element that has been stored at _Node node.

        Parameters
        ----------
        node : _Node
            _Node to delete at.

        Returns
        -------
        Any:
            Element that was stored at _Node node.

        Raises
        ------
        ValueError:
            If _Node node is invalid or node has two children.
        """
        if self.num_children(node) == 2:
            raise ValueError("_Node has two children")

        left_rank = 2 * node._rank
        right_rank = left_rank + 1

        try:
            left = self._tree[left_rank]
        except IndexError:
            left = None
        try:
            right = self._tree[right_rank]
        except IndexError:
            right = None

        child = left if left else right  # might be None

        if child is not None:
            child._parent = node._parent  # child's grandparent becomes parent

        if node is self._tree[1]:
            self._tree[1] = child  # child becomes root
        else:
            parent = self.parent(node)
            # if node is self.left(parent):
            #    parent._left = child
            # else:
            #    parent._right = child

        self._size -= 1
        # node._parent = node  # convention for deprecated node
        return node._element

    def _attach(self, node: Optional[_Node], t1: ArrayBinaryTree, t2: ArrayBinaryTree):
        """
        Attach trees t1 and t2 to _Node node.

        Attach trees t1 and t2, respectively, as the left and right subtrees of the
        external _Node node. As a side effect, set t1 and t2 to empty.

        Parameters
        ----------
        node : _Node
            _Node to attach trees to.
        t1 : ArrayBinaryTree
            Tree that will be attached to the left.
        t2 : ArrayBinaryTree
            Tree that will be attached to the right.

        Raises
        ------
        TypeError:
            If trees t1 and t2 do not match type of this tree.
        ValueError:
            If _Node node is invalid or not external.
        """
        if self.num_children(node) != 0:  # validate that this is a leaf
            raise ValueError("position must be leaf")
        if t1 is None and t2 is None:
            return
        elif t1 is None and t2 is not None:
            self._size += len(t2)
        elif t1 is not None and t2 is None:
            self._size += len(t1)
        else:
            self._size += len(t1) + len(t2)
        self._grow(self._size)
        idx = node._rank * 2
        if len(t1) > 1:  # attached t1 as left subtree of node
            self._tree[idx] = t1
        if len(t2) > 1:  # attached t2 as right subtree of node
            self._tree[idx + 1] = t2


class ExpressionTreeV2(ArrayBinaryTree):
    """An arithmetic expression tree using the ArrayBinaryTree."""

    def __init__(self, token: str, left=None, right=None) -> None:
        """
        Create an expression tree.

        In single parameter form, token should be a leaf value (e.g., '42'),
        and the expression tree will have that value at an isolated node.

        In a three-parameter version, token should be an operator, and left and right
        should be an existing ExpressionTreeV2 instances that become the operands for
        the binary operator.

        Parameters
        ----------
        token : str
            Token to parse.
        left : ExpressionTreeV2
            ExpressionTreeV2 instance that become operands for the binary operator.
        right : ExpressionTreeV2
            ExpressionTreeV2 instance that become operands for the binary operator.

        Raises
        ------
        TypeError:
            If token is not a string.
        ValueError:
            If token is not a valid operator.
        """
        super().__init__()  # ArrayBinaryTree initialization
        if not isinstance(token, str):
            raise TypeError("token must be a string.")
        self._add_root(token)
        if left is not None:  # presumable three-parameter form
            if token not in "+-*x/":
                raise ValueError("token must be a valid operator")
        self._attach(self.root(), left, right)

    def _parenthesize_recur(self, node, result):
        """Append piecewise representation of node's subtree to resulting list."""
        if self.num_children(node) == 0:
            result.append(str(node._element))
        else:
            result.append("(")
            self._parenthesize_recur(self.left(node), result)
            result.append(node._element)
            self._parenthesize_recur(self.right(node), result)
            result.append(")")

    def __str__(self) -> str:
        """String representation of the expression."""
        pieces: List[str] = []  # sequence of piecewise strings to compose
        self._parenthesize_recur(self.root(), pieces)
        return "".join(pieces)

    def _evaluate_recur(self, node):
        """Return the numeric result of subtree rooted at p."""
        if self.num_children(node) == 0:
            return float(node._element)  # assume element is numeric
        else:
            op = node._element
            left_val = self._evaluate_recur(self.left(node))
            right_val = self._evaluate_recur(self.right(node))
            if op == "+":
                return left_val + right_val
            if op == "-":
                return left_val - right_val
            elif op == "/":
                return left_val / right_val
            else:  # 'x' or '*' as multipication
                return left_val * right_val

    def evaluate(self):
        """Return the numeric result of the expression."""
        return self._evaluate_recur(self.root())


def tokenize(raw: str) -> List[str]:
    """
    Produces list of tokens indicated by a raw expression string.

    For example the string '(43-(3*10))' results in the list
    ['(', '43', '-', '(', '3', '*', '10', ')', ')']

    Parameters
    ----------
    raw : str
        List to tokenize.

    Returns
    -------
    List[str]:
        Tokenized list.
    """
    SYMBOLS = set("+-x*/() ")  # allow for '*' or 'x' for multiplication

    mark: int = 0
    tokens: List[str] = []
    n: int = len(raw)
    for j in range(n):
        if raw[j] in SYMBOLS:
            if mark != j:
                tokens.append(raw[mark:j])  # complete preceding token
            if raw[j] != " ":
                tokens.append(raw[j])  # include this token
            mark = j + 1  # update mark to being at next index
    if mark != n:
        tokens.append(raw[mark:n])  # complete preceding token
    return tokens


def build_expression_tree(tokens: List[str]) -> ExpressionTreeV2:
    """
    Returns an ExpressionTreeV2 based upon by a tokenized expression.

    Tokens must be an iterable of strings representing a fully parenthesized
    binary expression, such as ['(', '43', '-', '(', '3', '*', '10', ')', ')']

    Parameters
    ----------
    tokens : List[str]
        List of tokens to create an ExpressionTreeV2.

    Returns
    -------
    ExpressionTreeV2:
        ExpressionTreeV2 containing the given tokens.
    """
    S: List[Any] = []  # we use Python list as stack
    for t in tokens:
        if t in "+-x*/":  # t is an operator symbol
            S.append(t)  # push the operator symbol
        elif t not in "()":  # consider t to be a literal
            S.append(ExpressionTreeV2(t))  # push trivial tree storing value
        elif t == ")":  # compose a new tree from three constituent parts
            right = S.pop()  # right subtree as per LIFO
            op = S.pop()  # operator symbol
            left = S.pop()  # left subtree
            S.append(ExpressionTreeV2(op, left, right))  # repush tree
        # we ignore a left parenthesis
    return S.pop()


def main():
    # ExpressionTreeV2 which runs 5+3*5-(10*4)/2
    big = build_expression_tree(tokenize("5+3*5-(10*4)/2"))
    print(f"{big} = {big.evaluate()}")


if __name__ == "__main__":
    main()
