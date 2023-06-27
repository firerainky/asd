//
// Created by 赵启明 on 2023/6/27.
//
#pragma once

#include "numth_util.h"
#include "big_integer_test.h"

namespace zhejiangfhe {
    // 创建继承自 Test 的 test fixture
    template<typename T>
    class NumthUtilTest : public testing::Test {};



    TEST(NumthUtilTest, LeadingZero) {
        uint32_t value(1);
        EXPECT_EQ(util::nlz(value), 8 * sizeof(limbtype) - 1);

        uint32_t value2(2);
        EXPECT_EQ(util::nlz(value2), 8 * sizeof(limbtype) - 2);
    }

}

