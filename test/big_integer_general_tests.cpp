#include "big_integer.h"
#include <gtest/gtest.h>

/**
 * 使用 Typed Tests 同时测试不同 limb type 的 big integer
 * reference: https://github.com/google/googletest/blob/main/docs/advanced.md#typed-tests
 */

// 创建继承自 Test 的 test fixture
template<typename T>
class BigIntegerTest : public testing::Test {};

// 需要测试的 limb_type 类型列表，目前仅需要 uint32 和 uint64
using LimbTypes = ::testing::Types<uint32_t, uint64_t>;

// 将 test fixture 与类型列表绑定在一起，这样下面的测试就可以自动对类型列表中的每一种都执行一遍了
TYPED_TEST_SUITE(BigIntegerTest, LimbTypes);

TYPED_TEST(BigIntegerTest, CreatesZeroOnDefaultConstructor) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    BInt zero;
    EXPECT_EQ(zero.ConvertToLimb(), 0);
    EXPECT_EQ(zero.ConvertToString(), "0");
}

TYPED_TEST(BigIntegerTest, CreatesPositiveBigIntegersWithStringRepresentation) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    BInt bigInt("0");
    EXPECT_EQ(bigInt.ConvertToLimb(), 0);
    EXPECT_EQ(bigInt.ConvertToString(), "0");

    bigInt = BInt("10");
    EXPECT_EQ(bigInt.ConvertToLimb(), 10);
    EXPECT_EQ(bigInt.ConvertToString(), "10");

    bigInt = BInt("275");
    EXPECT_EQ(bigInt.ConvertToLimb(), 275);
    EXPECT_EQ(bigInt.ConvertToString(), "275");

    bigInt = BInt("12345678901234567890");
    EXPECT_EQ(bigInt.ConvertToString(), "12345678901234567890");

    bigInt = BInt("123456789012345678901");
    EXPECT_EQ(bigInt.ConvertToString(), "123456789012345678901");

    bigInt = BInt("1234567890123456789012345678901234567890");
    EXPECT_EQ(bigInt.ConvertToString(), "1234567890123456789012345678901234567890");

    bigInt = BInt("123456789012345678901234567890123456789012345678901234567890");
    EXPECT_EQ(bigInt.ConvertToString(), "123456789012345678901234567890123456789012345678901234567890");
}

TYPED_TEST(BigIntegerTest, CreatesNegativeBigIntegersWithStringRepresentation) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    BInt bigInt("-0");
    EXPECT_EQ(bigInt.ConvertToLimb(), 0);
    EXPECT_EQ(bigInt.ConvertToString(), "0");

    bigInt = BInt("-10");
    EXPECT_EQ(bigInt.ConvertToLimb(), -10);
    EXPECT_EQ(bigInt.ConvertToString(), "-10");

    bigInt = BInt("-275");
    EXPECT_EQ(bigInt.ConvertToLimb(), -275);
    EXPECT_EQ(bigInt.ConvertToString(), "-275");

    bigInt = BInt("-12345678901234567890");
    EXPECT_EQ(bigInt.ConvertToString(), "-12345678901234567890");

    bigInt = BInt("-123456789012345678901");
    EXPECT_EQ(bigInt.ConvertToString(), "-123456789012345678901");

    bigInt = BInt("-1234567890123456789012345678901234567890");
    EXPECT_EQ(bigInt.ConvertToString(), "-1234567890123456789012345678901234567890");

    bigInt = BInt("-123456789012345678901234567890123456789012345678901234567890");
    EXPECT_EQ(bigInt.ConvertToString(), "-123456789012345678901234567890123456789012345678901234567890");
}

TYPED_TEST(BigIntegerTest, CompareTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    EXPECT_EQ(BInt().Compare(BInt("0")), 0);
    EXPECT_EQ(BInt().Compare(BInt("-1")), 1);
    EXPECT_EQ(BInt("-1").Compare(BInt()), -1);
    EXPECT_EQ(BInt().Compare(BInt("1")), -1);
    EXPECT_EQ(BInt("2").Compare(BInt("4")), -1);
    EXPECT_EQ(BInt("8").Compare(BInt("1234567890123456789012345678901234567890")), -1);
    EXPECT_EQ(BInt("123456789012345678901234567890123456789012345678901234567890").Compare(BInt("1234567890123456789012345678901234567890")), 1);
    EXPECT_EQ(BInt("123456789012345678901234567890123456789012345678901234567890").Compare(BInt("18446744073709551615")), 1);
    EXPECT_EQ(BInt("-123456789012345678901234567890123456789012345678901234567890").Compare(BInt("-18446744073709551615")), -1);
    EXPECT_EQ(BInt("18446744073709551615").Compare(BInt("-123456789012345678901234567890123456789012345678901234567890")), 1);

    std::vector<TypeParam> vals1;
    std::vector<TypeParam> vals2;
    EXPECT_EQ(BInt(vals1).Compare(BInt(vals2)), 0);

    vals1.push_back(12);
    EXPECT_EQ(BInt(vals1).Compare(BInt(vals2)), 1);

    vals2.push_back(13);
    EXPECT_EQ(BInt(vals1).Compare(BInt(vals2)), -1);

    vals1.push_back(14);
    vals2.push_back(14);
    EXPECT_EQ(BInt(vals1).Compare(BInt(vals2)), -1);
}


