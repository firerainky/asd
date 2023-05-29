#include "big_integer.h"
#include "big_integer_test.h"

// 创建继承自 Test 的 test fixture
template<typename T>
class BigIntegerTest : public testing::Test {};

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
    EXPECT_EQ(BInt("-2").Compare(BInt("3")), -1);
    EXPECT_EQ(BInt("-2").Compare(BInt("-3")), 1);
    EXPECT_EQ(BInt("2").Compare(BInt("4")), -1);
    EXPECT_EQ(BInt("8").Compare(BInt("1234567890123456789012345678901234567890")), -1);
    EXPECT_EQ(BInt("123456789012345678901234567890123456789012345678901234567890").Compare(BInt("1234567890123456789012345678901234567890")), 1);
    EXPECT_EQ(BInt("123456789012345678901234567890123456789012345678901234567890").Compare(BInt("18446744073709551615")), 1);
    EXPECT_EQ(BInt("-123456789012345678901234567890123456789012345678901234567890").Compare(BInt("-18446744073709551615")), -1);
    EXPECT_EQ(BInt("18446744073709551615").Compare(BInt("-123456789012345678901234567890123456789012345678901234567890")), 1);

    EXPECT_TRUE(BInt("18446744073709551615") > BInt("-123456789012345678901234567890123456789012345678901234567890"));
    EXPECT_TRUE(BInt("-123456789012345678901234567890123456789012345678901234567890") == BInt("-123456789012345678901234567890123456789012345678901234567890"));

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

    bigIntOne = 0;
    bigIntTwo = 0;
    EXPECT_EQ(bigIntOne += bigIntTwo, 0);

    bigIntOne = 2;
    bigIntTwo = 3;
    EXPECT_EQ(bigIntOne += bigIntTwo, 5);

    bigIntOne = -2;
    bigIntTwo = 3;
    EXPECT_EQ(bigIntOne += bigIntTwo, 1);

    bigIntOne = 2;
    bigIntTwo = -3;
    EXPECT_EQ(bigIntOne += bigIntTwo, -1);
}

TYPED_TEST(BigIntegerTest, BiggerIntegerSubtractSmallerInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    BInt bigIntOne("3");
    BInt bigIntTwo("4");
    EXPECT_EQ(bigIntOne.Sub(bigIntTwo).ConvertToString(), "-1");
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

    bigIntOne = 0;
    bigIntTwo = 0;
    EXPECT_EQ(bigIntOne -= bigIntTwo, 0);

    // bigIntOne = 2;
    // bigIntTwo = 3;
    // EXPECT_EQ(bigIntOne -= bigIntTwo, -1);

    bigIntOne = -2;
    bigIntTwo = 3;
    EXPECT_EQ(bigIntOne -= bigIntTwo, -5);

    // bigIntOne = 2;
    // bigIntTwo = -3;
    // EXPECT_EQ(bigIntOne -= bigIntTwo, 5);
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
    EXPECT_EQ((BInt("-3") * BInt("-5")).ConvertToString(), "15");
    EXPECT_EQ(BInt("1234567").Mul(BInt("-1234567")).ConvertToString(), "-1524155677489");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("-1")).ConvertToString(),
              "-123456789012345678901234567890");
    EXPECT_EQ(BInt("-123456789012345678901234567890").Mul(BInt("0")).ConvertToString(), "0");
    EXPECT_EQ(BInt("123456789012345678901234567890").Mul(BInt("-123456789012345678901234567890")).ConvertToString(),
              "-15241578753238836750495351562536198787501905199875019052100");
    EXPECT_EQ((BInt("-999999999999999999999999999999") * BInt("-999999999999999999999999999999")).ConvertToString(),
              "999999999999999999999999999998000000000000000000000000000001");
}

