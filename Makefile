CC = gcc
CFLAGS = -O3 -fopenmp
BIN_DIR = bin

BENCHMARKS = dot_product matrix_mult prefix_sum sorting image_blur merge_sort pi_estimation

all: $(BENCHMARKS)

$(BENCHMARKS):
	$(CC) $(CFLAGS) benchmarks/$@/main.c -o $(BIN_DIR)/$@

clean:
	rm -f $(BIN_DIR)/*

