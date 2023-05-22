#include "big_integer.h"
#include "big_integer_modop.h"
#include "big_integer_test.h"


// 创建继承自 Test 的 test fixture
template<typename T>
class BigIntegerModTest : public testing::Test {};

// 将 test fixture 与类型列表绑定在一起，这样下面的测试就可以自动对类型列表中的每一种都执行一遍了
TYPED_TEST_SUITE(BigIntegerModTest, LimbTypes);

TYPED_TEST(BigIntegerModTest, Mod) {
    using BInt = zhejiangfhe::BigIntegerMod<TypeParam>;
    EXPECT_EQ(BInt("-2").Mod(BInt("3")).ConvertToString(), "1");
    EXPECT_EQ(BInt("-7").Mod(BInt("3")).ConvertToString(), "2");
    EXPECT_TRUE(BInt().Mod(BInt("1")) == BInt());
    EXPECT_TRUE(BInt().Mod(BInt("123456789012345678901234567890")) == BInt());
    EXPECT_TRUE(BInt("10").Mod(BInt("1")) == BInt());
    EXPECT_TRUE(BInt("1").Mod(BInt("2")) == BInt("1"));
    EXPECT_TRUE(BInt("123456789012345678901234567890").Mod(BInt("1234567890123456789012345678903")) == BInt("123456789012345678901234567890"));
    EXPECT_TRUE(BInt("123456789012345678901234567890").Mod(BInt("1234567890123456789012345678901")) == BInt("123456789012345678901234567890"));
    EXPECT_TRUE(BInt("2").Mod(BInt("3")) == BInt("2"));

    EXPECT_TRUE(BInt("123456789012345678901234567891").Mod(BInt("2")) == BInt("1"));
    EXPECT_TRUE(BInt("1234567890123456789012345678903").Mod(BInt("1234567890123456789012345678900")) == BInt("3"));
    EXPECT_TRUE(BInt("12345678901234567890123456789031234567890123456789012345678903").Mod(BInt("1234567890123456789012345678900")) == BInt("370370637037037063703703706403"));

    BInt num("12345678901234567890123456789031234567890123456789012345678903");
    num.ModEq(BInt("1234567890123456789012345678900"));
    EXPECT_TRUE(num == BInt("370370637037037063703703706403"));
}
TYPED_TEST(BigIntegerModTest, ModAdd) {
    using BInt = zhejiangfhe::BigIntegerMod<TypeParam>;

    EXPECT_TRUE(BInt().ModIncrement(BInt("1")) == BInt());
    EXPECT_TRUE(BInt("1").ModIncrement(BInt("2")) == BInt());
    EXPECT_TRUE(BInt().ModIncrement(BInt("123456789012345678901234567890")) == BInt("1"))<<BInt().ModIncrement(BInt("123456789012345678901234567890")).ConvertToString().c_str();
    EXPECT_TRUE(BInt("123456789012345678901234567889").ModIncrement(BInt("123456789012345678901234567890")) == BInt())<<BInt("123456789012345678901234567889").ModIncrement(BInt("123456789012345678901234567890")).ConvertToString().c_str();
    EXPECT_TRUE(BInt("1").ModAdd(BInt("2"),BInt("2")) == BInt("1"))<<BInt("1").ModAdd(BInt("2"),BInt("2")).ConvertToString().c_str();
    EXPECT_TRUE(BInt("123456789012345678901234567889").ModAdd(BInt("1"),BInt("1234567890123456789012345678903")) == BInt("123456789012345678901234567890"))<<BInt("123456789012345678901234567889").ModAdd(BInt("1"),BInt("1234567890123456789012345678903")).ConvertToString().c_str();
    EXPECT_TRUE(BInt("123456789012345678901234567890").ModAdd(BInt("1234567890123456789012345678901"),BInt("1234567890123456789012345678901")) == BInt("123456789012345678901234567890"))<<BInt("123456789012345678901234567890").ModAdd(BInt("1234567890123456789012345678901"),BInt("1234567890123456789012345678901")).ConvertToString().c_str();
    EXPECT_TRUE(BInt("2").ModAdd(BInt("3"),BInt("3")) == BInt("2"))<<BInt("2").ModAdd(BInt("3"),BInt("3")).ConvertToString().c_str();

    EXPECT_TRUE(BInt("123456789012345678901234567890").ModAdd(BInt("1"),BInt("2")) == BInt("1"))<<BInt("2").ModAdd(BInt("3"),BInt("3")).ConvertToString().c_str();

    BInt num("12345678901234567890123456789031234567890123456789012345678900");
    num.ModAddEq(BInt("2"),BInt("1234567890123456789012345678900"));
    // EXPECT_TRUE(num == BInt("370370637037037063703703706403"))<<num.ConvertToString().c_str();
}



TYPED_TEST(BigIntegerModTest, ModSub) {
    using BInt = zhejiangfhe::BigIntegerMod<TypeParam>;

    // (operand1 - operand2) == modulus
    EXPECT_TRUE(BInt("3").ModSub(BInt("1"), BInt("2")) == BInt());

    // (operand1 - operand2) > modulus
    EXPECT_TRUE(BInt("6").ModSub(BInt("1"), BInt("2")) == BInt(1));

    // (operand1 - operand2) < ~modulus
    EXPECT_TRUE(BInt("1").ModSub(BInt("3"), BInt("3")) == BInt(1));



    // operand1 - operand2 = 0
    EXPECT_TRUE(BInt("3").ModSub(BInt("3"), BInt("3")) == BInt());


    // operand1 - operand2 = 0
    EXPECT_EQ(BInt("122233444322332244332222").ModSub(BInt("456434678665445567"), BInt("3456789")).ConvertToString(), "1632671");

}


TYPED_TEST(BigIntegerModTest, ModBarret) {
    using BInt = zhejiangfhe::BigIntegerMod<TypeParam>;

    BInt result = BInt("11").ModBarrett(3);
    EXPECT_TRUE(result == BInt("2")) << "Expected mod result is 1, got " << result.ConvertToString() << " instead.";

    result = BInt("11").ModBarrett(5);
    EXPECT_TRUE(result == BInt("1")) << "Expected mod result is 1, got " << result.ConvertToString() << " instead.";
}

TYPED_TEST(BigIntegerModTest, ModMul) {
    using BInt = zhejiangfhe::BigIntegerMod<TypeParam>;

    // (m * n) mod modulus
    // m == 0 || n == 0, result should be 0
    EXPECT_TRUE(BInt("3").ModMul(BInt(), BInt("7")) == BInt());
    EXPECT_TRUE(BInt("").ModMul(BInt("3"), BInt("7")) == BInt());

    // (m * n) < modulus, result should be m * n
    EXPECT_TRUE(BInt("3").ModMul(BInt("5"), BInt("16")) == BInt("15"));

    // (m * n) > modulus
    EXPECT_TRUE(BInt("35").ModMul(BInt("21"), BInt("16")) == BInt("15"));

    // Big Integer Senario
    EXPECT_TRUE(BInt("123456789012345678901234567890").ModMul(BInt("987654321098765432109876543210"), BInt("9999999999999999999912")) == BInt("237334552122396220044"));

    // ModMul inplace
    BInt m("123456789012345678901234567890");
    BInt n("987654321098765432109876543210");
    BInt modulus("9999999999999999999912");
    BInt expectedResult("237334552122396220044");
    EXPECT_TRUE(m.ModMulEq(n, modulus) == expectedResult);
    EXPECT_TRUE(m == expectedResult);
}