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
        using Params  = typename Poly::Params;
        using BigInteger = typename Poly::Integer;

        BigInteger ringDimension(8);
        BigInteger modulus("1099511627873");
        BigInteger rootOfUnity("108163207722");
        std::shared_ptr<Params> params = std::make_shared<Params>(16, ringDimension, modulus, rootOfUnity);
        Poly poly;
        std::vector<int32_t> valueVec;
        valueVec.push_back(1);
        valueVec.push_back(2);
        valueVec.push_back(3);
        valueVec.push_back(4);


        std::vector<int64_t> int64ValueVec;
        int64ValueVec.push_back(1);
        int64ValueVec.push_back(2);
        int64ValueVec.push_back(3);
        int64ValueVec.push_back(4);
        poly = int64ValueVec;

        poly = {"1", "3", "5", "7"};

        poly = 4;
        Poly poly1(params, COEFFICIENT, true);


    }
}