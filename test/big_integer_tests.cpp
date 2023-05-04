#include "big_integer.h"
#include <gtest/gtest.h>

TEST(big_integer_tests, EmptyConstructorCreateZero) {
    zhejiangfhe::BigInteger<u_int32_t> zero;
    ASSERT_EQ(zero.ConvertToLimb(), 0);
    ASSERT_EQ(zero.ConvertToString(), "0");
}

TEST(big_integer_tests, CreatesBigUnsignedIntegerWithStringRepresentation) {
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

TEST(big_integer_tests, CreatesNegativeBigUnsignedIntegerWithStringRepresentation) {
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
TEST(big_integer_tests, AddTwoUnsignedBigIntegers) {
    zhejiangfhe::BigInteger<u_int32_t> bigIntOne("3");
    zhejiangfhe::BigInteger<u_int32_t> bigIntTwo("-4");
    ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "-1");
    bigIntOne = zhejiangfhe::BigInteger<u_int32_t>();
    bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("1234567");
    ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "1234567");
    bigIntOne = zhejiangfhe::BigInteger<u_int32_t>("0");
    bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890");
    ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "123456789012345678901234567890");
    bigIntOne = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890");
    bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("-123456789012345678901234567890");
    ASSERT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(),
              "0");
    bigIntOne = zhejiangfhe::BigInteger<u_int32_t>("12345678901234567890123456789012345678901234567890");
    bigIntTwo = zhejiangfhe::BigInteger<u_int32_t>("123456789012345678901234567890");
    ASSERT_EQ(bigIntOne.Add(bigIntTwo)
                      .ConvertToString(),
              "12345678901234567890246913578024691357802469135780");
}