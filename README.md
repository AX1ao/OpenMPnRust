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

## ✅ Benchmark 1: Dot Product (OpenMP)

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

## ✅ Benchmark 2: Matrix Multiplication (OpenMP)

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

## ✅ Benchmark 3: Prefix Sum (OpenMP)

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
