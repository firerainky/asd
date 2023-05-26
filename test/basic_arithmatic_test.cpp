#include "internal/basic_arithmatic.h"
#include <gtest/gtest.h>

TEST(BasicArithmaticTest, nlzTest) {
    EXPECT_EQ(zhejiangfhe::basic_arithmatic::nlz(1), zhejiangfhe::LimbSize - 1);
}