//
// Created by 赵启明 on 2023/5/31.
//

// #include "big_integer.h"
#include "big_integer_modop.h"
#include "big_integer_test.h"
#include "modulus.h"
#include "vector.h"


namespace zhejiangfhe {
    // 创建继承自 Test 的 test fixture
    template<typename T>
    class VectorTest : public testing::Test {};

    // 将 test fixture 与类型列表绑定在一起，这样下面的测试就可以自动对类型列表中的每一种都执行一遍了
    TYPED_TEST_SUITE(VectorTest, LimbTypes);

    TYPED_TEST(VectorTest, Construct) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;


        Modulus modulus = Modulus(BInt("3"));

        Vector v(5);

        v = Vector(5, modulus);

        v = Vector(5, "2");

        v = Vector(5, 4);

        Vector v1(v);

        v = Vector(5, 3, {1, 3, 5, 7});

        v = Vector(5, 3, {"1", "3", "5", "7"});

        std::vector<std::string> vec = {"1", "3", "5", "7"};
        v = Vector(vec, modulus);

        v = Vector(vec, 3);

        v = Vector(vec, "3");


        Vector k = v;
        v[0] = 2;
        EXPECT_EQ(k[0], 1);
    }

    TYPED_TEST(VectorTest, EqualTo) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;

        Vector v1(5);
        Vector v2(5);

        EXPECT_TRUE(v1 == v2);
        EXPECT_EQ(v1, v2);

        v2 = Vector(4);
        EXPECT_TRUE(v1 != v2);

        v1 = Vector(5, 3, {1, 3, 5, 7});
        v2 = Vector(5, 3, {1, 3, 5, 7});
        EXPECT_EQ(v1, v2);

        v2 = Vector(5, 4, {1, 3, 5, 7});
        EXPECT_TRUE(v1 != v2);

        v2 = Vector(4, 3, {1, 3, 5, 7});
        EXPECT_TRUE(v1 != v2);
    }



    TYPED_TEST(VectorTest, SwitchModulus) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;

        Vector v1 = Vector(5, 3, {"1", "3", "5", "7"});

        Modulus modulus = Modulus(BInt("5"));
        v1.SwitchModulus(modulus);

        Vector v2 = Vector(5, 5, {"1", "0", "4", "1"});

        EXPECT_EQ(v1, v2);



        v1 = Vector(5, 7, {"1", "3", "5", "7"});

        modulus = Modulus(BInt("5"));
        v1.SwitchModulus(modulus);

        v2 = Vector(5, 5, {"1", "3", "3", "0"});

        EXPECT_EQ(v1, v2);
    }


    TYPED_TEST(VectorTest, ModAdd) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;

        Vector v1 = Vector(5, 3, {"1", "3", "5", "7"});

        BInt operand = BInt(5);
        Vector v2 = v1.ModAdd(operand);

        Vector v3 = Vector(5, 3, {"0", "2", "1", "0", "2"});

        EXPECT_EQ(v2, v3);
        v1.ModAddEq(operand);
        EXPECT_EQ(v1, v3);


        v1 = Vector(5, 11, {"5", "4", "6", "7", "10"});
        Vector v4 = Vector(5, 11, {"2", "3", "1", "6", "2"});
        v2 = v1.ModAdd(v4);
        Vector v5 = Vector(5, 11, {"7", "7", "7", "2", "1"});
        EXPECT_EQ(v2, v5);

        v1.ModAddEq(v4);
        EXPECT_EQ(v1, v5);
    }


    TYPED_TEST(VectorTest, ModSub) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;

        Vector v1 = Vector(5, 11, {"5", "4", "6", "7", "10"});
        BInt operand = BInt(5);
        Vector v2 = v1.ModSub(operand);
        Vector v3 = Vector(5, 11, {"0", "10", "1", "2", "5"});
        EXPECT_EQ(v2, v3);

        v1.ModSubEq(operand);
        EXPECT_EQ(v1, v3);


        v1 = Vector(5, 11, {"5", "4", "6", "7", "10"});
        Vector v4 = Vector(5, 11, {"2", "3", "1", "6", "2"});
        v2 = v1.ModSub(v4);
        Vector v5 = Vector(5, 11, {"3", "1", "5", "1", "8"});
        EXPECT_EQ(v2, v5);

        v1.ModSubEq(v4);
        EXPECT_EQ(v1, v5);
    }



    TYPED_TEST(VectorTest, ModMul) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;

        Vector v1 = Vector(5, 11, {"5", "4", "6", "7", "10"});
        BInt operand = BInt(5);
        Vector v2 = v1.ModMul(operand);
        Vector v3 = Vector(5, 11, {"3", "9", "8", "2", "6"});
        EXPECT_EQ(v2, v3);

        v1.ModMulEq(operand);
        EXPECT_EQ(v1, v2);


        v1 = Vector(5, 11, {"5", "4", "6", "7", "10"});
        Vector v4 = Vector(5, 11, {"2", "3", "1", "6", "2"});
        v2 = v1.ModMul(v4);
        Vector v5 = Vector(5, 11, {"10", "1", "6", "9", "9"});
        EXPECT_EQ(v2, v5);

        v1.ModMulEq(v4);
        EXPECT_EQ(v1, v5);
    }


    TYPED_TEST(VectorTest, ModExp) {
        using BInt = zhejiangfhe::BigInteger<TypeParam>;
        using Vector = zhejiangfhe::Vector<TypeParam>;

        Vector v1 = Vector(5, 11, {"5", "4", "6", "7", "10"});
        BInt operand = BInt(2);
        Vector v2 = v1.ModExp(operand);
        Vector v3 = Vector(5, 11, {"3", "5", "3", "5", "1"});
        EXPECT_EQ(v2, v3);

        v1.ModExpEq(operand);
        EXPECT_EQ(v1, v3);
    }

}// namespace zhejiangfhe