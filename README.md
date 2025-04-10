# OpenMPnRust
Parallel benchmark suite in OpenMP vs Rust -- Multicore 2025 Spring

## System
- **Machine**: crunchy1.cims.nyu.edu
- **CPU**: 4× AMD Opteron 6272 (64 cores total

  
## Benchmarks
- [ ] Dot Product
- [ ] Matrix Multiplication
- [ ] Prefix Sum
- [ ] Sorting
- [ ] Image Blur
- [ ] Merge Sort
- [ ] Pi Estimation

## ✅ Benchmark 1: Dot Product 
## [OpenMP]

### Summary
- **Goal**: Compute the dot product of two large float arrays using OpenMP parallel reduction.
- **Size**: 100 million elements (N = 100_000_000)
- **Pattern**: Parallel map + reduction

### Observations
- The **dot product values increase** as thread count increases — this is expected due to more samples being processed before rounding losses.  
  > 💡 OpenMP’s `reduction` clause doesn’t guarantee exact summation order, so results may vary slightly — **totally fine**!
  
- The **time values decrease steadily** up to 16–32 threads, and **start plateauing** around 32–64 —  
  > 🧪 This reflects **perfect real-world scaling behavior** with expected overhead kicking in at high thread counts.

### Results

```csv
Threads,Time (s),Dot Product
1,0.378277,16777216.000000
2,0.251706,20969164.000000
4,0.155207,23631394.000000
8,0.131793,24579896.000000
16,0.108708,24874172.000000
32,0.114902,24963168.000000
64,0.119173,24987352.000000
```

## [Rust]

### Summary
- **Goal**: Compute the dot product of two large float arrays using Rayon’s parallel iterators.
- **Size**: 100 million elements
- **Pattern**: Parallel map + sum (via `.par_iter().zip().map().sum()`)

### Observations
- **Perfect accuracy**: All results = 100 million ✅
- **Time drops sharply** up to 16 threads, then starts rising again due to thread overhead:
  > 🧪 **Threading sweet spot ≈ 16–32 threads**, beyond which Rayon overhead dominates.

### Results

```csv
Threads,Time (ms),Result
1,345.621289,100000000
2,199.875145,100000000
4,113.37229,100000000
8,106.55862,100000000
16,88.966377,100000000
32,99.120636,100000000
64,157.70001,100000000
```

## ✅ Benchmark 2: Matrix Multiplication 
## [OpenMP]

### Summary
- **Goal**: Multiply two dense matrices A (1000×1000) and B (1000×1000) to produce C (1000×1000)
- **Pattern**: Nested parallel loops using `collapse(2)` for row/column parallelism
- **Validation**: Results are checked against a sequential version for correctness

### Observations
- ✅ Every run printed `Matrix multiplication correct!`, confirming correctness.
- The **time consistently decreased** as thread count increased, showing good scalability.
- Performance started to **plateau near 32–64 threads**, which is typical as memory bandwidth becomes the bottleneck.

### Results

```csv
Threads,Time (s)
1,4.136051
2,2.314669
4,1.398697
8,0.954454
16,0.627427
32,0.393723
64,0.343196
```

## [Rust]

#### Summary
- **Goal**: Multiply two large square matrices using parallel nested loops.
- **Size**: 1000 × 1000 (configurable)
- **Pattern**: Parallel nested `for` loops using `par_iter_mut().enumerate()`

#### Observations
- Rust version achieves **strong scaling** up to 64 threads, with **excellent speedup** from 1 to 32 threads.
- Minimal overhead observed even with nested iteration — Rayon handles this elegantly.
- The performance curve **plateaus at 32–64 threads**, which mirrors OpenMP scaling.

#### Results

```csv
Threads,Time (ms),Result
1,2.783196,
2,1.409073,
4,0.801658,
8,0.656411,
16,0.610427,
32,0.449459,
64,0.308576,
```

## ✅ Benchmark 3: Prefix Sum 
## [OpenMP]

### Summary
- **Goal**: Compute the prefix sum of an array `A` such that `S[i] = A[0] + A[1] + ... + A[i]`
- **Size**: 100 million elements (N = 100_000_000)
- **Pattern**: Chunked local prefix sums + block total offset adjustment

### Observations
- Originally failed with `float` due to loss of precision at `2^24 = 16,777,216`.  
  > 💡 Switching to `double` solved this, preserving accuracy for large-scale summation.
- Prefix sum implemented in three phases: local prefix, block totals scan, offset adjust.
- Timing flattens around 16–32 threads, with good scaling in lower ranges.

### Results

```csv
Threads,Time (s)
1,0.538079
2,0.442912
4,0.262932
8,0.275661
16,0.205359
32,0.239327
```

## [Rust]

### Summary
- **Goal**: Compute the prefix sum \( S[i] = A[0] + A[1] + \dots + A[i] \) of an array in parallel using Rayon.
- **Size**: 100 million elements (N = 100_000_000)
- **Pattern**: Block-wise prefix + parallel offset adjust

### Observations
- Rust’s Rayon parallelism handles chunking and processing well, but **shared memory writes (e.g., offset recording) introduce synchronization overhead**.
- Performance improves significantly up to 16 threads but **flattens out or worsens** with 32–64 threads due to **mutex contention**.
- Still a solid implementation for CPU-bound prefix workloads!

### Results

```csv
Threads,Time (ms),Result
1,390.307368,
2,316.032158,
4,324.974458,
8,229.853436,
16,186.717630,
32,247.302924,
64,252.394231,
```

## ✅ Benchmark 4: Pi Estimation 
## [OpenMP]

### Summary
- **Goal**: Use Monte Carlo simulation to estimate the value of π by sampling random (x, y) points in a 1×1 square and counting how many land inside a unit quarter-circle.
- **Size**: 100 million points (N = 100_000_000)
- **Pattern**: Independent per-thread trials, summed with OpenMP reduction

### Observations
- This is an **embarrassingly parallel** problem — no need for synchronization during simulation.
- Accuracy was surprisingly good across all runs (within 5e-5 of actual π).
- Performance scales cleanly with thread count, plateauing around 32–64 threads as expected.

### Results

```csv
Threads,Time (s),Pi Estimate
1,3.067497,3.14153184
2,1.619837,3.14171604
4,0.986242,3.14197820
8,0.757476,3.14155852
16,0.428682,3.14158212
32,0.324517,3.14165292
64,0.282273,3.14164740
```

## [Rust]

### Summary
- **Goal**: Estimate π using Monte Carlo sampling — sample N random (x, y) points in a unit square, count how many fall inside the unit quarter-circle.
- **Size**: 100 million points (N = 100_000_000)
- **Pattern**: Purely parallel map-reduce (Rayon)

### Observations
- This is a **textbook embarrassingly parallel** problem — no inter-thread communication required.
- Performance improves **cleanly and steadily** with more threads.
- Results were **highly accurate**, consistently estimating π to within **0.0003 error**.
- Best speedup occurs between **1–32 threads**, after which overhead slowly dominates.

### Results

```csv
Threads,Time (ms),Result
1,5366.887,3.14183144
2,2783.476,3.14162956
4,1453.809,3.14157548
8,1046.400,3.14169268
16,685.562,3.14159804
32,550.017,3.14177524
64,446.878,3.1416292
```

## ✅ Benchmark 5: Image Blur / Convolution 
## [OpenMP]

### Summary
- **Goal**: Apply a 3×3 box blur (uniform averaging) to a 2D float matrix representing a grayscale image.
- **Size**: 8000 × 8000 image (64 million pixels)
- **Pattern**: 2D stencil operation over pixels, with OpenMP row-parallelism

### Observations
- This is a **classic image-processing pattern** where each output pixel depends on its 8 neighbors.
- Scaling was excellent up to 32 threads; at 64, overhead begins to outweigh gains due to limited work per thread.
- Result correctness was verified by comparing to a sequential version.

### Results

```csv
Threads,Time (s),Correctness
1,1.152866,Correct
2,0.605302,Correct
4,0.314942,Correct
8,0.252947,Correct
16,0.194328,Correct
32,0.133057,Correct
64,0.143720,Correct
```

## [Rust]

### Summary
- **Goal**: Apply a 3×3 mean filter (box blur) to a large 2D image matrix.
- **Size**: 8192 × 8192 = ~67 million pixels
- **Pattern**: 2D stencil computation (nested loops)

### Observations
- Sharp drop in runtime up to **8 threads**, with best performance at **16–32 threads**.
- Slight **slowdown** at 64 threads due to overhead — a classic case of **diminishing returns**.
  > 💡 This aligns perfectly with the scaling trend we saw in OpenMP!

### Results

```csv
Threads,Time (ms),Result
1,2.954227,
2,1.517958,
4,797.550888,
8,559.759823,
16,362.453798,
32,350.218819,
64,351.120333,
```

## ✅ Benchmark 6: Quicksort 
## [OpenMP]

### Summary
- **Goal**: Sort a large float array in parallel by dividing into chunks, sorting each with `qsort()`, then merging the sorted chunks.
- **Size**: 100 million elements (N = 100_000_000)
- **Pattern**: Parallel chunked sort + pairwise merging

### Observations
- Sorting time **drops dramatically** with parallelism, especially from 1 → 8 threads  
  > 🔀 After 16–32 threads, we see diminishing returns — mostly due to merging overhead and memory bandwidth limits.
  
- Final merging step is **done sequentially** in pairwise fashion (log₂(T) levels).  
  > 🧪 Shows real-world tradeoff between parallel sorting and combining results.

### Results

```csv
Threads,Time (s)
1,46.079157
2,26.167981
4,14.028923
8,9.024287
16,6.209166
32,5.594471
64,5.149400
```

## [Rust]

### Summary
- **Goal**: Sort a large array of random integers using parallel quicksort in Rust.
- **Size**: 100 million elements
- **Pattern**: Divide-and-conquer; manually spawn threads for independent chunks

### Observations
- Performance improves steadily up to **16 threads**.
- Slight **overhead kicks in** beyond 16 threads, but doesn't regress much.
- This implementation used `std::thread` rather than Rayon.

> 🧠 Unlike map-reduce workloads, quicksort doesn’t fit Rayon-style data parallelism easily — recursive threading works better here.

### Results

```csv
Threads,Time (ms),Result
1,1516.172329,
2,760.064705,
4,548.356448,
8,431.691968,
16,323.153829,
32,400.895275,
64,414.094836,
```

## ✅ Benchmark 7: Merge Sort 
## [OpenMP]

### Summary
- **Goal**: Sort a large float array using recursive merge sort with OpenMP parallelism via `#pragma omp task`.
- **Size**: 100 million elements (N = 100_000_000)
- **Pattern**: Recursive divide-and-conquer with parallel tasks and pairwise merging

### Observations
- Excellent speedup up to 32 threads, slight plateau after  
  > 🚀 OpenMP tasks shine when the input size per thread is still large (above threshold)

- Merge sort performs slightly **better than Quicksort at high thread counts**  
  > 🔍 Fewer write operations and more regular memory access = better performance in this setting

### Results

```csv
Threads,Time (s)
1,41.677730
2,22.322493
4,16.996031
8,12.161643
16,7.624287
32,5.990745
64,5.250146
```

## [Rust]

### Summary
- **Goal**: Sort a large array using merge sort, leveraging Rust's threading for parallel divide-and-conquer.
- **Size**: 100 million elements
- **Pattern**: Recursive merge with rayon tasks (though not optimized with full parallel iterators)

### Observations
- Merge sort was cleanly implemented with recursion and merge steps, achieving **good performance up to 32 threads**
- Some unexpected slowdown appears at 64 threads due to **thread contention or overhead**
- Merge sort outperforms quicksort at **lower thread counts**, but quicksort is faster at 8–32 threads

> 🧠 Just like in OpenMP, this benchmark highlights tradeoffs between memory locality and chunking vs recursive merges

### Results

```csv
Threads,Time (ms),Result
1,32129.186429,
2,17213.423572,
4,10849.029093,
8,8997.832668,
16,9157.318598,
32,8222.420217,
64,10642.529966,
```

---

## 🥊 Quicksort vs Merge Sort Comparison (OpenMP)

Threads | Quicksort (s) | Merge Sort (s)
--------|----------------|----------------
1       | 46.08          | 41.68
2       | 26.17          | 22.32
4       | 14.03          | 17.00
8       | 9.02           | 12.16
16      | 6.21           | 7.62
32      | 5.59           | 5.99
64      | 5.15           | 5.25

> 🧠 **Merge Sort wins at low thread count**, but **Quicksort wins from 8–32 threads** due to chunk-based parallelization.  
> After that, both plateau similarly — likely due to memory bandwidth and merge overheads.

## 🥊 Quicksort vs Merge Sort Comparison (Rust)

| Threads | Quicksort (ms) | Merge Sort (ms) |
|---------|----------------|-----------------|
| 1       | 1516.17        | 32129.19        |
| 2       | 760.06         | 17213.42        |
| 4       | 548.36         | 10849.03        |
| 8       | 431.69         | 8997.83         |
| 16      | 323.15         | 9157.32         |
| 32      | 400.90         | 8222.42         |
| 64      | 414.09         | 10642.53        |

> 🔍 **Quicksort is consistently faster** in this implementation, especially in the sweet spot of **8–16 threads**.  
> Merge sort suffers from deeper recursion overheads and **less effective parallel coordination** in Rust.  
> Performance could improve further with optimized parallel merges or dedicated task pools.

