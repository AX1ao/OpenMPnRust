# OpenMPnRust

Parallel benchmark suite in OpenMP (C/C++) vs. Rust (Rayon)  
*Multicore Programming · Spring 2025*

---

## 💻 System Specs

- **Machine**: `crunchy1.cims.nyu.edu`  
- **CPU**: 4 × AMD Opteron 6272 (64 cores total)

---

## 🧪 Benchmark Suite

This project implements and compares **9 benchmarks** in OpenMP and Rust (Rayon), covering a variety of parallel patterns:

- `dot_product`
- `matrix_mult`
- `prefix_sum`
- `quick_sort`
- `merge_sort`
- `pi_estimation`
- `image_blur`
- `overhead` (system-level: thread creation)
- `synchronization` (system-level: atomic contention)

Each benchmark was parallelized idiomatically in both languages and run across 1–64 threads.

---

## ⚙️ How to Run

See [`readme.txt`](readme.txt) for instructions on building and executing all benchmarks.

---

## 📊 Results & Analysis

- Raw timing results are stored in [`results/`](./results/) as `.csv` files.
- All visualizations, speedup/efficiency graphs, and Amdahl's Law fits are located in the [`Analysis/`](./Analysis/) directory.

**Key files include:**

- `Analysis.ipynb` – Jupyter notebook with all analysis code and figures  
- `Analysis.pdf` – Final exported version  
- `Analysis.md` – Markdown version of the report content  

We analyze:
- Raw execution time across threads
- Speedup and efficiency
- Amdahl’s Law curve fitting (parallelizable fraction)
- Thread-level win counts and summary comparisons

---

## 📌 Key Takeaways

- **OpenMP** performs better in compute-heavy and memory-bound workloads due to low thread management overhead.
- **Rust (Rayon)** is more ergonomic and excels in irregular workloads and embarrassingly parallel tasks.
- System-level benchmarks show:
  - Rust incurs higher cost for spawning threads
  - OpenMP performs slightly better under atomic contention
