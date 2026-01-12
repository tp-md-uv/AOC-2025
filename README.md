# 🎄 Advent of Code 2025 🎄

My code for [Advent of Code 2025](https://adventofcode.com/2025), written in C++.

---

## Progress Overview

| AoC Puzzle | Part One | Part Two | Type (Part 1) | Type (Part 2) | Difficulty (Part 1) | Difficulty (Part 2) |
|------------|--------------------|--------------------|---------------|---------------|--------------------|--------------------|
| [Day 1: Secret Entrance](01/) | 1.1 ✅ | 1.2 ✅ | Modulo | Count + modulo | 🟢 | 🟢 |
| [Day 2: Gift Shop](02/) | 2.1 ✅ | 2.2 ✅ | Patterns in substring | Slightly more difficult patterns in substring | 🟢 | 🟢 |
| [Day 3: Lobby](03/) | 3.1 ✅ | 3.2 ✅ | Search max elements in list | Search larger numbers in a list | 🟢 | 🟡 |
| [Day 4: Printing Department](04/) | 4.1 ✅ | 4.2 ✅ | Checking grid elements | Changing grid based on condition | 🟢 | 🟢 |
| [Day 5: Cafeteria](05/) | 5.1 ✅ | 5.2 ✅ | Check element in range | Merging ranges | 🟢 | 🟢 |
| [Day 6: Trash Compactor](06/) | 6.1 ✅ | 6.2 ✅ | Read out calculation from input | Parsing input in a smart way | 🟢 | 🟡 |
| [Day 7: Laboratories](07/) | 7.1 ✅ | 7.2 ✅ | Loop in grid | Hidden Pascal's triangle | 🟢 | 🟡 |
| [Day 8: Playground](08/) | 8.1 ✅ | 8.2 ✅ | Grid string search | Adaptation to 8.1 | 🟢 | 🟢 |
| [Day 9: Movie Theater](09/) | 9.1 ✅ | 9.2 ❌ | Computing area | Checking if a point is in a polygon | 🟢 | 🔴 |
| [Day 10: Factory](10/) | 10.1 ✅ | 10.2 ❌ | Brute forcing options | ILP solver | 🟡 | 🟡 |
| [Day 11: Reactor](11/) | 11.1 ✅ | 11.2 ❌ | Recursion | DFS + memoization | 🟢 | 🔴 |
| [Day 12: Christmas Tree Farm](12/) | 12.1 ✅  | 12.2 ✅ | Backtracking | N.A. | 🟡  | N.A |
---

## ⭐ Legend

| Symbol | Meaning |
|--------|---------|
| ✅ | Solved it myself |
| ❌ | Wasn't able to solve it myself |
| 🟢 | Easy |
| 🟡 | Medium |
| 🔴 | Hard |

---

## Building and Running

### Prerequisites
- C++ compiler (g++)
- Make

### Quick Start

From the `2025/` directory, run the bash script:

```bash
# Run all solutions (default - no arguments needed!)
./run_all.sh

# Run all Part 1 solutions
./run_all.sh part1

# Run all Part 2 solutions
./run_all.sh part2

# Run specific days (e.g., days 3-7)
./run_all.sh range 3 7

# Benchmark all solutions with timing
./run_all.sh benchmark
```

### Individual Days

Navigate to any day's folder (e.g., `01/`) and use its Makefile:

```bash
cd 01/

# Build and run Part 1
make run1

# Build and run Part 2
make run2

# Build both parts
make all

# Clean compiled files
make clean
```

---

## ⏱️ Performance Summary

| Day | Part 1 (ms) | Part 2 (ms) | Total (ms) |
|-----|-------------|-------------|------------|
| 01  | 12          | 12          | 24         |
| 02  | 233         | 991         | 1224       |
| 03  | 12          | 14          | 26         |
| 04  | 17          | 194         | 211        |
| 05  | 14          | 12          | 26         |
| 06  | 12          | 18          | 30         |
| 07  | 12          | 16          | 28         |
| 08  | 938         | 939         | 1877       |
| 09  | 14          | 696050      | 696064     |
| 10  | 74          | 1256        | 1330       |
| 11  | 291         | 19          | 310        |
| 12  | 20          | 13          | 33         |
| **TOTAL** | **1649** | **699534** | **701183** |
| **AVG** | **137** | **58294** | **58431** |