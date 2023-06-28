//
// Created by 赵启明 on 2023/6/28.
//
#include <gtest/gtest.h>
#include "poly.h"
namespace zhejiangfhe {
    // 创建继承自 Test 的 test fixture
    template<typename T>
    class PolyTest : public testing::Test {};

    using LimbTypes = ::testing::Types<Vector<limbtype>>;

    // 将 test fixture 与类型列表绑定在一起，这样下面的测试就可以自动对类型列表中的每一种都执行一遍了
    TYPED_TEST_SUITE(PolyTest, LimbTypes);

    TYPED_TEST(PolyTest, Construct) {
        using Poly = zhejiangfhe::Poly<TypeParam>;


        Poly p();


    }
}