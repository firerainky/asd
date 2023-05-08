#include <benchmark/benchmark.h>

#include "big_integer.h"

static void BM_BigInt0Creation(benchmark::State &state) {
    for (auto _: state)
        zhejiangfhe::BigInteger<uint32_t> zero;
}
// Register the function as a benchmark
BENCHMARK(BM_BigInt0Creation);

BENCHMARK_MAIN();
