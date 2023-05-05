#include "big_integer.h"
#include <gtest/gtest.h>

TEST(big_integer_tests, EmptyConstructorCreateZero) {
    zhejiangfhe::BigInteger<u_int32_t> zero;
    ASSERT_EQ(zero.ConvertToLimb(), 0);
    ASSERT_EQ(zero.ConvertToString(), "0");
}

TEST(big_integer_tests, CreatesPositiveBigIntegersWithStringRepresentation) {
    zhejiangfhe::BigInteger<u_int32_t> bigInt("0");
    ASSERT_EQ(bigInt.ConvertToLimb(), 0);
    ASSERT_EQ(bigInt.ConvertToString(), "0");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("10");
    ASSERT_EQ(bigInt.ConvertToLimb(), 10ULL);
    ASSERT_EQ(bigInt.ConvertToString(), "10");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("275");
    ASSERT_EQ(bigInt.ConvertToLimb(), 275ULL);
    ASSERT_EQ(bigInt.ConvertToString(), "275");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("12345678901234567890");
    ASSERT_EQ(bigInt.ConvertToString(), "12345678901234567890");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901");
    ASSERT_EQ(bigInt.ConvertToString(), "123456789012345678901");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("1234567890123456789012345678901234567890");
    ASSERT_EQ(bigInt.ConvertToString(), "1234567890123456789012345678901234567890");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890123456789012345678901234567890");
    ASSERT_EQ(bigInt.ConvertToString(), "123456789012345678901234567890123456789012345678901234567890");
}

TEST(big_integer_tests, CreatesNegativeBigIntegersWithStringRepresentation) {
    zhejiangfhe::BigInteger<u_int32_t> bigInt("-0");
    ASSERT_EQ(bigInt.ConvertToLimb(), 0);
    ASSERT_EQ(bigInt.ConvertToString(), "0");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("-10");
    ASSERT_EQ(bigInt.ConvertToLimb(), -10);
    ASSERT_EQ(bigInt.ConvertToString(), "-10");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("-275");
    ASSERT_EQ(bigInt.ConvertToLimb(), -275);
    ASSERT_EQ(bigInt.ConvertToString(), "-275");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("-12345678901234567890");
    ASSERT_EQ(bigInt.ConvertToString(), "-12345678901234567890");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("-123456789012345678901");
    ASSERT_EQ(bigInt.ConvertToString(), "-123456789012345678901");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("-1234567890123456789012345678901234567890");
    ASSERT_EQ(bigInt.ConvertToString(), "-1234567890123456789012345678901234567890");

    bigInt = zhejiangfhe::BigInteger<u_int32_t>("-123456789012345678901234567890123456789012345678901234567890");
    ASSERT_EQ(bigInt.ConvertToString(), "-123456789012345678901234567890123456789012345678901234567890");
}

// TEST(big_integer_tests, AddTwoUnsignedBigIntegers) {
//     zhejiangfhe::BigInteger<u_int32_t> bigIntOne("3");
//     zhejiangfhe::BigInteger<u_int32_t> bigIntTwo("-4");
//     ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "-1");
//     bigIntOne = zhejiangfhe::BigInteger<u_int32_t>();
//     bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("1234567");
//     ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "1234567");
//     bigIntOne = zhejiangfhe::BigInteger<u_int32_t>("0");
//     bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890");
//     ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "123456789012345678901234567890");
//     bigIntOne = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890");
//     bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("-123456789012345678901234567890");
//     ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(),
//               "0");
//     bigIntOne = zhejiangfhe::BigInteger<u_int32_t>("12345678901234567890123456789012345678901234567890");
//     bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890");
//     ASSERT_EQ(bigIntOne.Add(bigIntTwo)
//                       .ConvertToString(),
//               "12345678901234567890246913578024691357802469135780");
// }

TEST(big_integer_tests, CompareTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<u_int32_t>;

    ASSERT_EQ(BInt().Compare(BInt("0")), 0);
    ASSERT_EQ(BInt().Compare(BInt("-1")), 1);
    ASSERT_EQ(BInt("-1").Compare(BInt()), -1);
    ASSERT_EQ(BInt().Compare(BInt("1")), -1);
    ASSERT_EQ(BInt("2").Compare(BInt("4")), -1);
    ASSERT_EQ(BInt("8").Compare(BInt("1234567890123456789012345678901234567890")), -1);
    ASSERT_EQ(BInt("123456789012345678901234567890123456789012345678901234567890").Compare(BInt("1234567890123456789012345678901234567890")), 1);
    ASSERT_EQ(BInt("123456789012345678901234567890123456789012345678901234567890").Compare(BInt("18446744073709551615")), 1);
    ASSERT_EQ(BInt("-123456789012345678901234567890123456789012345678901234567890").Compare(BInt("-18446744073709551615")), -1);
    ASSERT_EQ(BInt("18446744073709551615").Compare(BInt("-123456789012345678901234567890123456789012345678901234567890")), 1);

    std::vector<u_int32_t> vals1;
    std::vector<u_int32_t> vals2;
    ASSERT_EQ(BInt(vals1).Compare(BInt(vals2)), 0);

    vals1.push_back(12);
    ASSERT_EQ(BInt(vals1).Compare(BInt(vals2)), 1);

    vals2.push_back(13);
    ASSERT_EQ(BInt(vals1).Compare(BInt(vals2)), -1);

    vals1.push_back(14);
    vals2.push_back(14);
    ASSERT_EQ(BInt(vals1).Compare(BInt(vals2)), -1);
}

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
    sut.MultiplyWithKaratsuba(1ULL, 1ULL, result);
    ASSERT_EQ(1ULL, result[0]);
    ASSERT_EQ(0ULL, result[1]);
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