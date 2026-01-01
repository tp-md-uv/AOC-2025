# Day 10

## Part 1
Observe that you will press a button only zero or one time. Then you can get every combination of buttons and brute force the solution.

## Part 2

The extension of this problem is that we now care about the number of times we press a button. Whereas in part 1, we could use the nice property that you will only press a button once, since pressing it twice 'cancels' out the effect. Therefore you would press a button once, or not at all.

In part 2 however, this is a little different. Now we do actually care about the number of button presses. Since pressing a single button more than once is a valid option, brute forcing suddenly becomes infeasible.

But we can rewrite it as an optimization problem. Let's take the first example:

```
(3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
```

Denote with $J_i$ and $b_i$, $1 \leq i \leq 6$, the required joltage level in position $i$ and number of times button $i$ was pressed, respectively.

The effect on joltage 0 only comes from buttons 5 and 6, that would be:

$$b_5 + b_6 = J_1$$

The second joltage level is only affected by buttons 2 and 6:

$$b_2 + b_6 = J_2$$

Writing out all of these equations will result in the linear system:

$$
\begin{align}
0 \cdot b_1 + 0 \cdot b_2 + 0 \cdot b_3 + 0 \cdot b_4 + b_5 + b_6 &= 3 \\
0 \cdot b_1 + b_2 + 0 \cdot b_3 + 0 \cdot b_4 + 0 \cdot b_5 + b_6 &= 5 \\
0 \cdot b_1 + 0 \cdot b_2 + b_3 + 0 \cdot b_4 + b_5 + 0 \cdot b_6 &= 4 \\
b_1 + b_2 + 0 \cdot b_3 + b_4 + 0 \cdot b_5 + 0 \cdot b_6 &= 7
\end{align}
$$

Written differently:

$$
\begin{bmatrix}
0 & 0 & 0 & 0 & 1 & 1 \\
0 & 1 & 0 & 0 & 0 & 1 \\
0 & 0 & 1 & 0 & 1 & 0 \\
1 & 1 & 0 & 1 & 0 & 0
\end{bmatrix}
\begin{bmatrix}
b_1 \\ b_2 \\ b_3 \\ b_4 \\ b_5 \\ b_6
\end{bmatrix}
=
\begin{bmatrix}
3 \\ 5 \\ 4 \\ 7
\end{bmatrix}
$$

Or more compactly: $A\mathbf{b} = \mathbf{J}$

If the goal was to just solve this system of linear equations, the problem would not be that difficult. However, there are more constraints:

1. We cannot press the button a negative number of times, and only an integer number of times, meaning $\mathbf{b} \in \mathbb{N}^6$.
2. We are looking for the solution that minimizes the total number of button presses: $\min_{\mathbf{b} \in \mathbb{N}^6} \sum_{i=1}^6 b_i$.

So we arrive at an Integer Linear Program (ILP):

$$
\begin{align}
\min_{\mathbf{b} \in \mathbb{N}^6} \quad & \sum_{i=1}^6 b_i \\
\text{s.t.} \quad & A\mathbf{b} = \mathbf{J}
\end{align}
$$

Since ILPs are NP-hard, there is no clean heuristic I am going to implement myself here. A good library to solve such problems is [z3](https://github.com/Z3Prover/z3)


