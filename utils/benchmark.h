/*
 * BENCHMARK MACRO
 *
 */
#ifndef BENCHMARK_H
#include <time.h>
//variable to store start time
clock_t bm_start;
/**
 * @brief run passed function specified number of times and returns elapsed time
 *
 * @param rounds  : number of times to run function
 * @param cb : callback function to run
 *
 * @return clock_t elapsed time in microseconds
 */
#define BM(rounds, cb)({ \
    bm_start = clock(); \
    for(int i = 0; i < rounds; ++i){ \
        cb; \
    } \
    clock() - bm_start; \
})

#endif /*benchmark.h*/