TYPED_TEST(BigIntegerTest, AddTwoUnsignedBigIntegers) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    BInt bigIntOne("3");
    BInt bigIntTwo("-4");
    EXPECT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "-1");
    bigIntOne = BInt();
    bigIntTwo = BInt("1234567");
    EXPECT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "1234567");
    bigIntOne = BInt("0");
    bigIntTwo = BInt("123456789012345678901234567890");
    EXPECT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "123456789012345678901234567890");
    bigIntOne = BInt("123456789012345678901234567890");
    bigIntTwo = BInt("-123456789012345678901234567890");
    EXPECT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "0");
    bigIntOne = BInt("12345678901234567890123456789012345678901234567890");
    bigIntTwo = BInt("123456789012345678901234567890");
    EXPECT_EQ(bigIntOne.Add(bigIntTwo).ConvertToString(), "12345678901234567890246913578024691357802469135780");
}

TYPED_TEST(BigIntegerTest, BiggerIntegerSubtractSmallerInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    BInt bigIntOne("4");
    BInt bigIntTwo("3");
    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(), "1");
    bigIntOne = BInt("1234567");
    bigIntTwo = BInt();
    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(), "1234567");
    bigIntOne = BInt("123456789012345678901234567890");
    bigIntTwo = BInt("0");

    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(), "123456789012345678901234567890");
    bigIntOne = BInt("123456789012345678901234567890");
    bigIntTwo = BInt("123456789012345678901234567890");
    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(),
              "0");
    bigIntOne = BInt("12345678901234567890246913578024691357802469135780");
    bigIntTwo = BInt("123456789012345678901234567890");
    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(),
              "12345678901234567890123456789012345678901234567890");
    bigIntOne = BInt("100000000000000000000000000000000000000000000000000");
    bigIntTwo = BInt("99999999999999999999999999999999999999999999999999");
    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(),
              "1");
}

TYPED_TEST(BigIntegerTest, MultiplyTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    EXPECT_EQ(BInt().Mul(BInt("5")).ConvertToString(), "0");
    EXPECT_EQ(BInt("3").Mul(BInt("4")).ConvertToString(), "12");
    EXPECT_EQ(BInt("1234567").Mul(BInt()).ConvertToString(), "0");
    EXPECT_EQ(BInt("1234567").Mul(BInt("1234567")).ConvertToString(), "1524155677489");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("1")).ConvertToString(),
              "123456789012345678901234567890");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("0")).ConvertToString(), "0");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("123456789012345678901234567890")).ConvertToString(),
              "15241578753238836750495351562536198787501905199875019052100");
    EXPECT_EQ(BInt("999999999999999999999999999999").Mul(BInt("999999999999999999999999999999")).ConvertToString(),
              "999999999999999999999999999998000000000000000000000000000001");

    // For negative numbers
    EXPECT_EQ(BInt().Mul(BInt("-5")).ConvertToString(), "0");
    EXPECT_EQ(BInt("4").Mul(BInt("-5")).ConvertToString(), "-20");
    EXPECT_EQ(BInt("-3").Mul(BInt("-5")).ConvertToString(), "15");
    EXPECT_EQ(BInt("1234567").Mul(BInt("-1234567")).ConvertToString(), "-1524155677489");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("-1")).ConvertToString(),
              "-123456789012345678901234567890");
    EXPECT_EQ(BInt("-123456789012345678901234567890").Mul(BInt("0")).ConvertToString(), "0");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("-123456789012345678901234567890")).ConvertToString(),
              "-15241578753238836750495351562536198787501905199875019052100");
    EXPECT_EQ(BInt("-999999999999999999999999999999").Mul(BInt("-999999999999999999999999999999")).ConvertToString(),
              "999999999999999999999999999998000000000000000000000000000001");
}

TYPED_TEST(BigIntegerTest, MultiplyEqTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<uint32_t>;

    EXPECT_EQ(BInt().MulEq(BInt("5")).ConvertToString(), "0");

    BInt num = BInt("3");
    num.MulEq(BInt("4"));
    EXPECT_EQ(num.ConvertToString(), "12");

    num = BInt("999999999999999999999999999999");
    num.MulEq(BInt("999999999999999999999999999999"));
    EXPECT_EQ(num.ConvertToString(), "999999999999999999999999999998000000000000000000000000000001");

    num.MulEq(BInt("-999999999999999999999999999999"));
    EXPECT_EQ(num.ConvertToString(), "-999999999999999999999999999997000000000000000000000000000002999999999999999999999999999999");
}