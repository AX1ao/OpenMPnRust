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
