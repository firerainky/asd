#include "big_integer.h"
#include "big_integer_test.h"


// 创建继承自 Test 的 test fixture
template<typename T>
class BigIntegerModTest : public testing::Test {};

// 将 test fixture 与类型列表绑定在一起，这样下面的测试就可以自动对类型列表中的每一种都执行一遍了
TYPED_TEST_SUITE(BigIntegerModTest, LimbTypes);

TYPED_TEST(BigIntegerModTest, Mod) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;

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

TYPED_TEST(BigIntegerModTest, ModMul) {
    using BInt = zhejiangfhe::BigInteger<TypeParam>;
    BInt zero;
    EXPECT_TRUE(zero.ModMul(BInt(), BInt()) == BInt());
}