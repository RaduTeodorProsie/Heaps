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
| TEST 1 | 11.33 s    | 30.54 s  | 39.81 s     | 105.5 s   | 
| TEST 2 | 1.016 s    | 2.004 s  | 0.829 s     | 0.177 s   | 
| TEST 3 | 1.115 s    | 1.148 s  | 1.054 s     | 3.5 s     | 
| TEST 4 | 1.121 s    | 1.522 s  | 0.801 s     | 3.565 s   | 
| TEST 5 | 1.011 s    | 2.008 s  | 0.796 s     | 0.158 s   | 
| TEST 6 | 1.011 s    | 2.008 s  | 0.796 s     | 0.158 s   | 


---

### **Radix Sort (LSD)**
- **Great performance on random numbers (Test 1, 11.33s)**:  
  Thrived on 100M random numbers due to linear _O(nk)_ complexity. Fixed-width integers allow efficient digit-wise processing.  
- **Struggled in Smaller Tests**:  
  Overhead of multiple passes became noticeable compared to cache-friendly algorithms (e.g., 1.12s in TEST 4 vs. Counting Sort's 0.001s).
- **We used bitwise operations and a base of 2 ^ 16 to make everything run smoother. We also tried the 2 ^ 8 base and it performed simillarly. **

---

### **STL Sort (Introsort)**
- **TEST 1 Slowdown (30.54s)**:  
  Hybrid pivoting in Introsort (quicksort + heapsort) struggled with true randomness, unlike Radix/Parallel sorts.  
- **TEST 2/5 Issues (2.0s)**:  
  Nearly sorted/adversarial patterns trigger worst-case quicksort behavior, requiring heapsort fallback.

---

### **Shell Sort (Sedgewick Gaps)**
- **TEST 2/4 Efficiency (0.8s)**:  
  Sedgewick gaps minimized backward shifts in nearly/sorted data.  
- **TEST 1 Weakness (39.81s)**:  
  Random data nullified gap-sequence advantages, reverting to _O(n^(4/3))_ performance.
- **We implemented the SedgeWick gaps variant, which more straightforward than the Hibbard's gap one and performs simmilarly. 
---

### **Heap Sort**
- **TEST 1 Disaster (105.5s)**:  
  Poor cache locality during heapify operations crippled performance at scale.  
- **TEST 5 Surprise (0.158s)**:  
  Identical elements reduced swap operations, partially mitigating cache issues.

---

### **Merge Sort**
- **Consistent Mid-Pack**:  
  Guaranteed _O(n log n)_ avoided extremes but suffered from:  
  - Memory overhead (TEST 1: 29.61s vs. Radix's 11.33s)  
  - Fixed division strategy (TEST 4: 2.61s vs. Timsort's 1.35s).

---

### **Tim Sort**
- **TEST 1/4 Strength**:  
  Detected natural runs in random/sorted data (TEST 4: 1.35s vs. Merge Sort's 2.61s).  
- **TEST 3 Limitation (2.07s)**:  
  Reverse-sorted data forced minimum run sizes, reducing optimization benefits.

---

### **Parallel Sort**
- **TEST 1 Dominance (5.398s)**:  
  Embarrassingly parallel workload scaled near-linearly across cores.  
- **TEST 4 Peak (0.11s)**:  
  Pre-sorted data allowed trivial partitioning and parallel merging.
- **We made use of the <execution> library provided with C++ 17 for the parallelization of std::sort. There are only two lines of code extra from the standard STL sort. 

---

### **Counting Sort**
- **TEST 4/5 Supremacy (0.001s)**:  
  Small key range (sorted: 1M, identical: 1) enabled _O(n)_ magic.  
- **TEST 1/3 N/A**:  
  Large value ranges (100M random/descending numbers) made frequency counting impractical.


## Key takeaways
1. **Memory Patterns Matter**:  
   This is the main reason why heapsort has the lowest result on Test 1, and why Counting sort can achieve an incredible 0.001 seconds on sorting 10^7 (already sorted) elements.   
2. **Parallelism is great**:  
   Our clear winner is parallel sort, which is just std::sort parallelized. If you have the chance to parallelize or vectorize code, you should do it. 
3. **Adaptability**:  
   Each sorting algorithm is great for its own purpose. Try to make the algorithm fit the problem rather than make the problem fit the algorithm.   

*Note: All metrics collected on Ryzen 5 7640HS paired with an NVIDIA 4060 card.*
  


