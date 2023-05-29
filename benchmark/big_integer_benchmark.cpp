#include <benchmark/benchmark.h>

#include "big_integer.h"
#include "big_integer_modop.h"

static void BM_BigInt0Creation(benchmark::State &state) {
    for (auto _: state)
        zhejiangfhe::BigInteger<uint32_t> zero;
}
// Register the function as a benchmark
BENCHMARK(BM_BigInt0Creation);

static void BM_BigIntModMulDirect(benchmark::State &state) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;

    BInt a = 3, b = 4, modulus = 5;

    for (auto _: state) {
        BInt ans = (a * b) % modulus;
    }
}
BENCHMARK(BM_BigIntModMulDirect);

static void BM_BigIntModMulWithModulus(benchmark::State &state) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;
    // using Modulus = zhejiangfhe::Modulus<uint64_t>;

    // BInt a = 3, b = 4, m = 5;
    // Modulus modulus = Modulus(m);

    // for (auto _: state) {
    //     BInt ans = zhejiangfhe::ModMul(a, b, modulus);
    // }

    using Modulus = zhejiangfhe::Modulus<uint64_t>;

    BInt a = 3, b = 4, modulus = 5;

    for (auto _: state) {
        // BInt ans = zhejiangfhe::ModMul2(a, b, modulus);
    }
}
BENCHMARK(BM_BigIntModMulWithModulus);

BENCHMARK_MAIN();
