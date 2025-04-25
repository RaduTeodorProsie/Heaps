# Heap Performance Benchmark

This repository benchmarks the performance of several heap data structures across various workloads and hardware configurations. The goal is to provide insights into *why* certain heaps perform better under specific conditions, based on their underlying implementations.

**Heaps Tested (Implementations Provided):**

*   `Pairing Heap`: Node-based (`std::shared_ptr`) structure using linked children. Features O(1) `meld` and amortized O(1) `push`. `pop` involves a potentially complex multi-pass merging strategy for amortized O(log N) cost.
*   `Priority Queue`: Wrapper around `std::priority_queue`, which typically uses an optimized array-based (`std::vector`) binary heap. `meld` is simulated via small-to-large merging.
*   `Quaternary Heap`: Array-based (`std::vector`) d-ary heap with d=4. Operations involve sifting up/down within the vector. `meld` is simulated via small-to-large merging.
*   `Randomized Heap`: Node-based (`std::shared_ptr`) binary structure. Relies on a core `merge` function that uses randomization to decide merge paths. `push`, `pop`, and `meld` all utilize this `merge` operation.

## Benchmark Tests

The following tests were designed to stress different aspects of heap performance. **Unless otherwise specified, all random operations can include `push`, `pop`, and `meld` (where applicable or simulated).**

**Test 1: General Throughput (Large Scale)**
*   **Operations:** 100,000,000 random operations.
*   **Purpose:** To evaluate overall performance under high volume, stressing cache performance, memory allocation/deallocation, and the average cost of mixed operations.

**Test 2: General Throughput (Medium Scale)**
*   **Operations:** 10,000,000 random operations.
*   **Purpose:** Similar to Test 1, assessing mixed operation performance on a smaller scale.

**Test 3: Basic Operations Focus (No Meld)**
*   **Operations:** 10,000,000 random `push` and `pop` operations only.
*   **Purpose:** To isolate the performance characteristics of the core insertion and extraction algorithms, removing the influence of explicit `meld` calls.

**Test 4: Push-Heavy Workload**
*   **Operations:** 10,000,000 random operations (75% `push`, 25% other: `pop`, `meld`).
*   **Purpose:** To evaluate how efficiently each implementation handles frequent insertions, including node allocation costs (for node-based heaps) and sift-up efficiency.

**Test 5: Increasing Key Insertion Order**
*   **Operations:** 10,000,000 random operations, with pushed values increasing sequentially.
*   **Purpose:** To test behavior when insertions might hit best-case scenarios (e.g., minimal sifting) but are still mixed with random `pop`/`meld`.

**Test 6: Decreasing Key Insertion Order**
*   **Operations:** 10,000,000 random operations, with pushed values decreasing sequentially.
*   **Purpose:** To test behavior under potentially challenging insertion patterns that might lead to worst-case structural scenarios or cache performance for some implementations.

## Benchmark Results

| Test     | Scenario                  | `Pairing Heap` | `Priority Queue` | `Quaternary Heap` | `Randomized Heap` |
| :------- | :------------------------ | :------------- | :--------------- | :---------------- | :---------------- |
| **Test 1** | 100M Random Ops         | 67.7 s         | 35.58 s          | **23.27 s**       | 125.4 s           |
| **Test 2** | 10M Random Ops          | 4.874 s        | 3.082 s          | **2.25 s**        | 12.21 s           |
| **Test 3** | 10M Random (No Meld)    | 6.322 s        | 3.353 s          | **2.179 s**       | 3.27 s            |
| **Test 4** | 10M Random (75% Push)   | 11.23 s        | 5.028 s          | **3.175 s**       | 14.66 s           |
| **Test 5** | 10M Random (Increasing) | 3.966 s        | 4.041 s          | **2.204 s**       | 11.67 s           |
| **Test 6** | 10M Random (Decreasing) | 9.659 s        | 4.663 s          | **2.382 s**       | 22.79 s           |

---

*Note: All metrics collected on a Ryzen 5 7640HS CPU paired with an NVIDIA 4060 GPU (though the GPU is unlikely to be relevant for heap performance). RAM speed and configuration could also influence results.*

---

## Detailed Results Analysis (Why?)

*   **Test 1 & 2 (General Random Ops):**
    *   *`Quaternary Heap`*: Achieved the best time because its array-based (`std::vector`) implementation provides excellent cache locality (data is contiguous in memory). The 4-arity leads to a shallower tree compared to a binary heap, reducing the average number of swaps/comparisons needed during `push` (shift-up) and `pop` (shift-down). Its simulated small-to-large `meld` involves efficient vector iteration and `push` operations.
    *   *`Priority Queue`*: Performed well because it relies on the highly optimized `std::priority_queue`, which uses a cache-friendly `std::vector` internally for its binary heap. The standard library implementation likely has minimal overhead. Its simulated `meld` also benefits from the efficient underlying `push`/`pop`.
    *   *`Pairing Heap`*: Was slower due to its node-based (`std::shared_ptr`) structure, leading to pointer chasing and poor cache locality. While its `push` and `meld` have good amortized bounds, the multi-pass `pop` operation, involving potentially many merges and list manipulations, has higher constant factors and suffers from the cache effects of scattered nodes. `shared_ptr` overhead also contributes.
    *   *`Randomized Heap`*: Was slowest primarily due to its node-based (`std::shared_ptr`) nature (poor cache locality, pointer chasing, allocation overhead). Its core recursive `merge` function, used by *all* modifying operations, adds function call overhead and relies on randomization (`rng()` calls), which doesn't guarantee optimal structure and adds computational cost.

