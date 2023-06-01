#include <benchmark/benchmark.h>

#include "big_integer.h"
#include "big_integer_modop.h"

static void BM_BigInt0Creation(benchmark::State &state) {
    for (auto _: state)
        zhejiangfhe::BigInteger<uint32_t> zero;
}
// Register the function as a benchmark
BENCHMARK(BM_BigInt0Creation);

template<class... Args>
static void BM_ModMul_Direct(benchmark::State &state, Args &&...args) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;

    auto args_tuple = std::make_tuple(std::move(args)...);
    BInt a(std::get<0>(args_tuple));
    BInt b(std::get<1>(args_tuple));
    BInt modulus(std::get<2>(args_tuple));

    for (auto _: state) {
        BInt ans = (a * b) % modulus;
    }
}
BENCHMARK_CAPTURE(BM_ModMul_Direct, big_integer_test, "123456789012345678901234567890", "987654321098765432109876543210", "9999999999999999999912");
BENCHMARK_CAPTURE(BM_ModMul_Direct, small_integer_test, "3", "4", "5");

template<class... Args>
static void BM_ModMul_OpenFHEMethod(benchmark::State &state, Args &&...args) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;

    auto args_tuple = std::make_tuple(std::move(args)...);
    BInt a(std::get<0>(args_tuple));
    BInt b(std::get<1>(args_tuple));
    BInt modulus(std::get<2>(args_tuple));

    for (auto _: state) {
        BInt ans = a.ModMul(b, modulus);
    }
}
BENCHMARK_CAPTURE(BM_ModMul_OpenFHEMethod, big_integer_test, "123456789012345678901234567890", "987654321098765432109876543210", "9999999999999999999912");
BENCHMARK_CAPTURE(BM_ModMul_OpenFHEMethod, small_integer_test, "3", "4", "5");

template<class... Args>
static void BM_Mod_Direct(benchmark::State &state, Args &&...args) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;

    auto args_tuple = std::make_tuple(std::move(args)...);
    BInt a(std::get<0>(args_tuple));
    BInt modulus(std::get<1>(args_tuple));

    for (auto _: state) {
        BInt ans = a % modulus;
    }
}
BENCHMARK_CAPTURE(BM_Mod_Direct, big_integer_test, "12345678901234567890123456789031234567890123456789012345678903", "1234567890123456789012345678900");
BENCHMARK_CAPTURE(BM_Mod_Direct, small_integer_test, "5", "3");

template<class... Args>
static void BM_Mod_Barret(benchmark::State &state, Args &&...args) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;
    using Modulus = zhejiangfhe::Modulus<uint64_t>;

    auto args_tuple = std::make_tuple(std::move(args)...);
    BInt a(std::get<0>(args_tuple));
    Modulus modulus(BInt(std::get<1>(args_tuple)));

    for (auto _: state) {
        BInt ans = zhejiangfhe::util::Mod(a, modulus);
    }
}
BENCHMARK_CAPTURE(BM_Mod_Barret, big_integer_test, "12345678901234567890123456789031234567890123456789012345678903", "1234567890123456789012345678900");
BENCHMARK_CAPTURE(BM_Mod_Barret, small_integer_test, "5", "3");

BENCHMARK_MAIN();
