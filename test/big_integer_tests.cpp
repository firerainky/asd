#include "big_integer.h"
#include <gtest/gtest.h>

TEST(big_integer_tests, MultiplyWithKaratsuba) {
    using BInt = zhejiangfhe::BigInteger<u_int32_t>;
    BInt sut;
    u_int32_t result[2];

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
    using BInt = zhejiangfhe::BigInteger<u_int64_t>;
    BInt sut;
    u_int64_t result[2];

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

TEST(big_integer_tests, MultiplyTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<u_int32_t>;

    ASSERT_EQ(BInt().Mul(BInt("5")).ConvertToString(), "0");
    ASSERT_EQ(BInt("3").Mul(BInt("4")).ConvertToString(), "12");
    ASSERT_EQ(BInt("1234567").Mul(BInt()).ConvertToString(), "0");
    ASSERT_EQ(BInt("1234567").Mul(BInt("1234567")).ConvertToString(), "1524155677489");
    ASSERT_EQ(BInt("123456789012345678901234567890").Mul(BInt("1")).ConvertToString(),
              "123456789012345678901234567890");
    ASSERT_EQ(BInt("123456789012345678901234567890").Mul(BInt("0")).ConvertToString(), "0");
    ASSERT_EQ(BInt("123456789012345678901234567890").Mul(BInt("123456789012345678901234567890")).ConvertToString(),
              "15241578753238836750495351562536198787501905199875019052100");
    ASSERT_EQ(BInt("999999999999999999999999999999").Mul(BInt("999999999999999999999999999999")).ConvertToString(),
              "999999999999999999999999999998000000000000000000000000000001");

    // For negative numbers
    ASSERT_EQ(BInt().Mul(BInt("-5")).ConvertToString(), "0");
    ASSERT_EQ(BInt("4").Mul(BInt("-5")).ConvertToString(), "-20");
    ASSERT_EQ(BInt("-3").Mul(BInt("-5")).ConvertToString(), "15");
    ASSERT_EQ(BInt("1234567").Mul(BInt("-1234567")).ConvertToString(), "-1524155677489");
    ASSERT_EQ(BInt("123456789012345678901234567890").Mul(BInt("-1")).ConvertToString(),
              "-123456789012345678901234567890");
    ASSERT_EQ(BInt("-123456789012345678901234567890").Mul(BInt("0")).ConvertToString(), "0");
    ASSERT_EQ(BInt("123456789012345678901234567890").Mul(BInt("-123456789012345678901234567890")).ConvertToString(),
              "-15241578753238836750495351562536198787501905199875019052100");
    ASSERT_EQ(BInt("-999999999999999999999999999999").Mul(BInt("-999999999999999999999999999999")).ConvertToString(),
              "999999999999999999999999999998000000000000000000000000000001");
}

TEST(big_integer_tests, MultiplyEqTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<u_int32_t>;

    ASSERT_EQ(BInt().MulEq(BInt("5")).ConvertToString(), "0");

    BInt num = BInt("3");
    num.MulEq(BInt("4"));
    ASSERT_EQ(num.ConvertToString(), "12");

    num = BInt("999999999999999999999999999999");
    num.MulEq(BInt("999999999999999999999999999999"));
    ASSERT_EQ(num.ConvertToString(), "999999999999999999999999999998000000000000000000000000000001");

    num.MulEq(BInt("-999999999999999999999999999999"));
    ASSERT_EQ(num.ConvertToString(), "-999999999999999999999999999997000000000000000000000000000002999999999999999999999999999999");
}