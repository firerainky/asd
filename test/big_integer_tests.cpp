#include "big_integer.h"
#include <gtest/gtest.h>

TEST(big_integer_tests, MultiplyWithKaratsuba) {
    using BInt = zhejiangfhe::BigInteger<uint32_t>;
    BInt sut;
    uint32_t result[2];

    sut.MultiplyWithKaratsuba(0, 0, result);
    ASSERT_EQ(0, result[0]);
    ASSERT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(0, 1, result);
    ASSERT_EQ(0, result[0]);
    ASSERT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(1, 0, result);
    ASSERT_EQ(0, result[0]);
    ASSERT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(1, 1, result);
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(0x10000, 0xFABA, result);
    ASSERT_EQ(0xFABA0000, result[0]);
    ASSERT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(0x100000, 0xFABA, result);
    ASSERT_EQ(0xABA00000, result[0]);
    ASSERT_EQ(0xF, result[1]);
    sut.MultiplyWithKaratsuba(11223344, 55667788, result);
    ASSERT_EQ(2726795840, result[0]);
    ASSERT_EQ(145467, result[1]);
}

TEST(big_integer_tests, MultiplyWithKaratsubaWithUInt64Limb) {
    using BInt = zhejiangfhe::BigInteger<uint64_t>;
    BInt sut;
    uint64_t result[2];

    sut.MultiplyWithKaratsuba(0, 0, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(0, 1, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(1, 0, result);
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(1, 1, result);
    EXPECT_EQ(1ULL, result[0]);
    EXPECT_EQ(0ULL, result[1]);
    sut.MultiplyWithKaratsuba(0x10000, 0xFABA, result);
    EXPECT_EQ(0xFABA0000, result[0]);
    EXPECT_EQ(0, result[1]);
    sut.MultiplyWithKaratsuba(0x100000000ULL, 0xFAFABABAULL, result);
    EXPECT_EQ(0xFAFABABA00000000ULL, result[0]);
    EXPECT_EQ(0ULL, result[1]);
    sut.MultiplyWithKaratsuba(0x1000000000ULL, 0xFAFABABAULL, result);
    EXPECT_EQ(0xAFABABA000000000ULL, result[0]);
    EXPECT_EQ(0xFULL, result[1]);
    sut.MultiplyWithKaratsuba(1111222233334444ULL, 5555666677778888ULL, result);
    EXPECT_EQ(4140785562324247136ULL, result[0]);
    EXPECT_EQ(334670460471ULL, result[1]);
}