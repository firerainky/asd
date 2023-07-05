//
// Created by 赵启明 on 2023/6/27.
//

#include "big_integer_test.h"
#include "numth_util.h"

namespace zhejiangfhe {

    TEST(NumthUtilTest, LeadingZero) {
        limbtype value(1);
        EXPECT_EQ(util::nlz(value), 8 * sizeof(limbtype) - 1);

        limbtype value2(2);
        EXPECT_EQ(util::nlz(value2), 8 * sizeof(limbtype) - 2);
    }

}// namespace zhejiangfhe
