Multicore Project: Benchmarking OpenMP vs. Rust
===============================================

This project compares the performance and ergonomics of OpenMP (C/C++) and Rust (Rayon)
across 9 parallel benchmarks.

------------------------------------
COMPILATION & EXECUTION INSTRUCTIONS
------------------------------------

Requirements:
-------------
• C/C++ compiler with OpenMP support (e.g., gcc or clang)
• Rust toolchain with `cargo` installed
• Python 3 with Jupyter and matplotlib (for analysis)

------

1. OpenMP Benchmarks (C/C++)
----------------------------

To compile all OpenMP benchmarks:

    make

This will generate the executables in the `bin/` directory.

To run a specific benchmark (e.g., dot_product):

    export OMP_NUM_THREADS=<num_threads>
    ./bin/dot_product

For the full benchmarking script (varying thread counts):

    cd rust_proj
    bash run_benchmark.sh <benchmark_name>

Note: You may need to adjust the `Makefile` or paths depending on your system setup.

------

2. Rust Benchmarks (Rayon)
---------------------------

Navigate into the Rust project directory:

    cd rust_proj

To compile:

    cargo build --release

To run a benchmark (e.g., dot_product):

    export RAYON_NUM_THREADS=<num_threads>
    cargo run --release --bin dot_product

To run full benchmark suite with results output to CSV:

    bash run_benchmark.sh <benchmark_name>

Results will be saved in: `rust_proj/results/`

------

3. Viewing Benchmark Results & Analysis
---------------------------------------

Navigate to the `Analysis/` directory:

    cd Analysis

You can view the interactive plots and analysis in the Jupyter notebook:

    jupyter notebook Analysis.ipynb

A static version is available as `Analysis.pdf` for quick reference.

------

4. Directory Layout
--------------------

• `benchmarks/` – Source code for all 9 benchmarks (Rust and OpenMP)
• `bin/` – Compiled OpenMP binaries
• `results/` – Benchmark result CSV files (used in analysis)
• `rust_proj/` – Standalone Rust project with all Rust benchmark implementations
• `Analysis/` – Jupyter notebook, output plots, markdown, and PDF report
• `Makefile` – Build script for OpenMP programs
• `README.md` – Project overview (for GitHub)
• `readme.txt` – You are here

------

Git repo: 

