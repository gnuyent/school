# Algorithms I

## Fundamentals

### Algorithm Analysis

Program runtime is determined by the *cost of executing each statement* and the *frequency of execution of each statement*.

**Tilde approximations** are where low-order terms are ignored. `~ f(N)` represents any function, when divided by `f(N)` approaches 1 as N grows.

**Order-of-growth classifications** define how a function grows over time. These are commonly described as constant, logarithmic, linear, etc. growth.

| description  | order of growth |
| ------------ | --------------- |
| constant     | $1$             |
| logarithmic  | $\log N$        |
| linear       | $N$             |
| linearithmic | $N \log N$      |
| quadratic    | $N^2$           |
| cubic        | $N^3$           |
| exponential  | $2^N$           |



### Union-Find

#### Dynamic connectivity

Two queries:

1. Union command - Connect two objects
2. Find/connected query - Is there a path connecting two objects?

"Is connected" is an equivalence relation. It is 

* reflexive (p is connected to p)
* symmetric (p is connected to q, q is therefore connected to p)
* transitive (p is connected to q, q is connected to r, p is therefore connected to r).

**Connected components** are the maximal *set* of objects that are mutually connected.

#### Efficient implementation

The efficient union-find algorithm, called *weighted quick-union with path compression*, is implemented through multiple techniques.

* The first technique is called **weighting**. Weighting within this algorithm determines which tree's depth is shortest and places it's root under the root of the one that is deepest. This brings the algorithm to $\log_2 (N)$. The depth of any node only increases when a smaller tree $T_1$ is placed under a bigger tree $T_2$. Since $T_1$ is *always* $\leq$ $T_2$, the depth of $T_2$ will *always* at least double. This means that the size of the tree can only double at most $\log_2(N)$ or $\lg(N)$ times.
* The second technique is called **path compression**. Path compression shortens the length of every node in the tree by pointing it to its root node, therefore flattening the tree. A two-pass implementation would be defined in the `root` method where every node along a branch would be set to the root. A simpler, single-pass implementation would set every other node to point to its grandparent, halving the path length.

Weighted quick-union with path compression (WQUPC) has an extremely fast worst-case time of $N + M\lg^* N$ ($\lg^*$ is the iterated logarithm - the amount of time the logarithm function must be iteratively applied before the result is $\leq$ 1).