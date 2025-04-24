# This is a repo meant to benchmark different types of heaps on different tests and hardware. 

**test1**
  100.000.000 random operations

**test2**
  10.000.000 random operations

**test3**
  10.000.000 random operations without the meld operation

**test4**
  10.000.000 random operations
  75% of the operations are push operations

**test5**
  10.000.000 random operations
  the pushed values are increasing

**test6**
  10.000.000 random operations
  the pushed values are decreasing
    


|          | Pairing Heap | Priority Queue | Quaternary Heap | Randomized Heap | 
|----------|------------|----------|-------------|-----------|
| TEST 1 | 67.7 s    | 35.58  | 23.27 s     | 125.4 s   | 
| TEST 2 | 4.874 s    | 3.082 s  | 2.25 s     | 12.21 s   | 
| TEST 3 | 6.322 s    | 3.353 s  | 2.179 s     | 3.27 s     | 
| TEST 4 | 11.23 s    | 5.028 s  | 3.175 s     | 14.66 s   | 
| TEST 5 | 3.966 s    | 4.041 s  | 2.204 s     | 11.67 s   | 
| TEST 6 | 9.659 s    | 4.663 s  | 2.382 s     | 22.79 s   | 

---

*Note: All metrics collected on Ryzen 5 7640HS paired with an NVIDIA 4060 card.*



For small and large tests, the Quaternary Heap is the fastest option. The Priority Queue also works well and is a good second choice. The Randomized Heap and the Pairing Heap are slower.

