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

    TYPED_TEST(VectorTest, KDJFKJ) {
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
}// namespace zhejiangfhe