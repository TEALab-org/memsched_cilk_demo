# memsched_cilk_demo

This repo contains an experiment / demonstration for memory aware scheduling.

## Example
Note that often times you will need to include a `-I<path/to/cilk/include>` argument depending on your OpenCilk installation.
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

## Test
`test_mm.c` implements some integration tests to ensure the different matrix multiplication implementations have matching results.
You can build and run it with something like:
```
CILK_NWORKERS=8 \
clang \
        -fopencilk \
        -std=c11 \
        -g -Og \
        test_mm.c && ./a.out
```

## Formatting

```
clang-format -i --style=Chromium *.c *.h
```

