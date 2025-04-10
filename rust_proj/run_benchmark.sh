#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: ./run_benchmark.sh <benchmark_name>"
  exit 1
fi

BIN_NAME=$1
CSV_PATH="results/${BIN_NAME}_rust.csv"

echo "Running benchmark: $BIN_NAME"
echo "Threads,Time (ms),Result" > "$CSV_PATH"

for t in 1 2 4 8 16 32 64; do
  export RAYON_NUM_THREADS=$t
  echo "Running with $t threads..."

  output=$(cargo run --release --bin "$BIN_NAME")

  time=$(echo "$output" | grep "Time elapsed" | grep -oP '[\d.]+' | head -1)
  result=$(echo "$output" | grep -oP '(?<=\= )[\d.]+$' | tail -1)

  echo "$t,$time,$result" >> "$CSV_PATH"
done

echo "✅ Results saved to $CSV_PATH"

