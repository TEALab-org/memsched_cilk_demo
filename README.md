# memsched_cilk_demo

This repo contains an experiment / demonstration for memory aware scheduling.

## Example
```
clang \
	-fopencilk \
	-std=c11 \
	-O3 \
	mem_sched_demo.c \
    -o mem_sched_demo

# Run with 
#  * n = BASE_WIDTH * 2^2 sized matrix
#  * 1024 bytes memory limit
#  * Hybrid matrix multiplication
./mem_sched_demo 2 1024 2
```

## Formatting

```
clang-format -i --style=Chromium *.c *.h
```