TYPED_TEST(BigIntegerTest, MultiplyEqTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<uint32_t>;

    EXPECT_EQ(BInt().MulEq(BInt("5")).ConvertToString(), "0");

    BInt num = BInt("3");
    num.MulEq(BInt("4"));
    EXPECT_EQ(num.ConvertToString(), "12");

    num = BInt("999999999999999999999999999999");
    num *= BInt("999999999999999999999999999999");
    EXPECT_EQ(num.ConvertToString(), "999999999999999999999999999998000000000000000000000000000001");

    num *= BInt("-999999999999999999999999999999");
    EXPECT_EQ(num.ConvertToString(), "-999999999999999999999999999997000000000000000000000000000002999999999999999999999999999999");
}

TYPED_TEST(BigIntegerTest, DivideTwoBigInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    EXPECT_EQ(BInt(10).DividedBy(BInt(5)).first.ConvertToString(), "2");
    EXPECT_EQ(BInt("12884901888").DividedBy(BInt("4294967296")).first.ConvertToString(), "3");

    EXPECT_EQ(BInt("99999999999999999999999999999").DividedBy(BInt("33333333333333333333333333333")).first.ConvertToString(), "3");
    std::pair<BInt, BInt> ans = BInt("999999999999999999999999999998000000000000000000000000000001").DividedBy(BInt("999999999999999999999999999999"));
    EXPECT_EQ(ans.first.ConvertToString(), "999999999999999999999999999999");
}


TYPED_TEST(BigIntegerTest, LeftShiftBigInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    EXPECT_EQ(BInt(10).LeftShift(1).ConvertToString(), "20");

    EXPECT_EQ(BInt(1024).LeftShift(1).ConvertToString(), "2048");

    EXPECT_EQ(BInt("4294967297").LeftShift(1).ConvertToString(), "8589934594");

    EXPECT_EQ(BInt("4294967297") << 1, BInt("8589934594"));

    BInt num("4294967297");
    num <<= 1;
    EXPECT_EQ(num, BInt("8589934594"));
}


TYPED_TEST(BigIntegerTest, RightShiftBigInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    EXPECT_EQ(BInt(10).RightShift(1).ConvertToString(), "5");

    EXPECT_EQ(BInt(1024).RightShift(2).ConvertToString(), "256");
    EXPECT_EQ(BInt("4294967296").RightShift(1).ConvertToString(), "2147483648");

    EXPECT_EQ(BInt("4294967296") >> 1, BInt("2147483648"));

    BInt num("4294967296");
    num >>= 1;
    EXPECT_EQ(num, BInt("2147483648"));
}


TEST(BigIntegerTest, LeadingZero) {
    using BInt = zhejiangfhe::BigInteger<uint32_t>;
    EXPECT_EQ(BInt(1).nlz(1), 31);
    EXPECT_EQ(BInt(1).nlz(2), 30);


    using BInt64 = zhejiangfhe::BigInteger<uint64_t>;
    EXPECT_EQ(BInt64(1).nlz(1), 63);
    EXPECT_EQ(BInt64(1).nlz(2), 62);
}

TYPED_TEST(BigIntegerTest, ExpBigInteger) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    EXPECT_EQ(BInt(10).Exp(1).ConvertToString(), "10");
    EXPECT_EQ(BInt(10).Exp(2).ConvertToString(), "100");
}

TYPED_TEST(BigIntegerTest, BitwiseAnd) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    EXPECT_EQ(BInt("1").And(BInt()).ConvertToString(), "0");
    EXPECT_EQ(BInt("1").And(BInt("1")).ConvertToString(), "1");
    EXPECT_EQ(BInt("2").And(BInt("1")).ConvertToString(), "0");

    EXPECT_EQ(BInt("4294967295").And(BInt("4294967295")).ConvertToString(), "4294967295");
    EXPECT_EQ(BInt("1234567890123456789012345678901234567890").And(BInt("9876543210987654321098765432109876543210")).ConvertToString(), "340661406314991781975327611074046724802");
    EXPECT_EQ(BInt("1234567890123456789012345678901234567891").And(BInt("1")).ConvertToString(), "1");

    EXPECT_EQ(BInt("1234567890123456789012345678901234567891").And(BInt("-1")).ConvertToString(), "-1");
    EXPECT_EQ(BInt("1").And(BInt("-0")).ConvertToString(), "0");
    EXPECT_EQ(BInt("-1234567890123456789012345678901234567891").And(BInt("-1")).ConvertToString(), "-1");

    BInt num = BInt("-1234567890123456789012345678901234567891");
    EXPECT_TRUE(num.AndEq(BInt("-1")) == BInt("-1"));
    EXPECT_TRUE(num == BInt("-1"));

    EXPECT_TRUE((BInt("-1234567890123456789012345678901234567891") & BInt("-1")) == BInt("-1"));
}

