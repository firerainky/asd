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
using LimbTypes = ::testing::Types<u_int32_t, u_int64_t>;

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