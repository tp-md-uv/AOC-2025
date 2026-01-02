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

There are 8 total paths from `svr` to `out`:

1. `svr,aaa,fft,ccc,ddd,hub,fff,ggg,out` - visits `fft` only ❌
2. `svr,aaa,fft,ccc,ddd,hub,fff,hhh,out` - visits `fft` only ❌
3. **`svr,aaa,fft,ccc,eee,dac,fff,ggg,out`** - visits both `fft` and `dac` ✓
4. **`svr,aaa,fft,ccc,eee,dac,fff,hhh,out`** - visits both `fft` and `dac` ✓
5. `svr,bbb,tty,ccc,ddd,hub,fff,ggg,out` - visits neither ❌
6. `svr,bbb,tty,ccc,ddd,hub,fff,hhh,out` - visits neither ❌
7. `svr,bbb,tty,ccc,eee,dac,fff,ggg,out` - visits `dac` only ❌
8. `svr,bbb,tty,ccc,eee,dac,fff,hhh,out` - visits `dac` only ❌

Only 2 paths are valid (paths 3 and 4). Both must go through `aaa` → `fft` to visit the first checkpoint, and then through `eee` → `dac` to visit the second checkpoint.

### Why Naive Recursion Fails

A naive DFS would explore every path independently, but this graph has **convergence points** where multiple paths rejoin:

- Both `svr` → `aaa` → `fft` and `svr` → `bbb` → `tty` converge at `ccc`
- Both `ccc` → `ddd` → `hub` and `ccc` → `eee` → `dac` converge at `fff`
- Both `fff` → `ggg` and `fff` → `hhh` converge at `out`

Without caching, when we reach a convergence point like `ccc` from different paths, we would recompute the subproblem "count paths from `ccc` to `out` with constraints" multiple times.

### The Caching Strategy

The key insight is that we don't need to remember the entire path history, only:

1. **Current position** in the graph
2. **Whether we've visited** `fft`
3. **Whether we've visited** `dac`

We define our cache with a three-part key: **(node, visited_fft, visited_dac)**

The value stored is: **number of valid paths from this state to `out`**

When we reach a node, we check if we've already computed the answer for this exact combination of (position, fft_visited, dac_visited). If yes, we return the cached result. If no, we compute it recursively and store it.

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

From `ccc` we have two precursion starting from `svr` with both flags false (haven't visited `fft` or `dac` yet).

**Starting point**: `("svr", false, false)`

From `svr` we explore two branches:
- Path via `aaa` 
- Path via `bbb`

#### Following the `aaa` branch first:

**At `aaa`**: `("aaa", false, false)` → goes to `fft`

**At `fft`**: `("fft", true, false)` - now we've visited `fft`! → goes to `ccc`

**At `ccc`**: `("ccc", true, false)` - visited `fft`, not `dac` yet

From `ccc` we have two options: `ddd` or `eee`

**Branch 1 - via `ddd`**: 
- `("ddd", true, false)` → `("hub", true, false)` → `("fff", true, false)`
- At `fff` we can go to `ggg` or `hhh`, both lead to `out`
- But reaching `out` with `("out", true, false)` returns 0 (missing `dac`)
- Result: **paths 1 and 2 above are invalid**

**Cache entry**: `("fff", true, false) = 0`

**Branch 2 - via `eee`**:
- `("eee", true, false)` → goes to `dac`
- `("dac", true, true)` - now we've visited both! → goes to `fff`
- `("fff", true, true)` - both checkpoints visited!
- From `fff` → `ggg` → `out`: `("out", true, true)` returns 1 ✓
- From `fff` → `hhh` → `out`: `("out", true, true)` returns 1 ✓
- Result: **paths 3 and 4 are valid!**

**Cache entries**: 
- `("fff", true, true) = 2` (2 ways to reach `out` from here)
- `("ccc", true, false) = 0 + 2 = 2` (the answer for this state)

#### Following the `bbb` branch:

**At `bbb`**: `("bbb", false, false)` → `("tty", false, false)` → `("ccc", false, false)`

**At `ccc`**: `("ccc", false, false)` - haven't visited `fft` yet!

This is a *different cache state* than `("ccc", true, false)` we saw before.

From `ccc`, again two branches:

**Branch 1 - via `ddd`**:
- Path reaches `out` without visiting either checkpoint
- Result: **paths 5 and 6 are invalid**

**Cache entry**: `("fff", false, false) = 0`

**Branch 2 - via `eee`**:
- `("eee", false, false)` → `("dac", false, true)` - visited `dac` but not `fft`
- `("fff", false, true)` - still missing `fft`!
- Reaching `out` with only `dac` visited returns 0
- Result: **paths 7 and 8 are invalid**

**Cache entries**:
- `("fff", false, true) = 0`
- `("ccc", false, false) = 0`

**Final computation**:

Total from `svr` = paths via `aaa` + paths via `bbb` = 2 + 0 = **2 valid paths**

Notice that `fff` appears in the cache with **four different states**, each yielding different results:

- `("fff", true, true) = 2`: Both checkpoints visited → 2 ways to reach `out` (via `ggg` or `hhh`)
- `("fff", true, false) = 0`: Only `fft` visited → can't reach `dac` anymore → 0 valid paths
- `("fff", false, true) = 0`: Only `dac` visited → can't reach `fft` anymore → 0 valid paths
- `("fff", false, false) = 0`: Neither visited → can never satisfy the constraint → 0 valid paths

Similarly, `ccc` appears with different states:

- `("ccc", true, false) = 2`: Visited `fft`, can still reach `dac` via `eee` branch
- `("ccc", false, false) = 0`: Haven't visited `fft` and can't reach it anymore

If we only cached by node name (e.g., just `"fff"`), we'd incorrectly reuse the first computed value for all subsequent visits, regardless of whether we'd visited the checkpoints. This would give wrong answers