*   **Test 3 (No Meld Ops):**
    *   *`Quaternary Heap`* / *`Priority Queue`*: Remained fastest/second because their core `push`/`pop` efficiency stems from their cache-friendly array-based implementations, which are unaffected by removing `meld`.
    *   *`Randomized Heap`*: Showed significant improvement because removing explicit `meld` calls drastically reduced the total number of times its relatively expensive recursive `merge` function was invoked. Its basic `push` (one `merge`) and `pop` (one `merge` of children) are less costly in isolation than when combined with frequent explicit `meld`s.
    *   *`Pairing Heap`*: Performed slightly worse possibly because the specific mix of only `push`/`pop` operations might have led to structural states where its complex multi-pass `pop` hit less favorable scenarios more often compared to the structure resulting from a mix including `meld`.

*   **Test 4 (Push-Heavy):**
    *   *`Quaternary Heap`* / *`Priority Queue`*: Maintained their lead because their array-based `push` (sift-up) is inherently efficient due to good cache locality and contiguous memory access.
    *   *`Pairing Heap`*: Slowed down because each `push` involves node allocation (`make_shared`) and an O(1) amortized `meld`. While individually fast, the high frequency increases allocation overhead and might build structural complexity ("amortized debt") that makes occasional `pop` operations more expensive.
    *   *`Randomized Heap`*: Slowed down significantly as each `push` requires node allocation (`make_shared`) and a call to the recursive `merge` function. The high frequency highlights the overhead of both memory management and the `merge` operation itself.

*   **Test 5 (Increasing Keys):**
    *   *`Quaternary Heap`* / *`Priority Queue`*: Remained efficient because pushing increasing keys often results in best-case sift-up behavior (few or no swaps needed) in their array structures.
    *   *`Pairing Heap`*: Performed relatively well because pushing an increasing key often results in the new node becoming the root immediately via the simple O(1) `meld`. This creates a potentially simpler structure temporarily, possibly delaying the cost of the multi-pass `pop`.
    *   *`Randomized Heap`*: Showed little improvement because its performance is dominated by the overhead of the randomized `merge` algorithm and pointer chasing, which are less sensitive to input key order compared to deterministic sift-based heaps.

*   **Test 6 (Decreasing Keys):**
    *   *`Quaternary Heap`* / *`Priority Queue`*: Remained robust because their array-based sift operations handle this pattern efficiently, even if it requires more swaps during sift-up than increasing keys. Cache locality remains a major advantage.
    *   *`Pairing Heap`*: Slowed down notably because inserting decreasing keys can create degenerate structures (e.g., long chains of nodes linked via the `ptr[0]` child), which makes the multi-pass merge during `pop` less efficient and approach worst-case behavior.
    *   *`Randomized Heap`*: Performed very poorly. While randomization aims to avoid worst-case behavior from input order, this specific pattern combined with the recursive `merge` might lead to particularly bad cache performance or deeper recursion stacks due to the resulting node structure and pointer chasing.

## Conclusion

Based on these tests and implementation analysis:

1.  **`Quaternary Heap` excels due to cache-friendly array storage and shallow tree depth.** Its performance advantage stems from leveraging memory hierarchy effectively and reducing the path length for operations, even when simulating `meld`.
2.  **`Priority Queue` is strong because it uses a highly optimized standard library implementation** based on similar principles (array-based binary heap).
3.  **`Pairing Heap`'s performance varies.** Its O(1) `meld`/amortized `push` is offset by a complex `pop` and the inherent costs of node-based structures (pointer chasing, poor cache locality, allocation overhead).
4.  **`Randomized Heap` suffers from node-based overheads and the cost of its recursive, randomized `merge` operation.** While theoretically sound, the practical implementation costs (pointers, randomization, recursion) make it slower in these tests.

For general-purpose performance where cache efficiency is key, array-based heaps (`Quaternary Heap`, `Priority Queue`) demonstrate significant advantages. Node-based heaps (`Pairing Heap`, `Randomized Heap`) pay a performance penalty for their flexibility due to memory indirection and allocation overheads, though `Pairing Heap` offers a theoretically fast `meld`.

## Implementation Note: Meld Operation

For heap structures that do not natively support an efficient `meld` (merge) operation (`Priority Queue`, `Quaternary Heap` in these implementations), the benchmarks simulated this operation using the **small-to-large merging technique** whenever a `meld` operation was required by the test scenario.

This involves:
1. Identifying the smaller and the larger of the two heaps to be melded (based on `.size()` or internal vector size).
2. Iteratively extracting elements from the smaller heap (using `pop` for `PriorityQueue`, or iterating the vector for `Quaternary Heap`) and inserting them into the larger heap (using `push`).

**Complexity Consideration:** This simulation adds overhead. However, the small-to-large approach guarantees that any single element is moved (pushed into another heap during a meld) at most O(log N) times throughout the entire benchmark run (where N is the total number of elements inserted). This provides an amortized bound on the total cost of *all* simulated meld operations. Heaps with native `meld` (`Pairing Heap`, `Randomized Heap`) use their built-in merge logic directly.
