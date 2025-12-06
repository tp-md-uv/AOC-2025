# 🎄 Advent of Code 2024 🎄

My code for [Advent of Code 2024](https://adventofcode.com/2025), written in C++.

---

## Progress Overview

| AoC Puzzle | Part One | Part Two | Type (Part 1) | Type (Part 2) | Difficulty (Part 1) | Difficulty (Part 2) |
|------------|--------------------|--------------------|---------------|---------------|--------------------|--------------------|
| [Day 1: Secret Entrance](01/) | 1.1 ✅ | 1.2 ✅ | Modulo | Count + modulo | 🟢 Easy | 🟢 Easy |
| [Day 2: Gift Shop](02/) | 2.1 ✅ | 2.2 ✅ | Substring magic | Same | 🟢 Easy | 🟢 Easy |
| [Day 3: Mull It Over](03/) | 3.1 ✅ | 3.2 ✅ | Regular expressions | Added logic statements | 🟢 Easy | 🟢 Easy |
| [Day 4: Ceres Search](04/) | 4.1 ✅ | 4.2 ✅ | Words in a grid, in a line | Diagonal/circular word search | 🟢 Easy | 🟢 Easy |
| [Day 5: Print Queue](05/) | 5.1 ✅ | 5.2 ✅ | Ordering numbers | Different ordering | 🟢 Easy | 🟢 Easy |
| [Day 6: Guard Gallivant](06/) | 6.1 ✅ | 6.2 ✅ | Grid simulation | Storing path and direction (or brute force) | 🟢 Easy | 🟢 Easy |
| [Day 7: Bridge Repair](07/) | 7.1 ✅ | 7.2 ✅ | Recursion | Added rules | 🟡 Medium | 🟡 Medium |
| [Day 8: Resonant Collinearity](08/) | 8.1 ✅ | 8.2 ✅ | Grid string search | Adaptation to 8.1 | 🟢 Easy | 🟢 Easy |
| [Day 9: Disk Fragmenter](09/) | 9.1 ✅ | 9.2 ✅ | String manipulation | Memory allocation | 🟢 Easy | 🟢 Easy |
| [Day 10: Hoof It](10/) | 10.1 ✅ | 10.2 ✅ | Neighbouring grid values | Counting paths | 🟢 Easy | 🟢 Easy |
| [Day 11: Plutonian Pebbles](11/) | 11.1 ✅ | 11.2 ✅ | Recursion | Recursion | 🟢 Easy | 🟡 Medium |
| [Day 12: Garden Groups](12/) | 12.1 ✅ | 12.2 ✅ | Regions in grid | Borders of regions | 🟡 Medium | 🟡 Medium |
| [Day 13: Claw Contraption](day13/) | 13.1 ✅ | 13.2 ✅ | Linear algebra | Linear algebra | 🟢 Easy | 🟢 Easy |
| [Day 14: Restroom Redoubt](day14/) | 14.1 ✅ | 14.2 ✅ | Simulation of steps in a grid | Visual inspection | 🟡 Medium | 🟡 Medium |
| [Day 15: Warehouse Woes](day15/) | 15.1 ✅ | 15.2 ✅ | Moving objects in a grid | More advanced part 1 | 🟢 Easy | 🟡 Medium |
| [Day 16: Reindeer Maze](day16/) | 16.1 ✅ | 16.2 ✅❌ | Dijkstra with a twist | Unique paths from Dijkstra | 🔴 Hard  | 🔴 Hard ❌ |
| [Day 17: Chronospatial Computer](day17/) | 17.1 ✅ | 17.2 ✅❌ | Logic operations | Input hacking | 🟢 Easy | 🔴 Hard ❌ |
| [Day 18: RAM Run](day18/) | 18.1 ✅ | 18.2 ✅ | BFS | Checking when no path in a grid | 🟢 Easy | 🟢 Easy |
| [Day 19: Linen Layout](day19/) | 19.1 ✅ | 19.2 ✅❌ | Recursion | Recursion and keeping track of paths | 🔴 Hard | 🔴 Hard ❌ |
| [Day 20: Race Condition](day20/) | 20.1 ✅ | 20.2 ✅ | BFS/Dijkstra | Changing grid/possible paths to reach end | 🟡 Medium | 🟡 Medium |
| [Day 21: Keypad Conundrum](day21/) | 21.1 ✅ | 21.2 ✅❌ | Brute force | Recursion | 🟡 Medium | 🔴 Hard ❌ |
| [Day 22: Monkey Market](day22/) | 22.1 ✅ | 22.2 ✅ | Looping over data and following rules | Extracting as much info in one loop | 🟡 Medium | 🟡 Medium |
| [Day 23: LAN Party](day23/) | 23.1 ✅ | 23.2 ✅ | Graphs, connected components | Largest connected components | 🟢 Easy | 🟡 Medium |
| [Day 24: Crossed Wires](day24/) | 24.1 ✅ | 24.2 ✅❌ | Following rules, looping over data | Ripple carry adder | 🟢 Easy | 🔴 Hard ❌ |
| [Day 25: Code Chronicle](day25/) | 25.1 ✅ | 25.2 ✅ | Grid parsing | N.A. | 🟢 Easy | N.A. |

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

## Notes for Future Years

### **Things I would do differently:**
- When modifying values in a grid, build in **checks for elements that shouldn't be changed**.  
  - Example: Count the number of elements that should remain the same before and after the simulation.  
  - This would have saved me a lot of time in **Day 15 Part 2**!  
- When parsing data into a grid, **use `map()` and `list()`** to make the code **more concise** while maintaining **readability**.

### **Tips for next years:**
- **Practice recursion problems**.  
- **Rewrite** difficult problems after seeing **smarter/easier** solution methods.  
- **Debug visually** when working with grids (print intermediate steps!).  

---