TYPED_TEST(BigIntegerTest, BitwiseOr) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    EXPECT_EQ(BInt("1").Or(BInt()).ConvertToString(), "1");
    EXPECT_EQ(BInt("1").Or(BInt("1")).ConvertToString(), "1");
    EXPECT_EQ(BInt("2").Or(BInt("1")).ConvertToString(), "3");

    EXPECT_EQ(BInt("4294967295").Or(BInt("4294967295")).ConvertToString(), "4294967295");
    EXPECT_EQ(BInt("1234567890123456789012345678901234567890").Or(BInt("9876543210987654321098765432109876543210")).ConvertToString(), "10770449694796119328135783499937064386298");
    EXPECT_EQ(BInt("1234567890123456789012345678901234567890").Or(BInt("1")).ConvertToString(), "1234567890123456789012345678901234567891");

    EXPECT_EQ(BInt("1234567890123456789012345678901234567890").Or(BInt("-1")).ConvertToString(), "-1234567890123456789012345678901234567891");
    EXPECT_EQ(BInt("1").Or(BInt("-0")).ConvertToString(), "-1");
    EXPECT_EQ(BInt("-1234567890123456789012345678901234567890").Or(BInt("-1")).ConvertToString(), "-1234567890123456789012345678901234567891");

    BInt num = BInt("-1234567890123456789012345678901234567890");
    EXPECT_TRUE(num.OrEq(BInt("-1")) == BInt("-1234567890123456789012345678901234567891"));
    EXPECT_TRUE(num == BInt("-1234567890123456789012345678901234567891"));

    EXPECT_TRUE((BInt("-1234567890123456789012345678901234567890") | BInt("-1")) == BInt("-1234567890123456789012345678901234567891"));
}

template<typename T>
T maxT() {
    auto ret = T(0);
    auto lengthOfT = sizeof(T) * 8;
    for (auto i = 0; i < lengthOfT; ++i) {
        ret += T(1) << i;
    }
    return ret;
}

TYPED_TEST(BigIntegerTest, BitwiseNot) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    std::vector<TypeParam> vals = {maxT<TypeParam>()};
    EXPECT_TRUE(BInt().Not() == BInt(vals));

    vals = {maxT<TypeParam>() - 2, 0, maxT<TypeParam>() - 3};
    std::vector<TypeParam> valsAfterNot = {2, maxT<TypeParam>(), 3};
    EXPECT_TRUE(BInt(vals).Not() == BInt(valsAfterNot));
    EXPECT_TRUE(BInt(vals, true).Not() == BInt(valsAfterNot, true));

    BInt num = BInt(vals);
    EXPECT_TRUE(num.NotEq() == BInt(valsAfterNot));
    EXPECT_TRUE(num == BInt(valsAfterNot));
    EXPECT_TRUE(~BInt(vals) == BInt(valsAfterNot));

    num = BInt(vals);
    num = ~num;
    EXPECT_EQ(num, BInt(valsAfterNot));
}

