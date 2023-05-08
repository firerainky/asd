#include "basic_math.h"
#include <gtest/gtest.h>

using namespace basic_math;

TEST(basic_math, MultiplyWithKaratsuba) {

    uint32_t result[2];

    MultiplyWithKaratsuba<uint32_t>(0, 0, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint32_t>(0, 1, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint32_t>(1, 0, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint32_t>(1, 1, result);
    EXPECT_EQ(1, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint32_t>(0x10000, 0xFABA, result);
    EXPECT_EQ(0xFABA0000, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint32_t>(0x100000, 0xFABA, result);
    EXPECT_EQ(0xABA00000, result[0]);
    EXPECT_EQ(0xF, result[1]);
    MultiplyWithKaratsuba<uint32_t>(11223344, 55667788, result);
    EXPECT_EQ(2726795840, result[0]);
    EXPECT_EQ(145467, result[1]);
}

TEST(big_integer_tests, MultiplyWithKaratsubaWithUInt64Limb) {

    uint64_t result[2];

    MultiplyWithKaratsuba<uint64_t>(0, 0, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint64_t>(0, 1, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint64_t>(1, 0, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint64_t>(1, 1, result);
    EXPECT_EQ(1ULL, result[0]);
    EXPECT_EQ(0ULL, result[1]);
    MultiplyWithKaratsuba<uint64_t>(0x10000, 0xFABA, result);
    EXPECT_EQ(0xFABA0000, result[0]);
    EXPECT_EQ(0, result[1]);
    MultiplyWithKaratsuba<uint64_t>(0x100000000ULL, 0xFAFABABAULL, result);
    EXPECT_EQ(0xFAFABABA00000000ULL, result[0]);
    EXPECT_EQ(0ULL, result[1]);
    MultiplyWithKaratsuba<uint64_t>(0x1000000000ULL, 0xFAFABABAULL, result);
    EXPECT_EQ(0xAFABABA000000000ULL, result[0]);
    EXPECT_EQ(0xFULL, result[1]);
    MultiplyWithKaratsuba<uint64_t>(1111222233334444ULL, 5555666677778888ULL, result);
    EXPECT_EQ(4140785562324247136ULL, result[0]);
    EXPECT_EQ(334670460471ULL, result[1]);
}