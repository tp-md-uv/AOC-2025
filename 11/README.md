# Day 11

## Part 1
Standard recursion to count all paths from a source node to a destination node in a directed acyclic graph (DAG).

## Part 2

### Problem Extension

In Part 1, we counted all paths from source to destination. Part 2 adds a critical constraint: we must count only paths that visit both special checkpoint nodes `fft` and `dac` (in any order) before reaching the destination `out`.

This transforms the problem from simple path counting to a **constrained path counting problem with state tracking**.

### The Example

Consider the graph defined as:

```
svr: aaa bbb
aaa: fft
fft: ccc
bbb: tty
tty: ccc
ccc: ddd eee
ddd: hub
hub: fff
eee: dac
dac: fff
fff: ggg hhh
ggg: out
hhh: out
```

There are 8 total paths from `svr` to `out`, but only 2 paths visit both `dac` and `fft`:

1. `svr,aaa,fft,ccc,eee,dac,fff,ggg,out`
2. `svr,aaa,fft,ccc,eee,dac,fff,hhh,out`

Both valid paths must go through `aaa` → `fft` (to visit the `fft` checkpoint) and then `eee` → `dac` (to visit the `dac` checkpoint).

### Why Naive Recursion Fails

A naive DFS would explore every path independently, but this graph has **convergence points** where multiple paths rejoin:

- Both `svr` → `aaa` → `fft` and `svr` → `bbb` → `tty` converge at `ccc`
- Both `ccc` → `ddd` → `hub` and `ccc` → `eee` → `dac` converge at `fff`
- Both `fff` → `ggg` and `fff` → `hhh` converge at `out`

Without caching, when we reach a convergence point like `ccc` from different paths, we would recompute the subproblem "count paths from `ccc` to `out` with constraints" multiple times.

### State Space Formulation

The key insight is that we don't need to remember the entire path history, only:

1. **Current position** in the graph
2. **Whether we've visited** `fft`
3. **Whether we've visited** `dac`

Define the state as:

$$\text{State} = (v, F, D) \in V \times \{0, 1\} \times \{0, 1\}$$

where:
- $v \in V$ is the current node
- $F \in \{0, 1\}$ indicates whether `fft` has been visited
- $D \in \{0, 1\}$ indicates whether `dac` has been visited

The dynamic programming value function is:

$$P(v, F, D) = \text{number of valid paths from node } v \text{ to } \texttt{out}$$

with the constraint that we must visit both checkpoints.

### Recurrence Relation

The recursive structure is:

$$
P(v, F, D) = \begin{cases}
1 & \text{if } v = \texttt{out} \text{ and } F = 1 \text{ and } D = 1 \\
0 & \text{if } v = \texttt{out} \text{ and } (F = 0 \text{ or } D = 0) \\
\sum_{u \in N(v)} P(u, F', D') & \text{otherwise}
\end{cases}
$$

where $N(v)$ are the neighbors of $v$, and:

$$F' = F \lor (v = \texttt{fft})$$
$$D' = D \lor (v = \texttt{dac})$$

### Cache Walkthrough

Let's trace through the computation starting from `svr` with $F = 0, D = 0$:

**Step 1**: Compute $P(\texttt{svr}, 0, 0)$

The algorithm explores both branches from `svr`:
- Branch 1: `svr` → `aaa` 
- Branch 2: `svr` → `bbb`

$$P(\texttt{svr}, 0, 0) = P(\texttt{aaa}, 0, 0) + P(\texttt{bbb}, 0, 0)$$

**Step 2**: Compute $P(\texttt{aaa}, 0, 0)$

From `aaa` we go to `fft`, which sets $F = 1$:

$$P(\texttt{aaa}, 0, 0) = P(\texttt{fft}, 1, 0)$$

**Step 3**: Compute $P(\texttt{fft}, 1, 0)$

Since we're at `fft` and $F$ is already 1:

$$P(\texttt{fft}, 1, 0) = P(\texttt{ccc}, 1, 0)$$

**Cache entry created**: `("fft", 1, 0)` computed once

**Step 4**: Compute $P(\texttt{ccc}, 1, 0)$

From `ccc` we have two paths:

$$P(\texttt{ccc}, 1, 0) = P(\texttt{ddd}, 1, 0) + P(\texttt{eee}, 1, 0)$$

**Cache entry created**: `("ccc", 1, 0)` stores this result

**Step 5**: Compute $P(\texttt{ddd}, 1, 0)$ → $P(\texttt{hub}, 1, 0)$ → $P(\texttt{fff}, 1, 0)$

Following the chain `ddd` → `hub` → `fff`, we eventually reach:

$$P(\texttt{fff}, 1, 0) = P(\texttt{ggg}, 1, 0) + P(\texttt{hhh}, 1, 0)$$

Both branches lead to `out` without having visited `dac` ($D = 0$), so:

$$P(\texttt{ggg}, 1, 0) = P(\texttt{out}, 1, 0) = 0$$
$$P(\texttt{hhh}, 1, 0) = P(\texttt{out}, 1, 0) = 0$$

Therefore: $P(\texttt{fff}, 1, 0) = 0$

**Cache entry created**: `("fff", 1, 0) = 0` (visited `fft` but not `dac`)

**Step 6**: Compute $P(\texttt{eee}, 1, 0)$

From `eee` we go to `dac`, which sets $D = 1$:

$$P(\texttt{eee}, 1, 0) = P(\texttt{dac}, 1, 1)$$

**Step 7**: Compute $P(\texttt{dac}, 1, 1)$ → $P(\texttt{fff}, 1, 1)$

Now we reach `fff` with both checkpoints visited:

$$P(\texttt{fff}, 1, 1) = P(\texttt{ggg}, 1, 1) + P(\texttt{hhh}, 1, 1)$$
$$P(\texttt{ggg}, 1, 1) = P(\texttt{out}, 1, 1) = 1$$
$$P(\texttt{hhh}, 1, 1) = P(\texttt{out}, 1, 1) = 1$$

Therefore: $P(\texttt{fff}, 1, 1) = 2$

**Cache entry created**: `("fff", 1, 1) = 2` (both checkpoints visited, 2 ways to reach `out`)

**Step 8**: Back to $P(\texttt{ccc}, 1, 0)$

$$P(\texttt{ccc}, 1, 0) = P(\texttt{ddd}, 1, 0) + P(\texttt{eee}, 1, 0) = 0 + 2 = 2$$

**Step 9**: Compute $P(\texttt{bbb}, 0, 0)$

Now the second branch from `svr`:

$$P(\texttt{bbb}, 0, 0) = P(\texttt{tty}, 0, 0) = P(\texttt{ccc}, 0, 0)$$

**Step 10**: Compute $P(\texttt{ccc}, 0, 0)$

Notice we're at `ccc` again, but now with $F = 0, D = 0$ (different state than before):

$$P(\texttt{ccc}, 0, 0) = P(\texttt{ddd}, 0, 0) + P(\texttt{eee}, 0, 0)$$

Following the `ddd` → `hub` → `fff` path with $F = 0, D = 0$ leads to:

$$P(\texttt{fff}, 0, 0) = 0$$

**Cache entry created**: `("fff", 0, 0) = 0` (neither checkpoint visited)

Following the `eee` → `dac` path with $F = 0, D = 0$ leads to:

$$P(\texttt{dac}, 0, 1) = P(\texttt{fff}, 0, 1)$$
$$P(\texttt{fff}, 0, 1) = 0$$

**Cache entry created**: `("fff", 0, 1) = 0` (visited `dac` but not `fft`)

Therefore: $P(\texttt{ccc}, 0, 0) = 0$

**Final Answer**:

$$P(\texttt{svr}, 0, 0) = P(\texttt{aaa}, 0, 0) + P(\texttt{bbb}, 0, 0) = 2 + 0 = 2$$

### Key Cache Entries

The cache stores distinct states:

- `("ccc", 1, 0) = 2`: Reached `ccc` after visiting `fft`, can still visit `dac` → 2 valid paths
- `("ccc", 0, 0) = 0`: Reached `ccc` without visiting `fft`, cannot visit it anymore → 0 valid paths  
- `("fff", 1, 1) = 2`: Reached `fff` with both checkpoints visited → 2 ways to reach `out`
- `("fff", 1, 0) = 0`: Reached `fff` with only `fft` visited → 0 valid paths (can't reach `dac`)
- `("fff", 0, 1) = 0`: Reached `fff` with only `dac` visited → 0 valid paths (can't reach `fft`)
- `("fff", 0, 0) = 0`: Reached `fff` with neither visited → 0 valid paths

Notice that `fff` appears in the cache with four different states, each yielding different results. This demonstrates why the cache key must be the **tuple of (node, visited_fft, visited_dac)** rather than just the node.

### Complexity Analysis

**Without caching**: The algorithm would explore all 8 paths independently, recomputing subproblems at every convergence point. For larger graphs, this becomes exponential in the number of paths.

**With caching**: The state space has size $|V| \times 2 \times 2 = 4|V|$, where $|V|$ is the number of nodes. Each state is computed at most once, giving us $O(4|V|)$ complexity, effectively linear in the graph size.

For the actual problem input with hundreds of nodes and potentially millions of paths, caching is essential.

