#include "big_integer.h"
#include "big_integer_modop.h"
#include "big_integer_test.h"
#include "modulus.h"


namespace zhejiangfhe {
    // 创建继承自 Test 的 test fixture
    template<typename T>
    class BigIntegerModTest : public testing::Test {};

    // 将 test fixture 与类型列表绑定在一起，这样下面的测试就可以自动对类型列表中的每一种都执行一遍了
    TYPED_TEST_SUITE(BigIntegerModTest, LimbTypes);

    TYPED_TEST(BigIntegerModTest, Mod) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;

        BInt operand = BInt("-2");
        Modulus modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("1"));

        operand = BInt("-7");
        EXPECT_EQ(util::Mod(operand, modulus).ConvertToString(), "2");

        operand = BInt();
        modulus = Modulus(BInt("1"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt());

        modulus = Modulus(BInt("123456789012345678901234567890"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt());


        operand = BInt("10");
        modulus = Modulus(BInt("1"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt());


        operand = BInt("1");
        modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("1"));

        operand = BInt("2");
        modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("2"));


        operand = BInt("123456789012345678901234567890");
        modulus = Modulus(BInt("1234567890123456789012345678903"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("123456789012345678901234567890"));

        operand = BInt("123456789012345678901234567890");
        modulus = Modulus(BInt("1234567890123456789012345678901"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("123456789012345678901234567890"));


        operand = BInt("123456789012345678901234567891");
        modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("1"));

        operand = BInt("1234567890123456789012345678903");
        modulus = Modulus(BInt("1234567890123456789012345678900"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("3"));

        operand = BInt("12345678901234567890123456789031234567890123456789012345678903");
        modulus = Modulus(BInt("1234567890123456789012345678900"));
        EXPECT_EQ(util::Mod(operand, modulus), BInt("370370637037037063703703706403"));
    }

    TYPED_TEST(BigIntegerModTest, ModBarrett) {
        using BInt = zhejiangfhe::BigInteger<uint64_t>;

        BInt operand = 3;
        Modulus modulus = Modulus(BInt(2));
        EXPECT_EQ(util::ModBarrett(operand, modulus), 1);

        operand = 0;
        EXPECT_EQ(util::ModBarrett(operand, modulus), 0);

        operand = BInt(18446744073709551614ULL);
        modulus = Modulus(BInt(14097));
        EXPECT_EQ(util::ModBarrett(operand, modulus), BInt(13589));
    }

    TYPED_TEST(BigIntegerModTest, ModAdd) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Modulus = zhejiangfhe::Modulus<TypeParam>;

        BInt operand = BInt();
        Modulus modulus = Modulus(BInt("1"));
        EXPECT_EQ(util::ModIncrement(operand, modulus), BInt());

        operand = BInt("1");
        modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::ModIncrement(operand, modulus), BInt());

        operand = BInt();
        modulus = Modulus(BInt("123456789012345678901234567890"));
        EXPECT_EQ(util::ModIncrement(operand, modulus), BInt("1"));

        operand = BInt("123456789012345678901234567889");
        modulus = Modulus(BInt("123456789012345678901234567890"));
        EXPECT_EQ(util::ModIncrement(operand, modulus), BInt());

        BInt operand1 = BInt("1");
        BInt operand2 = BInt("2");
        modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::ModAdd(operand1, operand2, modulus), BInt("1"));


        operand1 = BInt("123456789012345678901234567889");
        operand2 = BInt("1");
        modulus = Modulus(BInt("1234567890123456789012345678903"));
        EXPECT_EQ(util::ModAdd(operand1, operand2, modulus), BInt("123456789012345678901234567890"));


        operand1 = BInt("123456789012345678901234567890");
        operand2 = BInt("1234567890123456789012345678901");
        modulus = Modulus(BInt("1234567890123456789012345678901"));
        EXPECT_EQ(util::ModAdd(operand1, operand2, modulus), BInt("123456789012345678901234567890"));


        operand1 = BInt("2");
        operand2 = BInt("3");
        modulus = Modulus(BInt("4"));
        EXPECT_EQ(util::ModAdd(operand1, operand2, modulus), BInt("1"));


        operand1 = BInt("123456789012345678901234567890");
        operand2 = BInt("1");
        modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::ModAdd(operand1, operand2, modulus), BInt("1"));
    }


    TYPED_TEST(BigIntegerModTest, ModSub) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Modulus = zhejiangfhe::Modulus<TypeParam>;

        // (operand1 - operand2), modulus
        BInt operand1 = BInt("3");
        BInt operand2 = BInt("1");
        Modulus modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::ModSub(operand1, operand2, modulus), BInt());

        // (operand1 - operand2) > modulus
        operand1 = BInt("6");
        operand2 = BInt("1");
        modulus = Modulus(BInt("2"));
        EXPECT_EQ(util::ModSub(operand1, operand2, modulus), BInt(1));

        // (operand1 - operand2) < ~modulus
        operand1 = BInt("1");
        operand2 = BInt("3");
        modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::ModSub(operand1, operand2, modulus).ConvertToString(), BInt(1).ConvertToString());


        // operand1 - operand2 = 0
        operand1 = BInt("2");
        operand2 = BInt("3");
        modulus = Modulus(BInt("4"));
        EXPECT_EQ(util::ModSub(operand1, operand2, modulus), BInt(3));


        // operand1 - operand2 = 0
        operand1 = BInt("122233444322332244332222");
        operand2 = BInt("456434678665445567");
        modulus = Modulus(BInt("3456789"));
        EXPECT_EQ(util::ModSub(operand1, operand2, modulus), BInt("1632671"));
    }


    TYPED_TEST(BigIntegerModTest, ModMul) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;

        // (m * n) mod modulus
        // m, 0 || n, 0, result should be 0
        BInt operand1 = BInt("3");
        BInt operand2 = BInt();
        Modulus modulus = Modulus(BInt("7"));
        EXPECT_EQ(util::ModMul(operand1, operand2, modulus), BInt());

        operand1 = BInt();
        operand2 = BInt("3");
        EXPECT_EQ(util::ModMul(operand1, operand2, modulus), BInt());

        // (m * n) < modulus, result should be m * n
        operand1 = BInt("3");
        operand2 = BInt("5");
        modulus = Modulus(BInt("16"));
        EXPECT_EQ(util::ModMul(operand1, operand2, modulus), BInt("15"));

        // (m * n) > modulus
        operand1 = BInt("35");
        operand2 = BInt("21");
        modulus = Modulus(BInt("16"));
        EXPECT_EQ(util::ModMul(operand1, operand2, modulus), BInt("15"));

        // Big Integer Senario
        operand1 = BInt("123456789012345678901234567890");
        operand2 = BInt("987654321098765432109876543210");
        modulus = Modulus(BInt("9999999999999999999912"));
        EXPECT_EQ(util::ModMul(operand1, operand2, modulus), BInt("237334552122396220044"));
    }


    TYPED_TEST(BigIntegerModTest, ModExp) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;

        BInt operand = BInt("2");
        BInt exponent = BInt();
        Modulus modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(1));
        
        operand = BInt("5");
        exponent = BInt("1");
        modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(2));

        operand = BInt("2");
        exponent = BInt("1");
        modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(2));

        operand = BInt("2");
        exponent = BInt("2");
        modulus = Modulus(BInt("3"));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(1));


        operand = BInt("1");
        exponent = BInt(0xFFFFFFFFFFFFFFFFLL, false);
        modulus = Modulus(BInt("5"));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(1));


        operand = BInt("2");
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(3));


        operand = BInt("2");
        exponent = BInt("60");
        modulus = Modulus(BInt(0x1000000000000000ULL, false));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt());


        operand = BInt("2");
        exponent = BInt("59");
        modulus = Modulus(BInt(0x1000000000000000ULL, false));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt((1ULL << 59), false));

        operand = BInt("2424242424");
        exponent = BInt("16");
        modulus = Modulus(BInt("131313131313"));
        EXPECT_EQ(util::ModExp(operand, exponent, modulus), BInt(39418477653));
    }
    TYPED_TEST(BigIntegerModTest, ModInverse) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;

        BInt operand = BInt("3");
        Modulus modulus = Modulus(BInt("7"));
        EXPECT_EQ(util::ModInverse(operand, modulus), BInt("5"));

        BInt operand2 = BInt("4");
        Modulus modulus2 = Modulus(BInt("6"));
        EXPECT_THROW(util::ModInverse(operand2, modulus2), zhejiangfhe::MathException);

        BInt operand3 = BInt("-2");
        Modulus modulus3 = Modulus(BInt("5"));
        EXPECT_EQ(util::ModInverse(operand3, modulus3), BInt("-3"));

        // BInt operand4 = BInt("3");
        // Modulus modulus4 = Modulus(BInt("-7"));
        // EXPECT_EQ(util::ModInverse(operand4, modulus4), BInt("5"));
    }
}// namespace zhejiangfhe