TYPED_TEST(BigIntegerTest, BitwiseXor) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    std::vector<TypeParam> vals = {maxT<TypeParam>()};
    EXPECT_TRUE(BInt().Xor(BInt()) == BInt());
    EXPECT_TRUE(BInt().Xor(BInt("1")) == BInt("1"));
    EXPECT_TRUE(BInt("1").Xor(BInt("1")) == BInt("0"));
    EXPECT_TRUE(BInt("4294967295").Xor(BInt("4294967295")) == BInt("0"));
    EXPECT_TRUE(BInt("1234567890123456789012345678901234567890").Xor(BInt("1")) == BInt("1234567890123456789012345678901234567891"));
    EXPECT_TRUE(BInt("1234567890123456789012345678901234567890").Xor(BInt("9876543210987654321098765432109876543210")) == BInt("10429788288481127546160455888863017661496"));
    EXPECT_TRUE(BInt("1234567890123456789012345678901234567890").Xor(BInt("-9876543210987654321098765432109876543210")) == BInt("-10429788288481127546160455888863017661496"));
    EXPECT_TRUE(BInt("-1234567890123456789012345678901234567890").Xor(BInt("-9876543210987654321098765432109876543210")) == BInt("10429788288481127546160455888863017661496"));

    BInt num = BInt("-1234567890123456789012345678901234567890");
    EXPECT_TRUE(num.XorEq(BInt("-9876543210987654321098765432109876543210")) == BInt("10429788288481127546160455888863017661496"));
    EXPECT_TRUE(num == BInt("10429788288481127546160455888863017661496"));

    EXPECT_TRUE((BInt("-1234567890123456789012345678901234567890") ^ BInt("-9876543210987654321098765432109876543210")) == BInt("10429788288481127546160455888863017661496"));

    BInt num1("-1234567890123456789012345678901234567890");
    BInt num2("-9876543210987654321098765432109876543210");
    EXPECT_EQ(num1 ^ num2, BInt("10429788288481127546160455888863017661496"));
    num1 ^= num2;
    EXPECT_EQ(num1, BInt("10429788288481127546160455888863017661496"));
}

TYPED_TEST(BigIntegerTest, BitwiseNegate) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    std::vector<TypeParam> vals = {0, 1};
    EXPECT_TRUE(BInt().Negate() == BInt(vals));

    vals = {1, 0, 0};
    std::vector<TypeParam> valsAfterNegate = {maxT<TypeParam>(), 0, 0};
    EXPECT_TRUE(BInt(vals).Negate() == BInt(valsAfterNegate));
    EXPECT_TRUE(BInt(vals, true).Negate() == BInt(valsAfterNegate, true));

    BInt num = BInt(vals);
    EXPECT_TRUE(num.NegateEq() == BInt(valsAfterNegate));
    EXPECT_TRUE(num == BInt(valsAfterNegate));
}

TYPED_TEST(BigIntegerTest, OperatorMod) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    EXPECT_EQ(BInt() % BInt("1"), BInt());
    EXPECT_EQ(BInt("-2") % BInt("3"), BInt("1"));

    BInt operand("12345678901234567890123456789031234567890123456789012345678903");
    BInt modulus("1234567890123456789012345678900");
    EXPECT_EQ(operand % modulus, BInt("370370637037037063703703706403"));

    EXPECT_EQ(operand, BInt("12345678901234567890123456789031234567890123456789012345678903"));
    operand %= modulus;
    EXPECT_EQ(operand, BInt("370370637037037063703703706403"));
}

TYPED_TEST(BigIntegerTest, ModMul) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

    // (m * n) mod modulus
    // m, 0 || n, 0, result should be 0
    BInt operand1 = 3;
    BInt operand2 = 0;
    BInt modulus = 7;
    EXPECT_EQ(operand1.ModMul(operand2, modulus), 0);

    operand1 = 0;
    operand2 = 5;
    EXPECT_EQ(operand1.ModMul(operand2, modulus), 0);

    // (m * n) < modulus, result should be m * n
    operand1 = 3;
    operand2 = 5;
    modulus = 16;
    EXPECT_EQ(operand1.ModMul(operand2, modulus), 15);

    // (m * n) > modulus
    operand1 = 35;
    operand2 = 21;
    modulus = 16;
    EXPECT_EQ(operand1.ModMul(operand2, modulus), 15);

    // Big Integer Senario
    operand1 = BInt("123456789012345678901234567890");
    operand2 = BInt("987654321098765432109876543210");
    modulus = BInt("9999999999999999999912");
    EXPECT_EQ(operand1.ModMul(operand2, modulus), BInt("237334552122396220044"));
}