#include <benchmark/benchmark.h>

#include "big_integer.h"
#include "big_integer_modop.h"

static void BM_BigInt0Creation(benchmark::State &state) {
    for (auto _: state)
        zhejiangfhe::BInt zero;
}
// Register the function as a benchmark
BENCHMARK(BM_BigInt0Creation);

template<class... Args>
static void BM_ModMul_Direct(benchmark::State &state, Args &&...args) {

    auto args_tuple = std::make_tuple(std::move(args)...);
    zhejiangfhe::BInt a(std::get<0>(args_tuple));
    zhejiangfhe::BInt b(std::get<1>(args_tuple));
    zhejiangfhe::BInt modulus(std::get<2>(args_tuple));

    for (auto _: state) {
        zhejiangfhe::BInt ans = (a * b) % modulus;
    }
}
BENCHMARK_CAPTURE(BM_ModMul_Direct, big_integer_test, "123456789012345678901234567890", "987654321098765432109876543210", "9999999999999999999912");
BENCHMARK_CAPTURE(BM_ModMul_Direct, small_integer_test, "3", "4", "5");

template<class... Args>
static void BM_ModMul_OpenFHEMethod(benchmark::State &state, Args &&...args) {

    auto args_tuple = std::make_tuple(std::move(args)...);
    zhejiangfhe::BInt a(std::get<0>(args_tuple));
    zhejiangfhe::BInt b(std::get<1>(args_tuple));
    zhejiangfhe::BInt modulus(std::get<2>(args_tuple));

    for (auto _: state) {
        zhejiangfhe::BInt ans = a.ModMul(b, modulus);
    }
}
BENCHMARK_CAPTURE(BM_ModMul_OpenFHEMethod, big_integer_test, "123456789012345678901234567890", "987654321098765432109876543210", "9999999999999999999912");
BENCHMARK_CAPTURE(BM_ModMul_OpenFHEMethod, small_integer_test, "3", "4", "5");

template<class... Args>
static void BM_Mod_Direct(benchmark::State &state, Args &&...args) {
    auto args_tuple = std::make_tuple(std::move(args)...);
    zhejiangfhe::BInt a(std::get<0>(args_tuple));
    zhejiangfhe::BInt modulus(std::get<1>(args_tuple));

    for (auto _: state) {
        zhejiangfhe::BInt ans = a % modulus;
    }
}
BENCHMARK_CAPTURE(BM_Mod_Direct, big_integer_test, "18446744073709551614", "14097");
BENCHMARK_CAPTURE(BM_Mod_Direct, small_integer_test, "5", "3");


template<class... Args>
static void BM_Mod_Direct64(benchmark::State &state, Args &&...args) {
    auto args_tuple = std::make_tuple(std::move(args)...);
    uint64_t a = std::get<0>(args_tuple);
    uint64_t modulus = std::get<1>(args_tuple);

    for (auto _: state) {
        uint64_t ans = a % modulus;
    }
}
BENCHMARK_CAPTURE(BM_Mod_Direct64, big_integer_test, 18446744073709551614ULL, 14097);
BENCHMARK_CAPTURE(BM_Mod_Direct64, small_integer_test, 5, 3);


template<class... Args>
static void BM_Mod_Barret(benchmark::State &state, Args &&...args) {
    auto args_tuple = std::make_tuple(std::move(args)...);
    zhejiangfhe::BInt a(std::get<0>(args_tuple));
    zhejiangfhe::BMod modulus(zhejiangfhe::BInt(std::get<1>(args_tuple)));

    for (auto _: state) {
        zhejiangfhe::BInt ans = zhejiangfhe::util::ModBarrett(a, modulus);
    }
}
BENCHMARK_CAPTURE(BM_Mod_Barret, big_integer_test, "18446744073709551614", "14097");
BENCHMARK_CAPTURE(BM_Mod_Barret, small_integer_test, "5", "3");


// template<class... Args>
// static void BM_Mod_Barret64(benchmark::State &state, Args &&...args) {

//     auto args_tuple = std::make_tuple(std::move(args)...);
//     limbtype a = std::get<0>(args_tuple);
//     zhejiangfhe::BMod modulus(zhejiangfhe::BInt(std::get<1>(args_tuple)));

//     for (auto _: state) {
//         BInt ans = zhejiangfhe::util::ModBarrett64(a, modulus);
//     }
// }
// BENCHMARK_CAPTURE(BM_Mod_Barret64, big_integer_test, 4294967294, "14097");
// BENCHMARK_CAPTURE(BM_Mod_Barret64, small_integer_test, 5, "3");

BENCHMARK_MAIN();
